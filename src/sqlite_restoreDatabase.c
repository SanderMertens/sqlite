#include <sqlite3.h>

#include "cx.h"
#include "cx_depresolver.h"
#include "sqlite_depInfo.h"
#include "sqlite_util.h"
#include "sqlite_stmt_deser.h"


typedef struct restore_row_info_s {
    int rowCount;
    cx_object object;
} restore_row_info;

static int sqlite_resolveDeclareAction(cx_object o, void *userData) {
    CX_UNUSED(o);
    CX_UNUSED(userData);
    return 0;
}

static int sqlite_resolveDefineAction(cx_object o, void *userData) {
    CX_UNUSED(o);
    CX_UNUSED(userData);
    return 0;
}

static cx_equalityKind sqlite_compareObjectId(cx_type _this, const void *o1, const void *o2) {
    CX_UNUSED(_this);
    int result = strcmp(o1, o2);
    return result > 0 ? CX_GT : (result < 0 ? CX_LT : CX_EQ);
}

static void sqlite_storeObjectsRowNames(cx_rbtree tree, const sqlite_depInfo *in, sqlite_depInfo *out) {
    char *outObject, *outName, *outParent, *outType;
    if (!cx_rbtreeHasKey(tree, in->object, (void **)&outObject)) {
        outObject = cx_malloc(strlen(in->object) + 1);
        strcpy(outObject, in->object);
        cx_rbtreeSet(tree, outObject, outObject);
    }
    if (!cx_rbtreeHasKey(tree, in->name, (void **)&outName)) {
        outName = cx_malloc(strlen(in->name) + 1);
        strcpy(outName, in->name);
        cx_rbtreeSet(tree, outName, outName);
    }
    if (!cx_rbtreeHasKey(tree, in->parent, (void **)&outParent)) {
        outParent = cx_malloc(strlen(in->parent) + 1);
        strcpy(outParent, in->parent);
        cx_rbtreeSet(tree, outParent, outParent);
    }
    if (!cx_rbtreeHasKey(tree, in->type, (void **)&outType)) {
        outType = cx_malloc(strlen(in->type) + 1);
        strcpy(outType, in->type);
        cx_rbtreeSet(tree, outType, outType);
    }
    sqlite_depInfo info = {outObject, outName, outParent, outType, in->state};
    *out = info;
}

static cx_bool tableExists(sqlite3 *db, const char *type) {
    static const char *typeTableQuery = "SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name=?;";
    int code;
    int exists;
    sqlite3_stmt *stmt = NULL;
    if ((code = sqlite3_prepare_v2(db, typeTableQuery, strlen(typeTableQuery) + 1, &stmt, NULL)) != SQLITE_OK) {
        cx_error("could not check existance of table %s", type);
        goto error;
    }
    if ((code = sqlite3_bind_text(stmt, 1, type, -1, SQLITE_STATIC)) != SQLITE_OK) {
        cx_error("could not bind type name %s", type);
        goto error;
    }
    if ((code = sqlite3_step(stmt)) != SQLITE_ROW) {
        cx_error("could not retrieve existance of table %s", type);
        goto error;
    }
    exists = sqlite3_column_int(stmt, 0);
    if (sqlite3_finalize(stmt) != SQLITE_OK) {
        cx_critical("cannot finalize statement");
        goto error;
    }
    return exists;
error:
    return 0;
}

static cx_bool sameTypes(const cx_object object, const char *dbType, char *storeTypeFullname) {
    cx_object storeType = cx_typeof(object);
    cx_fullname(storeType, storeTypeFullname);
    cx_bool result = strcmp(storeTypeFullname, dbType) == 0;
    return result;
}

static cx_bool dependenciesSatisfied(const sqlite_depInfo *info, cx_object *parent, cx_type *type) {
    *parent = cx_resolve(NULL, (char*)info->parent);
    *type = cx_type(cx_resolve(NULL, (char *)info->type));
    cx_bool result = TRUE;
    result = result && parent != NULL;
    result = result && type != NULL;
    return result;
}

static int prepareQuery(char *type, char *object, char **query) {
    int length;
    length = snprintf(NULL, 0, "SELECT * FROM \"%s\" WHERE \"ObjectId\" = '%s';", type, object);
    if (length < 0) {
        cx_error("error calculating size for query of %s", object);
        goto error;
    }
    *query = cx_malloc(length + 1);
    if (sprintf(*query, "SELECT * FROM \"%s\" WHERE \"ObjectId\" = '%s';", type, object) < 0) {
        cx_dealloc(*query);
        cx_error("error writing query for %s", object);
        goto error;
    }
    return 0;
error:
    return 1;
}

static int retrieveObject(sqlite3 *db, const sqlite_depInfo *info, cx_object o) {
    char *query;
    int rowCount;
    sqlite3_stmt *stmt = NULL;
    int code;
    prepareQuery(info->type, info->object, &query);
    if (sqlite3_prepare_v2(db, query, strlen(query) + 1, &stmt, NULL) != SQLITE_OK) {
        cx_error("could not prepare query to retrieve %s", info->object);
        goto error;
    }
    cx_dealloc(query);
    rowCount = 0;
    while ((code = sqlite3_step(stmt)) == SQLITE_ROW) {
        if (rowCount) {
            cx_critical("found more than one row for %s", info->object);
            goto finalize;
        }
        if (sqlite_stmt_deser(stmt, o)) {
            cx_error("could not deserialize %s", info->object);
            goto finalize;
        }
        rowCount++;
    }
    if (code != SQLITE_DONE) {
        cx_error("cannot finish restoring database, code %d", code);
        goto finalize;
    }
    if (sqlite3_finalize(stmt) != SQLITE_OK) {
        cx_critical("cannot finalize statement");
        goto error;
    }
    return 0;
error:
    return 1;
finalize:
    if (sqlite3_finalize(stmt) != SQLITE_OK) {
        cx_critical("cannot finalize statement");
        goto error;
    }
    return 1;
}

static int sqlite_storeRow(sqlite3 *db, const sqlite_depInfo *info) {
    cx_object storeObj;
    if ((storeObj = cx_resolve(NULL, (char *)info->object))) {
        if (!isCore(storeObj)) {
            cx_id storeTypeFullname;
            if (sameTypes(storeObj, (char *)info->type, storeTypeFullname)) {
                cx_debug("%s exists in db and store with equal types", info->object);
            } else {
                cx_error("%s is declared in store as %s and in database as %s",
                    info->object, storeTypeFullname, info->type);
                goto error;
            }
        }
    } else {
        cx_object parent;
        cx_type type;
        if (dependenciesSatisfied(info, &parent, &type)) {
            cx_debug("%s can be declared", info->object);
            cx_object o = cx_declare(parent, (char *)info->name, type);
            cx_debug("%s state=%d, defined=%d", info->object, info->state, CX_DEFINED);
            if (info->state & CX_DEFINED) {
                if (tableExists(db, info->type)) {
                    printf("table exists!\n");
                    retrieveObject(db, info, o);
                    cx_define(o);
                } else {
                    cx_error("cannot define object %s;"
                        " no table exists for the type %s",
                        info->object, info->type);
                }
            } else  {
                cx_debug("%s not defined in database", info->object);
            }
        } else {
            /* TODO insert into dependency resolver */
            printf("dependencies not satisfied for %s\n", info->object);
        }
    }
    return 0;
error:
    return 1;
}

static int sqlite_freeTree(void *o, void *data) {
    CX_UNUSED(data);
    cx_dealloc(o);
    return 0;
}

int sqlite_restoreDatabase(sqlite3 *db) {
    static const char *retrieveQuery = "SELECT ObjectId, Name, Parent, Type, State FROM Objects;";
    int code;
    // Do I need to dispose of this object?
    cx_rbtree objects = cx_rbtreeNew_w_func(sqlite_compareObjectId);
    sqlite3_stmt *stmt = NULL;
    cx_depresolver resolver = cx_depresolver__create(
        sqlite_resolveDeclareAction, sqlite_resolveDefineAction, NULL);
    CX_UNUSED(resolver);
    code = sqlite3_prepare_v2(db, retrieveQuery, strlen(retrieveQuery) + 1, &stmt, NULL);
    if (code != SQLITE_OK) {
        cx_error("cannot prepare statement, error code %d", code);
        goto error;
    }
    while ((code = sqlite3_step(stmt)) == SQLITE_ROW) {
        char *_object = (char *)sqlite3_column_text(stmt, 0);
        char *_name = (char *)sqlite3_column_text(stmt, 1);
        char *_parent = (char *)sqlite3_column_text(stmt, 2);
        char *_type = (char *)sqlite3_column_text(stmt, 3);
        int state = sqlite3_column_int(stmt, 4);
        sqlite_depInfo depInfoIn = {_object, _name, _parent, _type, state};
        sqlite_depInfo depInfoOut;
        sqlite_storeObjectsRowNames(objects, &depInfoIn, &depInfoOut);
        sqlite_storeRow(db, &depInfoOut);
    }
    if (code != SQLITE_DONE) {
        cx_error("cannot finish restoring database, code %d", code);
        sqlite3_finalize(stmt);
        goto error;
    }
    if (sqlite3_finalize(stmt) != SQLITE_OK) {
        cx_critical("cannot finalize statement");
        goto error;
    }
    cx_rbtreeWalk(objects, sqlite_freeTree, NULL);
    return 0;
error:
    cx_rbtreeWalk(objects, sqlite_freeTree, NULL);
    return 1;
}

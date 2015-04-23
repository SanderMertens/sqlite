#include <sqlite3.h>

#include "cx.h"
#include "cx_depresolver.h"
#include "sqlite_depInfo.h"


int sqlite_resolveDeclareAction(cx_object o, void *userData) {
    CX_UNUSED(o);
    CX_UNUSED(userData);
    return 0;
}

int sqlite_resolveDefineAction(cx_object o, void *userData) {
    CX_UNUSED(o);
    CX_UNUSED(userData);
    return 0;
}

cx_equalityKind sqlite_compareObjectId(cx_type _this, const void *o1, const void *o2) {
    CX_UNUSED(_this);
    int result = strcmp(o1, o2);
    return result > 0 ? CX_GT : (result < 0 ? CX_LT : CX_EQ);
}

void sqlite_storeRow(cx_rbtree tree, const sqlite_depInfo *in, sqlite_depInfo *out) {
    char *outObject, *outParent, *outType;
    if (!cx_rbtreeHasKey(tree, in->object, (void **)&outObject)) {
        outObject = cx_malloc(strlen(in->object) + 1);
        strcpy(outObject, in->object);
        cx_rbtreeSet(tree, outObject, outObject);
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
    sqlite_depInfo info = {outObject, outParent, outType, in->state};
    *out = info;
}

void sqlite_setDependencies(cx_depresolver resolver,
        const char *object, const char *parent, const char *type,
        cx_uint8 parentState) {
    printf("name=%s, parent=%s, type=%s, parentState=%d\n", object, parent, type, parentState);
    /* TODO remove cast */
    cx_depresolver_depend(resolver, (void *)object, CX_DECLARED, (void *)parent, parentState);
    if (parentState == CX_DECLARED) {
        /* TODO remove cast */
        cx_depresolver_depend(resolver, (void *)parent, CX_DEFINED, (void *)object, CX_DEFINED);
    }
    /* TODO remove cast */
    cx_depresolver_depend(resolver, (void *)object, CX_DECLARED, (void *)type, CX_DEFINED);
}

static cx_bool tableExists(sqlite3 *db, const char *type) {
    static const char *typeTableQuery = "SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name=?;";
    int code;
    int exists;
    sqlite3_stmt *stmt = NULL;
    if ((code = sqlite3_prepare_v2(db, typeTableQuery, sizeof(typeTableQuery), &stmt, NULL)) != SQLITE_OK) {
        cx_error("could not check existance of table %s", type);
        goto error;
    }
    if ((code = sqlite3_bind_text(stmt, 0, type, -1, SQLITE_STATIC)) != SQLITE_OK) {
        cx_error("could not bind type name %s", type);
        goto error;
    }
    if ((code = sqlite3_step(stmt)) != SQLITE_OK) {
        cx_error("could not retrieve existance of table %s", type);
        goto error;
    }
    exists = sqlite3_column_int(stmt, 0);
    return exists;
error:
    return 0;
}

static int storeNewest(sqlite3 *db, cx_object object, const sqlite_depInfo *dbObj) {
    /*
     * storeObj
     */
    CX_UNUSED(db);
    CX_UNUSED(object);
    CX_UNUSED(dbObj);
    return 0;
}

static int dbNewest(const sqlite3 *db, cx_object object, const sqlite_depInfo *dbObj) {
    CX_UNUSED(db);
    CX_UNUSED(object);
    CX_UNUSED(dbObj);
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
    result = result && parent == NULL;
    result = result && type == NULL;
    return result;
}

int sqlite_restoreRow(sqlite3 *db, const sqlite_depInfo *info) {
    cx_object storeObj; 
    if ((storeObj = cx_resolve(NULL, (char *)info->object))) {
        cx_id storeTypeFullname;
        if (sameTypes(storeObj, (char *)info->type, storeTypeFullname)) {
            /* resolve */
        } else {
            cx_error("%s is locally declared as %s and in the database as %s",
                info->object, storeTypeFullname, info->type);
            goto error;
        }
    } else {
        cx_object parent;
        cx_type type;
        if (dependenciesSatisfied(info, &parent, &type)) {
            cx_declare(parent, (char *)info->object, type);
            if (info->state & CX_DEFINED) {
                if (tableExists(db, info->type)) {

                } else {
                    cx_error("cannot defined object %s;"
                        " no table exists for the type %s",
                        info->object, info->type);
                }
            }
        } else {
            /* TODO insert into dependency resolver */
        }
    }
    return 0;
error:
    return 1;
}

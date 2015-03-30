/* sqlite_connector.c
 *
 * This file contains the implementation for the generated interface.
 *
 *    Don't mess with the begin and end tags, since these will ensure that modified
 *    code in interface functions isn't replaced when code is re-generated.
 */

#include "sqlite.h"
#include "sqlite__meta.h"

/* $header() */
#include <sqlite3.h>

#include "cx_files.h"
#include "sqlite_ser.h"

static cx_bool isBlacklisted(cx_object o) {
    cx_bool result = FALSE;
    if (o == root_o) {
        result = TRUE;
    } else if (cx_checkAttr(o, CX_ATTR_SCOPED)) {
        cx_object p = o;
        while ((p = cx_parentof(p)) != root_o) {
            if (p == cortex_o) {
                result = TRUE;
                break;
            }
        }
    }
    return result;
}

static cx_void bootstrapDatabase(sqlite_connector _this) {
    const char bootstrap[] =
        "CREATE TABLE IF NOT EXISTS \"Objects\" ("
        "\"ObjectId\" TEXT PRIMARY KEY, "
        "\"Name\" TEXT, "
        "\"Parent\" TEXT REFERENCES \"Objects\"(\"ObjectId\"), "
        "\"Type\" TEXT REFERENCES \"Objects\"(\"ObjectId\")"
        ");"
        "INSERT OR IGNORE INTO \"Objects\" (\"ObjectId\", \"Name\", \"Parent\") VALUES "
        "(\"::\", NULL, NULL)"
        ;
    char *errmsg = NULL;
    // TODO delete
    if (sqlite3_exec((sqlite3 *)_this->db, "PRAGMA foreign_keys = OFF;", NULL, NULL, &errmsg) != SQLITE_OK) {
        cx_error(errmsg);
        sqlite3_free(errmsg);
    }
    if (sqlite3_exec((sqlite3 *)_this->db, bootstrap, NULL, NULL, &errmsg) != SQLITE_OK) {
        cx_critical("%s", errmsg);
        sqlite3_free(errmsg);
    }
}

static cx_void restore(sqlite_connector _this) {
    static const char const query[] = "SELECT \"ObjectId\", \"Name\", \"Parent\", \"Type\" "
                                      "FROM Objects;";
    sqlite3_stmt *stmt = NULL;
    int code;
    if ((code = sqlite3_prepare_v2((sqlite3 *)_this->db, query, sizeof(query), &stmt, NULL)) != SQLITE_OK) {
        cx_error("could not prepare statement, error code = %s", code);
        goto error;
    }
    while ((code = sqlite3_step(stmt)) == SQLITE_ROW) {
        const unsigned char *name = sqlite3_column_text(stmt, 1);
        const unsigned char *parent = sqlite3_column_text(stmt, 2);
        const unsigned char *type =sqlite3_column_text(stmt, 3);
        printf("name=%s, parent=%s, type=%s\n", name, parent, type);
    }
    if (code != SQLITE_DONE) {
        cx_error("could not finish restoring database");
        goto error;
    }
    if (sqlite3_finalize(stmt) != SQLITE_OK) {
        cx_critical("could not finalize statement");
        goto error;
    }
error:;
}
/* $end */

/* ::cortex::sqlite::connector::construct() */
cx_int16 sqlite_connector_construct(sqlite_connector _this) {
/* $begin(::cortex::sqlite::connector::construct) */
    char *errmsg;
    if (!cx_fileTest(_this->filename)) {
        cx_touch(_this->filename);
    }
    if (sqlite3_open(_this->filename, (sqlite3 **)&(_this->db)) != SQLITE_OK) {
        cx_error("%s", sqlite3_errmsg((sqlite3 *)_this->db));
    }
    /* TODO PUT FOREIGN KEYS BACK ON */
    if (sqlite3_exec((sqlite3 *)_this->db, "PRAGMA foreign_keys = OFF;", NULL, NULL, &errmsg) != SQLITE_OK) {
        cx_error(errmsg);
        sqlite3_free(errmsg);
    }
    bootstrapDatabase(_this);
    restore(_this);
    sqlite_connector_onDeclare_o->mask = CX_ON_DECLARE | CX_ON_SCOPE | CX_ON_SELF;
    sqlite_connector_onDefine_o->mask = CX_ON_DEFINE | CX_ON_SCOPE | CX_ON_SELF;
    sqlite_connector_onUpdate_o->mask = CX_ON_UPDATE | CX_ON_SCOPE | CX_ON_SELF;
    cx_listen(root_o, sqlite_connector_onDeclare_o, _this);
    cx_listen(root_o, sqlite_connector_onDefine_o, _this);
    cx_listen(root_o, sqlite_connector_onUpdate_o, _this);
    return 0;
/* $end */
}

/* ::cortex::sqlite::connector::destruct() */
cx_void sqlite_connector_destruct(sqlite_connector _this) {
/* $begin(::cortex::sqlite::connector::destruct) */
    CX_UNUSED(_this);
    if (sqlite3_close((sqlite3 *)_this->db) != SQLITE_OK) {
        cx_critical("error closing database");
    }
/* $end */
}

/* ::cortex::sqlite::connector::onDeclare() */
cx_void sqlite_connector_onDeclare(sqlite_connector _this, cx_object *observable, cx_object *source) {
/* $begin(::cortex::sqlite::connector::onDeclare) */
    CX_UNUSED(_this);
    CX_UNUSED(observable);
    char *errmsg = NULL;
    /* TODO run dependency algorithm */
    {
        struct cx_serializer_s serializer = sqlite_ser_declare(CX_PRIVATE, CX_NOT, CX_SERIALIZER_TRACE_NEVER);
        struct sqlite_ser data = {NULL, NULL, 0, 0, 0, 0};
        cx_serialize(&serializer, source, &data);
        cx_debug(data.buffer);
        if (sqlite3_exec((sqlite3 *)_this->db, data.buffer, NULL, NULL, &errmsg) != SQLITE_OK) {
            cx_error((char *)sqlite3_errmsg((sqlite3 *)_this->db));
            cx_error(errmsg);
            sqlite3_free(errmsg);
        }
    }
    /* TODO run dependency algorithm */
    if (isBlacklisted(source) && cx_instanceof(cx_type(cx_type_o), source)) {
        if (cx_type(source)->kind != CX_COMPOSITE || cx_interface(source)->kind != CX_INTERFACE) {
            struct cx_serializer_s serializer = sqlite_ser_type(CX_PRIVATE, CX_NOT, CX_SERIALIZER_TRACE_NEVER);
            struct sqlite_ser data = {NULL, NULL, 0, 0, 0, 0};
            cx_metaWalk(&serializer, cx_type(source), &data);
            cx_debug(data.buffer);
            if (sqlite3_exec((sqlite3 *)_this->db, data.buffer, NULL, NULL, &errmsg) != SQLITE_OK) {
                cx_error((char *)sqlite3_errmsg((sqlite3 *)_this->db));
                cx_error(errmsg);
                sqlite3_free(errmsg);
            }
        }
    }
/* $end */
}

/* ::cortex::sqlite::connector::onDefine() */
cx_void sqlite_connector_onDefine(sqlite_connector _this, cx_object *observable, cx_object *source) {
/* $begin(::cortex::sqlite::connector::onDefine) */
    CX_UNUSED(_this);
    CX_UNUSED(source);
    char *errmsg;
    if (!isBlacklisted(observable)) {
        if (cx_instanceof(cx_type(cx_type_o), observable)) {
            struct cx_serializer_s serializer = sqlite_ser_type(CX_PRIVATE, CX_NOT, CX_SERIALIZER_TRACE_NEVER);
            struct sqlite_ser data = {NULL, NULL, 0, 0, 0, 0};
            cx_metaWalk(&serializer, cx_type(observable), &data);
            cx_debug("%s\n", data.buffer);
            if (sqlite3_exec((sqlite3 *)_this->db, data.buffer, NULL, NULL, &errmsg) != SQLITE_OK) {
                cx_error((char *)sqlite3_errmsg((sqlite3 *)_this->db));
                cx_error(errmsg);
                sqlite3_free(errmsg);
            }
        } else if (cx_typeof(observable)->kind != CX_VOID) {
            struct cx_serializer_s serializer = sqlite_ser_define(CX_PRIVATE, CX_NOT, CX_SERIALIZER_TRACE_NEVER);
            struct sqlite_ser data = {NULL, NULL, 0, 0, 0, 0};
            cx_serialize(&serializer, observable, &data);
            cx_debug("%s\n", data.buffer);
            if (sqlite3_exec((sqlite3 *)_this->db, data.buffer, NULL, NULL, &errmsg) != SQLITE_OK) {
                cx_error((char *)sqlite3_errmsg((sqlite3 *)_this->db));
                cx_error(errmsg);
                sqlite3_free(errmsg);
            }
        }
    }
/* $end */
}

/* ::cortex::sqlite::connector::onUpdate() */
cx_void sqlite_connector_onUpdate(sqlite_connector _this, cx_object *observable, cx_object *source) {
/* $begin(::cortex::sqlite::connector::onUpdate) */
    CX_UNUSED(_this);
    CX_UNUSED(source);
    char *errmsg;
    struct cx_serializer_s serializer = sqlite_ser_update(CX_PRIVATE, CX_NOT, CX_SERIALIZER_TRACE_NEVER);
    struct sqlite_ser data = {NULL, NULL, 0, 0, 0, 0};
    cx_serialize(&serializer, observable, &data);
    cx_debug("%s\n", data.buffer);
    if (sqlite3_exec((sqlite3 *)_this->db, data.buffer, NULL, NULL, &errmsg) != SQLITE_OK) {
        cx_error((char *)sqlite3_errmsg((sqlite3 *)_this->db));
        cx_error(errmsg);
        sqlite3_free(errmsg);
    }
/* $end */
}

/* ::cortex::sqlite::connector::post(event e) */
cx_void sqlite_connector_post(sqlite_connector _this, cx_event e) {
/* $begin(::cortex::sqlite::connector::post) */
    CX_UNUSED(_this);
    CX_UNUSED(e);
/* $end */
}

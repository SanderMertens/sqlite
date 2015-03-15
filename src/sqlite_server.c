/* sqlite_server.c
 *
 * This file contains the implementation for the generated interface.
 *
 *    Don't mess with the begin and end tags, since these will ensure that modified
 *    code in interface functions isn't replaced when code is re-generated.
 */

#include "sqlite.h"
#include "sqlite__meta.h"

/* $header() */
#include "cx_files.h"
#include "ser_sqlite.h"
#include <sqlite3.h>

static sqlite3 *db;

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

static cx_void bootstrapDatabase(void) {
    // TODO GET THIS FROM bootstrap.sql
    const char bootstrap[] =
        "CREATE TABLE IF NOT EXISTS \"Objects\" ("
        "\"ObjectId\" INTEGER PRIMARY KEY,"
        "\"Name\" TEXT,"
        "\"Parent\" NUMERIC REFERENCES \"Objects\"(\"ObjectId\")"
        ");"
        ;
    char *errMsg = NULL;
    if (sqlite3_exec(db, "PRAGMA foreign_keys = ON;", NULL, NULL, &errMsg) != SQLITE_OK) {
        cx_critical(errMsg);
        sqlite3_free(errMsg);
    }
    if (sqlite3_exec(db, bootstrap, NULL, NULL, &errMsg) != SQLITE_OK) {
        cx_critical(errMsg);
        sqlite3_free(errMsg);
    }
}
/* $end */

/* ::cortex::sqlite::server::construct() */
cx_int16 sqlite_server_construct(sqlite_server _this) {
/* $begin(::cortex::sqlite::server::construct) */
    if (!cx_fileTest(_this->filename)) {
        cx_touch(_this->filename);
    }
    if (sqlite3_open(_this->filename, &db) != SQLITE_OK) {
        cx_critical("%s", sqlite3_errmsg(db));
    }
    bootstrapDatabase();
    sqlite_server_onDeclare_o->mask = CX_ON_DECLARE | CX_ON_SCOPE | CX_ON_SELF;
    sqlite_server_onDefine_o->mask = CX_ON_DEFINE | CX_ON_SCOPE | CX_ON_SELF;
    sqlite_server_onUpdate_o->mask = CX_ON_UPDATE | CX_ON_SCOPE | CX_ON_SELF;
    cx_listen(root_o, sqlite_server_onDeclare_o, _this);
    cx_listen(root_o, sqlite_server_onDefine_o, _this);
    cx_listen(root_o, sqlite_server_onUpdate_o, _this);
    return 0;
/* $end */
}

/* ::cortex::sqlite::server::destruct() */
cx_void sqlite_server_destruct(sqlite_server _this) {
/* $begin(::cortex::sqlite::server::destruct) */
    CX_UNUSED(_this);
    sqlite3_close(db);
/* $end */
}

/* ::cortex::sqlite::server::onDeclare() */
cx_void sqlite_server_onDeclare(sqlite_server _this, cx_object *observable, cx_object *source) {
/* $begin(::cortex::sqlite::server::onDeclare) */
    CX_UNUSED(_this);
    CX_UNUSED(observable);
    char *errMsg = NULL;
    if (!isBlacklisted(source)) {
        struct cx_serializer_s serializer = cx_sqlite_declare_ser(CX_PRIVATE, CX_NOT, CX_SERIALIZER_TRACE_NEVER);
        cx_sqlite_ser_t sqlData = {NULL, NULL, 0, 0, 0};
        cx_serialize(&serializer, source, &sqlData);
        sqlite3 *db;
        if (sqlite3_open(_this->filename, &db) != SQLITE_OK) {
            cx_critical((char *)sqlite3_errmsg(db));
        }
        if (sqlite3_exec(db, "PRAGMA foreign_keys = ON;", NULL, NULL, &errMsg) != SQLITE_OK) {
            cx_critical(errMsg);
            sqlite3_free(errMsg);
        }
        if (sqlite3_exec(db, sqlData.buffer, NULL, NULL, &errMsg) != SQLITE_OK) {
            cx_critical((char *)sqlite3_errmsg(db));
            cx_critical(errMsg);
            sqlite3_free(errMsg);
        }
        if (sqlite3_close(db) != SQLITE_OK) {
            cx_critical("error closing database");
        }
        cx_debug(sqlData.buffer);
    } else if (cx_instanceof(cx_type(cx_type_o), source)) {
        struct cx_serializer_s serializer = cx_type_ser(CX_PRIVATE, CX_NOT, CX_SERIALIZER_TRACE_NEVER);
        cx_sqlite_ser_t sqlData = {NULL, NULL, 0, 0, 0};
        cx_metaWalk(&serializer, cx_type(source), &sqlData);
        cx_debug(sqlData.buffer);
    }
/* $end */
}

/* ::cortex::sqlite::server::onDefine() */
cx_void sqlite_server_onDefine(sqlite_server _this, cx_object *observable, cx_object *source) {
/* $begin(::cortex::sqlite::server::onDefine) */
    CX_UNUSED(_this);
    CX_UNUSED(source);
    if (!isBlacklisted(observable)) {
        if (cx_instanceof(cx_type(cx_type_o), observable)) {
            struct cx_serializer_s serializer = cx_type_ser(CX_PRIVATE, CX_NOT, CX_SERIALIZER_TRACE_NEVER);
            cx_sqlite_ser_t sqlData = {NULL, NULL, 0, 0, 0};
            cx_metaWalk(&serializer, cx_type(observable), &sqlData);
            printf("%s\n", sqlData.buffer); // TODO DELETE
        } else {
            struct cx_serializer_s serializer = cx_sqlite_define_ser(CX_PRIVATE, CX_NOT, CX_SERIALIZER_TRACE_NEVER);
            cx_sqlite_ser_t sqlData = {NULL, NULL, 0, 0, 0};
            cx_serialize(&serializer, observable, &sqlData);
            printf("%s\n", sqlData.buffer);
        }
    }
/* $end */
}

/* ::cortex::sqlite::server::onUpdate() */
cx_void sqlite_server_onUpdate(sqlite_server _this, cx_object *observable, cx_object *source) {
/* $begin(::cortex::sqlite::server::onUpdate) */
    CX_UNUSED(_this);
    CX_UNUSED(observable);
    CX_UNUSED(source);
    printf("This is an update of %s!\n", cx_nameof(observable));
/* $end */
}

/* ::cortex::sqlite::server::post(event e) */
cx_void sqlite_server_post(sqlite_server _this, cx_event e) {
/* $begin(::cortex::sqlite::server::post) */
    CX_UNUSED(_this);
    CX_UNUSED(e);
/* $end */
}

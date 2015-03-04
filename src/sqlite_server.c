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

static const char filename[] = "db.sqlite";

static cx_bool isBlacklisted(cx_object *o) {
    cx_bool result = FALSE;
    if (o == root_o) {
        result = TRUE;
    } else if (cx_checkAttr(o, CX_ATTR_SCOPED)) {
        cx_object *p = o;
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
    const char bootstrap[] = "CREATE TABLE IF NOT EXISTS Objects (Name TEXT, Parent NUMERIC, FOREIGN KEY(Parent) REFERENCES Objects(Oid) );";
    char *errMsg = NULL;
    sqlite3 *db;
    if (sqlite3_open(filename, &db) != SQLITE_OK) {
        cx_critical("%s", sqlite3_errmsg(db));
    }
    if (sqlite3_exec(db, "PRAGMA foreign_keys = ON;", NULL, NULL, &errMsg) != SQLITE_OK) {
        cx_critical(errMsg);
        sqlite3_free(errMsg);
    }
    if (sqlite3_exec(db, bootstrap, NULL, NULL, &errMsg) != SQLITE_OK) {
        cx_critical(errMsg);
        sqlite3_free(errMsg);
    }
    sqlite3_close(db);
}

/* $end */

/* ::cortex::sqlite::server::construct() */
cx_int16 sqlite_server_construct(sqlite_server _this) {
/* $begin(::cortex::sqlite::server::construct) */
    if (!cx_fileTest(filename)) {
        cx_touch(filename);
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
/* $end */
}

/* ::cortex::sqlite::server::onDeclare() */
cx_void sqlite_server_onDeclare(sqlite_server _this, cx_object *observable, cx_object *source) {
/* $begin(::cortex::sqlite::server::onDeclare) */
    CX_UNUSED(_this);
    CX_UNUSED(observable);
    char *errMsg = NULL;
    if (!isBlacklisted(source)) {
        sqlite3 *db;
        struct cx_serializer_s serializer = cx_sqlite_declare_ser(CX_PRIVATE, CX_NOT, CX_SERIALIZER_TRACE_NEVER);
        cx_sqlite_ser_t sqlData = {NULL, NULL, 0, 0, 0};
        cx_serialize(&serializer, source, &sqlData);
        if (sqlite3_open(filename, &db) != SQLITE_OK) {
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
    }
/* $end */
}

/* ::cortex::sqlite::server::onDefine() */
cx_void sqlite_server_onDefine(sqlite_server _this, cx_object *observable, cx_object *source) {
/* $begin(::cortex::sqlite::server::onDefine) */
    CX_UNUSED(_this);
    CX_UNUSED(observable);
    if (!isBlacklisted(source)) {
        struct cx_serializer_s serializer = cx_sqlite_define_ser(CX_PRIVATE, CX_NOT, CX_SERIALIZER_TRACE_NEVER);
        cx_sqlite_ser_t sqlData = {NULL, NULL, 0, 0, 0};
        cx_serialize(&serializer, source, &sqlData);
        printf("%s\n", sqlData.buffer);
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

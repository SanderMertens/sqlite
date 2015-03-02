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
#include "ser_sqlite.h"
#include "sqlite.h"
#include "sqlite3.h"
/* $end */

/* ::cortex::sqlite::server::construct() */
cx_int16 sqlite_server_construct(sqlite_server _this) {
/* $begin(::cortex::sqlite::server::construct) */
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
    struct cx_serializer_s serializer = cx_sqlite_declare_ser(CX_PRIVATE, CX_NOT, CX_SERIALIZER_TRACE_NEVER);
    cx_sqlite_ser_t sqlData = {NULL, NULL, 0, 0, 0};
    cx_serialize(&serializer, source, &sqlData);
    printf("%s\n", sqlData.buffer);
/* $end */
}

/* ::cortex::sqlite::server::onDefine() */
cx_void sqlite_server_onDefine(sqlite_server _this, cx_object *observable, cx_object *source) {
/* $begin(::cortex::sqlite::server::onDefine) */
    CX_UNUSED(_this);
    CX_UNUSED(observable);
    struct cx_serializer_s serializer = cx_sqlite_define_ser(CX_PRIVATE, CX_NOT, CX_SERIALIZER_TRACE_NEVER);
    cx_sqlite_ser_t sqlData = {NULL, NULL, 0, 0, 0};
    cx_serialize(&serializer, source, &sqlData);
    printf("%s\n", sqlData.buffer);
/* $end */
}

/* ::cortex::sqlite::server::onUpdate() */
cx_void sqlite_server_onUpdate(sqlite_server _this, cx_object *observable, cx_object *source) {
/* $begin(::cortex::sqlite::server::onUpdate) */
    CX_UNUSED(_this);
    CX_UNUSED(observable);
    CX_UNUSED(source);
    printf("This is an update!");
/* $end */
}

/* ::cortex::sqlite::server::post(event e) */
cx_void sqlite_server_post(sqlite_server _this, cx_event e) {
/* $begin(::cortex::sqlite::server::post) */
    CX_UNUSED(_this);
    CX_UNUSED(e);
/* $end */
}

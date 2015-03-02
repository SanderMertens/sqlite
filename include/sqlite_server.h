/* sqlite_server.h
 *
 * This file contains generated code. Do not modify!
 */

#ifndef sqlite_server_H
#define sqlite_server_H

#include "cortex.h"
#include "sqlite__type.h"

#include "sqlite__api.h"

#include "sqlite__meta.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ::cortex::sqlite::server::construct() */
cx_int16 sqlite_server_construct(sqlite_server _this);

/* ::cortex::sqlite::server::destruct() */
cx_void sqlite_server_destruct(sqlite_server _this);

/* ::cortex::sqlite::server::onDeclare() */
cx_void sqlite_server_onDeclare(sqlite_server _this, cx_object *observable, cx_object *source);

/* ::cortex::sqlite::server::onDefine() */
cx_void sqlite_server_onDefine(sqlite_server _this, cx_object *observable, cx_object *source);

/* ::cortex::sqlite::server::onUpdate() */
cx_void sqlite_server_onUpdate(sqlite_server _this, cx_object *observable, cx_object *source);

/* ::cortex::sqlite::server::post(event e) */
cx_void sqlite_server_post(sqlite_server _this, cx_event e);

#ifdef __cplusplus
}
#endif
#endif


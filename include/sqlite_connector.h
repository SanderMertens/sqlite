/* sqlite_connector.h
 *
 * This file contains generated code. Do not modify!
 */

#ifndef sqlite_connector_H
#define sqlite_connector_H

#include "cortex.h"
#include "sqlite__type.h"

#include "sqlite__api.h"

#include "sqlite__meta.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ::cortex::sqlite::connector::construct() */
cx_int16 sqlite_connector_construct(sqlite_connector _this);

/* ::cortex::sqlite::connector::destruct() */
cx_void sqlite_connector_destruct(sqlite_connector _this);

/* ::cortex::sqlite::connector::onDeclare() */
cx_void sqlite_connector_onDeclare(sqlite_connector _this, cx_object *observable, cx_object *source);

/* ::cortex::sqlite::connector::onDefine() */
cx_void sqlite_connector_onDefine(sqlite_connector _this, cx_object *observable, cx_object *source);

/* ::cortex::sqlite::connector::onUpdate() */
cx_void sqlite_connector_onUpdate(sqlite_connector _this, cx_object *observable, cx_object *source);

/* ::cortex::sqlite::connector::post(event e) */
cx_void sqlite_connector_post(sqlite_connector _this, cx_event e);

#ifdef __cplusplus
}
#endif
#endif


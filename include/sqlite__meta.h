/* sqlite__meta.h
 *
 * Loads objects in database.
 * This file contains generated code. Do not modify!
 */

#ifndef sqlite_META_H
#define sqlite_META_H

#include "cortex.h"

#include "sqlite__type.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ::cortex */
extern cx_object _o;

/* ::cortex::sqlite */
extern cx_package sqlite_o;

/* ::cortex::sqlite::server */
extern cx_class sqlite_server_o;

/* ::cortex::sqlite::server::construct() */
extern cx_method sqlite_server_construct_o;

/* ::cortex::sqlite::server::destruct() */
extern cx_method sqlite_server_destruct_o;

/* ::cortex::sqlite::server::events */
extern cx_member sqlite_server_events_o;

/* ::cortex::sqlite::server::onDeclare() */
extern cx_observer sqlite_server_onDeclare_o;

/* ::cortex::sqlite::server::onDefine() */
extern cx_observer sqlite_server_onDefine_o;

/* ::cortex::sqlite::server::post(event e) */
extern cx_method sqlite_server_post_o;

/* ::cortex::sqlite::server::thread */
extern cx_member sqlite_server_thread_o;

int sqlite_load(void);
#ifdef __cplusplus
}
#endif
#endif


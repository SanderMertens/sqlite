/* sqlite__type.h
 *
 *    Type-definitions for C-language.
 *    This file contains generated code. Do not modify!
 */

#ifndef sqlite__type_H
#define sqlite__type_H

#include "cortex.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Casting macro's for classes */
#define sqlite_server(o) ((sqlite_server)o)

/* Type definitions */
CX_LIST(cx_event_list);

/*  ::cortex::sqlite::server */
CX_CLASS(sqlite_server);

CX_CLASS_DEF(sqlite_server) {
    cx_word thread;
    cx_event_list events;
};

#ifdef __cplusplus
}
#endif
#endif


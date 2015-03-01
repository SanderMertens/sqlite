/* sqlite__api.h
 *
 *    API convenience functions for C-language.
 *    This file contains generated code. Do not modify!
 */

#ifndef sqlite__API_H
#define sqlite__API_H

#include "sqlite__type.h"
#include "cortex.h"

#ifdef __cplusplus
extern "C" {
#endif
/* ::cortex::sqlite::server */
sqlite_server sqlite_server__new(void);
sqlite_server sqlite_server__declare(cx_object _parent, cx_string _name);
int sqlite_server__define(sqlite_server _this);
sqlite_server sqlite_server__create(void);


/* <0x9673d58> */
#define cx_event_list__foreach(list, elem) \
    cx_iter elem##_iter = cx_llIter(list);\
    cx_event elem;\
    while(cx_iterHasNext(&elem##_iter)) {\
        elem = cx_iterNext(&elem##_iter);

void cx_event_list__insert(cx_event_list list, cx_event element);
void cx_event_list__append(cx_event_list list, cx_event element);
cx_event cx_event_list__takeFirst(cx_event_list list);
cx_event cx_event_list__last(cx_event_list list);
void cx_event_list__clear(cx_event_list list);
cx_event cx_event_list__get(cx_event_list list, cx_uint32 index);
cx_uint32 cx_event_list__size(cx_event_list list);

#ifdef __cplusplus
}
#endif
#endif


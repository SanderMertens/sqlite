/* sqlite__api.c
 *
 *    API convenience functions for C-language.
 *    This file contains generated code. Do not modify!
 */

#include "sqlite__meta.h"
#include "sqlite__api.h"

sqlite_server sqlite_server__new(void) {
    return cx_new(cx_type(sqlite_server_o));
}

sqlite_server sqlite_server__declare(cx_object _parent, cx_string _name) {
    return cx_declare(_parent, _name, cx_type(sqlite_server_o));
}

int sqlite_server__define(sqlite_server _this) {
    return cx_define(_this);
}

sqlite_server sqlite_server__create(void) {
    sqlite_server _this;
    _this = cx_new(cx_type(sqlite_server_o));
    if (cx_define(_this)) {
        cx_free(_this);
        _this = NULL;
    }
    return _this;
}

void cx_event_list__insert(cx_event_list list, cx_event element) {
    cx_llInsert(list, (void*)element);
}

void cx_event_list__append(cx_event_list list, cx_event element) {
    cx_llAppend(list, (void*)element);
}

cx_event cx_event_list__takeFirst(cx_event_list list) {
    return (cx_event)cx_llTakeFirst(list);
}

cx_event cx_event_list__last(cx_event_list list) {
    return (cx_event)cx_llLast(list);
}

void cx_event_list__clear(cx_event_list list) {
    void *element;

    while((element = cx_llTakeFirst(list))) {
        cx_free(element);
    }
}

cx_event cx_event_list__get(cx_event_list list, cx_uint32 index) {
    return (cx_event)cx_llGet(list, index);
}

cx_uint32 cx_event_list__size(cx_event_list list) {
    return cx_llSize(list);
}


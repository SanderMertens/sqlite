/* sqlite__wrapper.c
 *
 * This file contains wrapper functions for ::cortex::sqlite.
 */

#include "sqlite.h"
#include "sqlite__meta.h"

void __sqlite_server_construct(cx_function f, void *result, void *args) {
    CX_UNUSED(f);
    CX_UNUSED(args);
    *(cx_int16*)result = sqlite_server_construct(
        *(void**)args);
}

void __sqlite_server_destruct(cx_function f, void *result, void *args) {
    CX_UNUSED(f);
    CX_UNUSED(args);
    CX_UNUSED(result);
    sqlite_server_destruct(
        *(void**)args);
}

void __sqlite_server_onDeclare(cx_function f, void *result, void *args) {
    CX_UNUSED(f);
    CX_UNUSED(result);
    sqlite_server_onDeclare(
        *(void**)args,
        *(void**)((intptr_t)args + sizeof(void*)),
        *(void**)((intptr_t)args + sizeof(void*) + sizeof(void*)));
}

void __sqlite_server_onDefine(cx_function f, void *result, void *args) {
    CX_UNUSED(f);
    CX_UNUSED(result);
    sqlite_server_onDefine(
        *(void**)args,
        *(void**)((intptr_t)args + sizeof(void*)),
        *(void**)((intptr_t)args + sizeof(void*) + sizeof(void*)));
}

void __sqlite_server_post(cx_function f, void *result, void *args) {
    CX_UNUSED(f);
    CX_UNUSED(result);
    sqlite_server_post(
        *(void**)args,
        *(cx_event*)((intptr_t)args + sizeof(void*)));
}

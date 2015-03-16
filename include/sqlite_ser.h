#include <stdio.h>

#include "cortex.h"
#include "cx_serializer.h"

typedef struct sqlite_ser {
    /**/
    cx_string buffer;
    cx_string ptr;
    /**/
    unsigned int length;
    unsigned int maxlength;
    unsigned int itemCount;
    unsigned int depth; /* how many many bases this has */
} sqlite_ser_t;

struct cx_serializer_s sqlite_ser_declare(cx_modifier access, cx_operatorKind accessKind, cx_serializerTraceKind trace);
struct cx_serializer_s sqlite_ser_define(cx_modifier access, cx_operatorKind accessKind, cx_serializerTraceKind trace);
struct cx_serializer_s sqlite_ser_update(cx_modifier access, cx_operatorKind accessKind, cx_serializerTraceKind trace);
struct cx_serializer_s sqlite_ser_type(cx_modifier access, cx_operatorKind accessKind, cx_serializerTraceKind trace);

size_t escsqlstr(char *out, size_t n, const char *in);

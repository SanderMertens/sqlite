#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#include "cx_generator.h"
#include "cx_serializer.h"
#include "cx_string.h"
#include "cortex.h"
#include "sqlite_ser.h"

/*
 * https://www.sqlite.org/lang_update.html
 */

/* agreed not to change anything except the first argument of the signature */
static cx_bool cx_ser_appendstrbuff(struct sqlite_ser* data, char* str) {
    if (!data->ptr) {
        data->ptr = data->buffer;
    }
    if (!data->ptr) {
        data->buffer = strdup(str);
        data->ptr = data->buffer;
    } else {
        cx_uint32 length, bufferLength;

        if (!data->length) {
            data->buffer = cx_realloc(data->buffer, strlen(data->buffer) + strlen(str) + 1);
            data->ptr = data->buffer;
        }

        length = strlen(str);
        bufferLength = strlen(data->buffer);

        if (data->maxlength && ((bufferLength + length) >= data->maxlength)) {
            strncat(data->ptr, str, data->maxlength - bufferLength);
            goto maxreached;
        } else {
            strcat(data->ptr, str);
        }
    }

    return TRUE;
maxreached:
    return FALSE;
}

/* agreed not to change anything except the first argument of the signature */
/* Append string to serializer-result */
static cx_bool cx_ser_appendstr(struct sqlite_ser* data, cx_string fmt, ...) {
    char alloc[1024];
    char *buff = alloc;
    va_list args;
    cx_uint32 memRequired;
    cx_uint32 result = TRUE;

    if (data) {
        va_list argcpy;
        va_copy(argcpy, args);
        va_start(args, fmt);
        memRequired = vsnprintf(buff, 1024, fmt, args);
        if (memRequired >= 1024) {
            buff = cx_malloc(memRequired + 1);
            vsprintf(buff, fmt, argcpy);
        }
        va_end(args);
        result = cx_ser_appendstrbuff(data, buff);
        if (buff != alloc) {
            cx_dealloc(buff);
        }
    }

    return result;
}

static cx_int16 serializePrimitive(cx_serializer s, cx_value *v, void *userData) {
    CX_UNUSED(s);
    struct sqlite_ser *data = userData;
    cx_string valueString = NULL;
    if (sqlite_ser_serializePrimitiveValue(v, &valueString)) {
        goto error;
    }
    if (!cx_ser_appendstr(data, "%s", valueString)) {
        goto finished;
    }
    cx_dealloc(valueString);
    return 0;
finished:
    return 1;
error:
    return -1;
}

static cx_int16 serializeReference(cx_serializer s, cx_value *v, void *userData) {
    CX_UNUSED(s);
    struct sqlite_ser *data = userData;
    cx_string valueString = NULL;
    if (sqlite_ser_serializeReferenceValue(v, &valueString)) {
        goto error;
    }
    if (!cx_ser_appendstr(data, "%s", valueString)) {
        goto finished;
    }
    cx_dealloc(valueString);
    return 0;
finished:
    return 1;
error:
    return -1;
}

static cx_int16 serializeMember(cx_serializer s, cx_value *v, void *userData) {
    struct sqlite_ser *data = userData;
    if (data->itemCount) {
        if (!cx_ser_appendstr(data, ", ")) {
            goto finished;
        }
    }
    if (cx_serializeValue(s, v, data)) {
        goto error;
    }
    data->itemCount++;
    return 0;
finished:
    return 1;
error:
    return -1;
}

static cx_int16 serializeComposite(cx_serializer s, cx_value* v, void* userData) {
    struct sqlite_ser *_data = userData;
    struct sqlite_ser data = *_data;
    if (cx_serializeMembers(s, v, &data)) {
        goto error;
    }
    _data->itemCount = data.itemCount;
    _data->buffer = data.buffer;
    _data->ptr = data.ptr;
    return 0;
error:
    return -1;
}

static cx_int16 serializeCollection(cx_serializer s, cx_value* v, void* userData) {
    CX_UNUSED(s);
    CX_UNUSED(v);
    struct sqlite_ser *_data = userData;
    struct sqlite_ser data = *_data;
    if (!cx_ser_appendstr(&data, " NULL ")) {
        goto finished;
    }
    _data->itemCount = data.itemCount;
    _data->buffer = data.buffer;
    _data->ptr = data.ptr;
    return 0;
finished:
    return 1;
}

static cx_int16 serializeObject(cx_serializer s, cx_value* v, void* userData) {
    struct sqlite_ser *data = userData;
    cx_id typeFullname;
    cx_id fullname;
    cx_fullname(cx_valueType(v), typeFullname);
    cx_fullname(cx_valueObject(v), fullname);
    if (!cx_ser_appendstr(data, "INSERT INTO \"%s\" VALUES ('%s', ", typeFullname, fullname)) {
        goto finished;
    }
    if (cx_serializeValue(s, v, data)) {
        goto error;        
    }
    if (!cx_ser_appendstr(data, ");")) {
        goto finished;
    }
    return 0;
finished:
    return 1;
error:
    return -1;
}

struct cx_serializer_s sqlite_ser_define(cx_modifier access, cx_operatorKind accessKind, cx_serializerTraceKind trace) {
    struct cx_serializer_s s;
    cx_serializerInit(&s);

    s.access = access;
    s.accessKind = accessKind;
    s.traceKind = trace;
    s.reference = serializeReference;
    s.program[CX_PRIMITIVE] = serializePrimitive;
    s.program[CX_COMPOSITE] = serializeComposite;
    s.program[CX_COLLECTION] = serializeCollection;
    s.metaprogram[CX_MEMBER] = serializeMember;
    s.metaprogram[CX_OBJECT] = serializeObject;
    return s;
}

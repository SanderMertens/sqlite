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
    unsigned int depth;
    struct sqlite_ser *data = userData;
    cx_string memberName;
    if (data->itemCount) {
        if (!cx_ser_appendstr(data, ", ")) {
            goto finished;
        }
    }
    if (!cx_ser_appendstr(data, "\"")) {
        goto finished;
    }
    depth = data->depth;
    memberName = cx_nameof(v->is.member.t);
    while (depth--) {
        if (!cx_ser_appendstr(data, "_")) {
            goto finished;
        }    
    }
    if (!cx_ser_appendstr(data, "%s\"=", memberName)) {
        goto finished;
    }
    cx_serializeValue(s, v, data);
    data->itemCount++;
    return 0;
finished:
    return 1;
}

// static cx_int16 serializeComposite(cx_serializer s, cx_value* v, void* userData) {
//     struct sqlite_ser data = *(struct sqlite_ser*)userData;
//     data.itemCount = 0;
//     cx_type type = cx_valueType(v);
//     if (type->kind == CX_COMPOSITE) {
//         if (cx_serializeMembers(s, v, &data)) {
//             goto error;
//         }
//     }
//     ((struct sqlite_ser*)userData)->buffer = data.buffer;
//     ((struct sqlite_ser*)userData)->ptr = data.ptr;
//     return 0;
// error:
//     return -1;
// }

static cx_int16 serializeBase(cx_serializer s, cx_value *v, void *userData) {
    struct sqlite_ser *_data = userData;
    struct sqlite_ser data = *_data;
    data.depth++;
    cx_int16 errors;
    errors = cx_serializeValue(s, v, &data);
    if (errors == 1) {
        goto finished;
    } else if (errors == -1) {
        goto error;
    }
    _data->itemCount = data.itemCount;
    _data->buffer = data.buffer;
    _data->ptr = data.ptr;
    return 0;
finished:
    return 1;
error:
    return -1;
}

/*
 * It should be responsibility of the underlying value kind to add
 * "(column1, column2) VALUES (value1, value2)".
 */
static cx_int16 serializeObject(cx_serializer s, cx_value* v, void* userData) {
    struct sqlite_ser *data = userData;
    cx_id typeFullname;
    cx_id fullname;
    cx_fullname(cx_valueType(v), typeFullname);
    cx_fullname(cx_valueObject(v), fullname);
    if (!cx_ser_appendstr(data, "UPDATE \"%s\" SET ", typeFullname)) {
        goto finished;
    }
    if (cx_valueType(v)->kind == CX_PRIMITIVE) {
        if (!cx_ser_appendstr(data, "\"Value\"=")) {
            goto finished;
        }
    }
    if (cx_serializeValue(s, v, data)) {
        goto error;
    }
    if (!cx_ser_appendstr(data, " WHERE \"ObjectId\"='%s';", fullname)) {
        goto finished;
    }
    return 0;
finished:
    return 1;
error:
    return -1;
}

struct cx_serializer_s sqlite_ser_update(cx_modifier access, cx_operatorKind accessKind, cx_serializerTraceKind trace) {
    struct cx_serializer_s s;
    cx_serializerInit(&s);

    s.access = access;
    s.accessKind = accessKind;
    s.traceKind = trace;
    s.reference = serializeReference;
    s.program[CX_PRIMITIVE] = serializePrimitive;
    s.metaprogram[CX_MEMBER] = serializeMember;
    s.metaprogram[CX_BASE] = serializeBase;
    s.metaprogram[CX_OBJECT] = serializeObject;
    return s;
}

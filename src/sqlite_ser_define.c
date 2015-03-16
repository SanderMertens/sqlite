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

static cx_int16 cx_ser_primitive(cx_serializer s, cx_value *v, void *userData) {
    CX_UNUSED(s);
    cx_type type;
    cx_void *value;
    struct sqlite_ser *data = userData;
    cx_int16 result;

    type = cx_valueType(v);
    value = cx_valueValue(v);

    switch (cx_primitive(type)->kind) {
        case CX_BINARY:
            if (!cx_ser_appendstr(data, "%s", "0")) {
                goto finished;
            }
            break;
        case CX_BITMASK:
            {
                cx_string valueString = NULL;
                result = cx_convert(cx_primitive(cx_uint32_o), value, cx_primitive(cx_string_o), &valueString);
                if (result) {
                    goto error;
                }
                if (!cx_ser_appendstr(data, "%s", valueString)) {
                    goto finished;
                }
                cx_dealloc(valueString);
            }
            break;
        case CX_ENUM:
            {
                cx_string valueString = NULL;
                result = cx_convert(cx_primitive(cx_int32_o), value, cx_primitive(cx_string_o), &valueString);
                if (result) {
                    goto error;
                }
                if (!cx_ser_appendstr(data, "%s", valueString)) {
                    goto finished;
                }
                cx_dealloc(valueString);
            }
            break;
        case CX_CHARACTER:
        case CX_TEXT:
            {
                cx_string valueString = NULL;
                result = cx_convert(cx_primitive(type), value, cx_primitive(cx_string_o), &valueString);
                if (result) {
                    goto error;
                }
                if (!*(cx_string *)value) {
                    if (!cx_ser_appendstr(data, "NULL")) {
                        goto finished;
                    }
                } else {
                    if (!cx_ser_appendstr(data, "'")) {
                        goto finished;
                    }
                    size_t length = escsqlstr(NULL, 0, valueString);
                    char *escapedString = cx_malloc(length + 1);
                    escsqlstr(escapedString, length, valueString);
                    if (!cx_ser_appendstr(data, escapedString)) {
                        goto finished;
                    }
                    cx_dealloc(escapedString);
                    if (!cx_ser_appendstr(data, "'")) {
                        goto finished;
                    }
                }
                cx_dealloc(valueString);
            }
            break;
        case CX_BOOLEAN:
            {
                cx_bool *b = cx_valueValue(v);
                if (!cx_ser_appendstr(data, "%s", (*b) ? "88" : "77")) {
                    goto finished;
                }
                
            }
            break;
        case CX_INTEGER:
        case CX_FLOAT:
        case CX_UINTEGER:
            {
                cx_string valueString = NULL;
                result = cx_convert(cx_primitive(type), value, cx_primitive(cx_string_o), &valueString);
                if (result) {
                    goto error;
                }
                if (!cx_ser_appendstr(data, valueString)) {
                    goto finished;
                }
                cx_dealloc(valueString);
            }
            break;
        case CX_ALIAS:
            cx_critical("Cannot serialize alias");
            break;
    }
    return 0;
finished:
    return 1;
error:
    return -1;
}

static cx_int16 cx_ser_reference(cx_serializer s, cx_value *v, void *userData) {
    CX_UNUSED(s);
    CX_UNUSED(v);
    struct sqlite_ser *data = userData;
    if (!cx_ser_appendstr(data, "NULL")) {
        goto finished;
    }
    return 0;
finished:
    return 1;
}

static cx_int16 cx_ser_item(cx_serializer s, cx_value *v, void *userData) {
    struct sqlite_ser *data = userData;
    printf("%s - %d\n", cx_nameof(v->is.member.t), data->itemCount);
    if (data->itemCount) {
        if (!cx_ser_appendstr(data, ", ")) {
            goto finished;
        }
    }
    cx_serializeValue(s, v, data);
    data->itemCount++;
    return 0;
finished:
    return 1;
}

static cx_int16 cx_ser_composite(cx_serializer s, cx_value* v, void* userData) {
    struct sqlite_ser *_data = userData;
    struct sqlite_ser data = *_data;
    // data.itemCount = 0;
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

static cx_int16 cx_ser_collection(cx_serializer s, cx_value* v, void* userData) {
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
// error:
    // return -1;
}

// static cx_int16 cx_ser_base(cx_serializer s, cx_value* v, void* userData) {
//     CX_UNUSED(s);
//     CX_UNUSED(v);
//     CX_UNUSED(userData);
//     return 0;
// }

static cx_int16 cx_ser_object(cx_serializer s, cx_value* v, void* userData) {
    struct sqlite_ser *data = userData;
    cx_object *o = cx_valueObject(v);
    cx_id columnName;
    cx_fullname(cx_typeof(o), columnName);
    if (!cx_ser_appendstr(data, "INSERT INTO \"%s\" VALUES (NULL, ", columnName)) {
        goto finished;
    }
    cx_serializeValue(s, v, data);
    if (!cx_ser_appendstr(data, ");")) {
        goto finished;
    }
    if (cx_instanceof(cx_typeof(cx_type_o), o)) {
        
    }
    return 0;
finished:
    return 1;
}

struct cx_serializer_s sqlite_ser_define(cx_modifier access, cx_operatorKind accessKind, cx_serializerTraceKind trace) {
    struct cx_serializer_s s;
    cx_serializerInit(&s);

    s.access = access;
    s.accessKind = accessKind;
    s.traceKind = trace;
    s.program[CX_PRIMITIVE] = cx_ser_primitive;
    s.reference = cx_ser_reference;
    s.program[CX_COMPOSITE] = cx_ser_composite;
    s.program[CX_COLLECTION] = cx_ser_collection;
    // s.metaprogram[CX_ELEMENT] = cx_ser_item;
    s.metaprogram[CX_MEMBER] = cx_ser_item;
    // s.metaprogram[CX_BASE] = cx_ser_base;
    s.metaprogram[CX_OBJECT] = cx_ser_object;
    return s;
}

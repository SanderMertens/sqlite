#include "cortex.h"

/*
 * Writes at most `n` - 1 bytes from `in` into `out`, plus a null character.
 * Returns the total length that should have been written without the null character.
 * Escapes all single quotes as two single quotes.
 * https://www.sqlite.org/lang_expr.html
 */
size_t sqlite_ser_escsqlstr(char *out, size_t n, const char *in) {
    size_t len = 0;
    const char *p = in;
    char *q = out;
    char c;
    while ((c = *p++)) {
        if (c != '\'') {
            len++;
            if (n && (size_t)(q - out) < n) {
                *q++ = c;
            }
        } else {
            len += 2;
            if (n && (size_t)(q - out) < n) {
                *(q + 1) = *q = '\'';
                q += 2;
            }
        }
    }
    if (n) {
        *q = '\0';
    }
    return len;
}

/*
 * Returns 0 on success.
 */
cx_int16 sqlite_ser_serializePrimitiveValue(cx_value *v, cx_string *buffer) {
    cx_type type;
    void *value;
    cx_int16 errors;

    cx_assert(*buffer == NULL, "Buffer not NULL");

    type = cx_valueType(v);
    value = cx_valueValue(v);

    switch (cx_primitive(type)->kind) {
        case CX_BITMASK:
            errors = cx_convert(cx_primitive(cx_uint32_o), value, cx_primitive(cx_string_o), buffer);
            break;
        case CX_ENUM:
            errors = cx_convert(cx_primitive(cx_int32_o), value, cx_primitive(cx_string_o), buffer);
            break;
        case CX_CHARACTER:
        case CX_TEXT:
            if (*(cx_string *)value == NULL) {
                *buffer = cx_malloc(sizeof("NULL"));
                strcpy(*buffer, "NULL");
            } else {
                size_t length = sqlite_ser_escsqlstr(NULL, 0, *(cx_string *)value);
                *buffer = cx_malloc(1 + length + 1 + 1);
                (*buffer)[0] = '\'';
                sqlite_ser_escsqlstr(*buffer + 1, length, *(cx_string *)value);
                (*buffer)[1 + length] = '\'';
                (*buffer)[1 + length + 1] = '\0';
            }
            errors = 0;
            break;
        case CX_BOOLEAN:
            *buffer = cx_malloc(sizeof("1"));
            (*buffer)[0] = (*((cx_bool *)v)) ? '1' : '0';
            (*buffer)[1] = '\0';
            errors = 0;
            break;
        case CX_BINARY:
        case CX_INTEGER:
        case CX_FLOAT:
        case CX_UINTEGER:
            errors = cx_convert(cx_primitive(type), value, cx_primitive(cx_string_o), buffer);
            break;
        case CX_ALIAS:
            *buffer = cx_malloc(sizeof("NULL"));
            strcpy(*buffer, "NULL");
            errors = 0;
            cx_critical("Cannot serialize alias");
            break;
    }
    if (errors) {
        goto error;
    }
    return 0;
error:
    return -1;
}

cx_int16 sqlite_ser_serializeReferenceValue(cx_value *v, cx_string *buffer) {
    cx_assert(*buffer == NULL, "buffer is not NULL");
    cx_object o = *(cx_object *)cx_valueValue(v);
    if (o && cx_checkAttr(o, CX_ATTR_SCOPED)) {
        cx_id referenceFullName;
        cx_fullname(o, referenceFullName);
        *buffer = cx_malloc(1 + strlen(referenceFullName) + 1 + 1);
        sprintf(*buffer, "'%s'", referenceFullName);
    } else {
        *buffer = cx_malloc(sizeof("NULL"));
        strcpy(*buffer, "NULL");
    }
    return 0;
}

#include "cortex.h"

/*
 * Writes at most `n` - 1 bytes from `in` into `out`, plus a null character.
 * Returns the total length that should have been written without the null character.
 * Escapes all single quotes as two single quotes.
 * https://www.sqlite.org/lang_expr.html
 */
size_t escsqlstr(char *out, size_t n, const char *in) {
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
 * Provides a fully scoped name that represents a Cortex type's corresponding table name.
 */
// int cx_sqlite_solveName(cx_object o, cx_id buffer) {
//     cx_char *ptr;
//     if (!o) {
//         *buffer = '\0';
//     } else if (!cx_checkAttr(o, CX_ATTR_SCOPED)) {
//         goto error;
//     } else {
//         cx_object stack[CX_MAX_SCOPE_DEPTH];
//         cx_uint32 depth = 0;
//         do {
//             stack[depth++] = o;
//         } while((o = cx_parentof(o)));
//         ptr = buffer;
//         if (depth == 1) {
//             *ptr++ = '_';
//         } else {
//             while (depth--) {
//                 cx_string name;
//                 o = stack[depth];
//                 if ((name = cx_nameof(o))) {
//                     size_t len;
//                     *ptr++ = '_';
//                     if ((size_t)(ptr - buffer) >= (sizeof(cx_id) - 1)) {
//                         goto finished;
//                     }
//                     len = strlen(name);
//                     if ((size_t)(ptr - buffer + len) >= (sizeof(cx_id) - 1)) {
//                         goto finished;
//                     }
//                     memcpy(ptr, name, len + 1);
//                     ptr += len;
//                 }
//             }
//         }
//         *ptr = '\0';
//     }
//     return 0;
// finished:
//     *ptr = '\0';
//     return 1;
// error:
//     return -1;
// }

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
                size_t length = escsqlstr(NULL, 0, *(cx_string *)value);
                *buffer = cx_malloc(1 + length + 1 + 1);
                (*buffer)[0] = '\'';
                escsqlstr(*buffer + 1, length, *(cx_string *)value);
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

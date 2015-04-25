
#include <sqlite3.h>

#include "cx.h"
#include "cx_string.h"


static void complainCannotCoerceValue(char *text, cx_type type) {
    cx_id typeFullname;
    char *escapedString;
    size_t length;
    cx_fullname(type, typeFullname);
    length = stresc(NULL, 0, text);
    escapedString = cx_malloc(length + 1);
    stresc(escapedString, length + 1, text);
    cx_error("cannot coerce \"%s\" into %s", escapedString, typeFullname);
    cx_dealloc(escapedString);
}

// static int sqlite_stmt_convertSignedInteger(void *to, cx_type type, char *text) {
//     cx_int64 v = atol(text);
//     if (v == 0) {
//         cx_assert(!strcmp(text, "0"), "Not deserializing int value: %s", text);
//     }
//     int result = cx_convert(cx_primitive(cx_int32_o), &v, cx_primitive(type), &toValue);
//     return result;
// }

// static int sqlite_stmt_convertUnsignedInteger(void *to, cx_type type, char *text) {
//     cx_uint64 v = atoi(text);
//     if (v == 0) {
//         cx_assert(!strcmp(text, "0"), "Not deserializing int value: %s", text);
//     }
//     int result = cx_convert(cx_primitive(cx_int32_o), &v, cx_primitive(type), &toValue);
//     return result;
// }

static int sqlite_stmt_setPrimitive(void *o, cx_type type, char *text) {
    int result = 0;
    switch (cx_primitive(type)->kind) {
        case CX_ALIAS:
            cx_error("alias primitive cannot be deserialized");
            result = 1;
            break;
        case CX_BINARY:
        case CX_INTEGER:
        case CX_FLOAT:
        case CX_UINTEGER:
        case CX_CHARACTER:
            result = cx_convert(cx_primitive(cx_string_o), &text, cx_primitive(type), o);
            break;
        case CX_BOOLEAN:
        case CX_BITMASK:
        case CX_ENUM:
            {
                cx_int32 v;
                result = cx_convert(cx_primitive(cx_string_o), &text, cx_primitive(cx_int32_o), &v);
                if (!result) {
                    result = cx_convert(cx_primitive(cx_int32_o), &v, cx_primitive(type), o);
                }
            }
            break;
        case CX_TEXT:
            *(char **)o = cx_strdup(text);
            break;
    }
    // if (result) {
    //     complainCannotCoerceValue(text, type);
    // } else if (cx_primitive(type)->kind != CX_TEXT) {
    //     memcpy(o, &toValue, type->size);
    // } else {
    // }
    return result;
}

static int sqlite_stmt_deserPrimitive(sqlite3_stmt *stmt, cx_object o) {
    {
        cx_id fullname;
        int count = sqlite3_column_count(stmt);
        char *columnName = (char *)sqlite3_column_name(stmt, 1);
        cx_fullname(o, fullname);
        cx_assert(count == 2, "did not find exactly 2 columns for %s (found %d)", fullname, count);
        cx_assert(stricmp(columnName, "Value") == 0, "Column not Value for %s", fullname);
    }
    char *text = (char *)sqlite3_column_text(stmt, 1);
    cx_debug("value found for %s: %s", cx_nameof(o), text);
    if (sqlite_stmt_setPrimitive(o, cx_typeof(o), text)) {
        goto error;
    }
    return 0;
error:
    return 1;
}

static int sqlite_stmt_deserComposite(sqlite3_stmt *stmt, cx_object o) {
    cx_type type = cx_typeof(o);
    int columnCount = sqlite3_column_count(stmt);
    int i;
    for (i = 0; i < columnCount; i++) {
        char *columnName = (char *)sqlite3_column_name(stmt, i);
        char *columnText;
        cx_member member;
        if (strcmp(columnName, "ObjectId") == 0) {
            continue;
        }
        columnText = (char *)sqlite3_column_text(stmt, i);
        member = cx_resolve(type, columnName);
        if (member == NULL) {
            cx_id typeFullname;
            cx_fullname(cx_typeof(o), typeFullname);
            cx_critical("cannot find member %s of type %s", columnName, typeFullname);
        }
        {
            cx_type memberType = member->type;
            void *offset = CX_OFFSET(o, member->offset);
            if (sqlite_stmt_setPrimitive(offset, memberType, columnText)) {
                cx_error("cannot set member %s", columnName);
                goto error;
            }
        }
    }
    return 0;
error:
    return 1;
}

int sqlite_stmt_deser(sqlite3_stmt *stmt, cx_object o) {
    int result = 0;
    {
        int count = sqlite3_column_count(stmt);
        cx_assert(count > 1, "Could not find at least 1 column");
        char *objectId = (char *)sqlite3_column_name(stmt, 0);
        cx_assert(stricmp(objectId, "ObjectId") == 0, "First column is not ObjectId");
    }
    switch(cx_typeof(o)->kind) {
        case CX_ANY:
            cx_error("cannot deserialize void object");
            result = 1;
            break;
        case CX_COLLECTION:
            cx_error("cannot deserialize collections");
            break;
        case CX_ITERATOR:
            cx_error("cannot deserialize iterator");
        case CX_COMPOSITE:
            result = sqlite_stmt_deserComposite(stmt, o);
            break;
        case CX_PRIMITIVE:
            result = sqlite_stmt_deserPrimitive(stmt, o);
            break;
        case CX_VOID:
            cx_error("cannot deserialize void object");
            result = 1;
            break;
    }
    return result;
}

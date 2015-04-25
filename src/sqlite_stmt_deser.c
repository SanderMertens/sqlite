
#include <sqlite3.h>

#include "cx.h"

static int sqlite_stmt_deserPrimitive(sqlite3_stmt *stmt, cx_object o) {
    CX_UNUSED(stmt);
    CX_UNUSED(o);
    return 0;
}

static void complainCannotCoerceValue(cx_object o, const char *columnName,
        cx_type memberType, const char *columnText) {
    cx_id fn, tfn, mtfn;
    cx_fullname(o, fn);
    cx_fullname(cx_typeof(o), tfn);
    cx_fullname(memberType, mtfn);
    cx_error("%s - cannot deserialize column %s from type %s; "
        "\"%s\" cannot be coerced into %s",
        fn, columnName, tfn, columnText, mtfn);
}

static int setMember(void *o, cx_type type, char *text, char *columnName) {
    void *toValue;
    int result = 0;
    switch (cx_primitive(type)->kind) {
        case CX_BINARY:
        case CX_BITMASK:
        case CX_BOOLEAN:
        case CX_ENUM:
        case CX_FLOAT:
        case CX_INTEGER:
        case CX_UINTEGER:
            {
                int v = atoi(text);
                result = cx_convert(cx_primitive(cx_int32_o), &v, cx_primitive(type), &toValue);
            }
            break;
        case CX_TEXT:
            toValue = text;
            break;
    }
    if (result) {
        complainCannotCoerceValue(o, columnName, type, text);
    } else {
        memcpy(o, &toValue, type->size);
    }
    return result;
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
            if (setMember(offset, memberType, columnText, columnName)) {
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
    switch(cx_typeof(o)->kind) {
        case CX_PRIMITIVE:
            sqlite_stmt_deserPrimitive(stmt, o);
            break;
        case CX_COMPOSITE:
            sqlite_stmt_deserComposite(stmt, o);
            break;
    }
    return 0;
}
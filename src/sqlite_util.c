#include "cx.h"

cx_bool isCore(cx_object o) {
    cx_bool result = FALSE;
    if (o == root_o) {
        result = TRUE;
    } else if (cx_checkAttr(o, CX_ATTR_SCOPED)) {
        cx_object p = o;
        while ((p = cx_parentof(p)) != root_o) {
            if (p == cortex_o) {
                result = TRUE;
                break;
            }
        }
    }
    return result;
}

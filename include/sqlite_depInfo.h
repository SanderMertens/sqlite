#include "cx.h"

typedef struct sqlite_depInfo_s {
    const char *object;
    const char *parent;
    const char *type;
    const cx_int8 state;
} sqlite_depInfo;

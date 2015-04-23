#include "cx.h"

typedef struct sqlite_depInfo_s {
    char *object;
    char *parent;
    char *type;
    cx_int8 state;
} sqlite_depInfo;

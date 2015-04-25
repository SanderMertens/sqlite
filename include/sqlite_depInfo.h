
#ifndef SQLITE_DEPINFO_H
#define SQLITE_DEPINFO_H

#include "cx.h"


typedef struct sqlite_depInfo_s {
    char *object;
    char *name;
    char *parent;
    char *type;
    cx_int8 state;
} sqlite_depInfo;

#endif /* SQLITE_DEPINFO_H */

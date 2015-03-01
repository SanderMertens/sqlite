/* sqlite__meta.c
 *
 * Loads objects in database.
 * This file contains generated code. Do not modify!
 */

#include "sqlite__type.h"

/* Variable definitions */
cx_package sqlite_o;

/* Load objects in database */
int sqlite_load(void) {
    cx_object _a_; /* Used for resolving anonymous objects */
    _a_ = NULL;

    /* Declare ::sqlite */
    sqlite_o = cx_declare(root_o, "sqlite", cx_type(cx_package_o));
    if (!sqlite_o) {
        cx_error("sqlite_load: failed to declare object '::sqlite'.");
        goto error;
    }

    /* Define ::sqlite */
    if (!cx_checkState(sqlite_o, CX_DEFINED)) {
        sqlite_o->url = NULL;
        if (cx_define(sqlite_o)) {
            cx_error("sqlite_load: failed to define object '::sqlite'.");
            goto error;
        }
    }

    if (_a_) {
        cx_free(_a_);
    }

    return 0;
error:
    return -1;
}

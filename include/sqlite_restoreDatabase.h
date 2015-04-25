
#ifndef SQLITE_RESTOREDATABASE_H
#define SQLITE_RESTOREDATABASE_H

#include "cx.h"
#include "sqlite_depInfo.h"

// int sqlite_resolveDeclareAction(cx_object o, void *userData);

// int sqlite_resolveDefineAction(cx_object o, void *userData);

// /* Compares id's as strings. */
// cx_equalityKind sqlite_compareObjectId(cx_type _this, const void *o1, const void *o2);

// /*
//  * Stores a copy of the object's, parent's, and type's fullnames. Returns the copy.
//  */
// void sqlite_storeRow(cx_rbtree tree, const sqlite_depInfo *in, sqlite_depInfo *out);

// /*
//  * Sets the dependencies between an object, its parent, and its type.
//  */
// void sqlite_setDependencies(cx_depresolver resolver, cx_rbtree tree,
//         const char *object, const char *parent, const char *type,
//         cx_uint8 parentState);

// void sqlite_restoreRow(sqlite3 *db, const sqlite_depInfo *row);

int sqlite_restoreDatabase(sqlite3 *db);

#endif /* SQLITE_RESTOREDATABASE_H */

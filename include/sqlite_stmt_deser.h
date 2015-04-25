
#ifndef SQLITE_STMT_DESER_H
#define SQLITE_STMT_DESER_H

#ifdef __cplusplus
extern "C" {
#endif


int sqlite_stmt_deser(sqlite3_stmt *stmt, cx_object o);

#ifdef __cplusplus
}
#endif

#endif /* SQLITE_STMT_DESER_H */

//
// Created by realv on 2/24/2024.
//
#ifndef KERNEL_SQLITE_SQLITE_DEFS_H
#define KERNEL_SQLITE_SQLITE_DEFS_H
#include <linux/printk.h>
#include <linux/mutex.h>
#include <linux/sched.h>
#include <linux/vmalloc.h>    

// int ksqlite_open_db(char *, sqlite3 **);
// sqlite3_int64 make_data_id(sqlite3 *, unsigned int, unsigned int);
// int ksqlite_start_transaction(sqlite3*, char **);

#define HAVE_STDIO_H 1
#define HAVE_STDLIB_H 1

#define HAVE_STRING_H 1


// #define HAVE_INTTYPES_H 0: No support for inttypes.
// #define HAVE_STDINT_H 1
#define SQLITE_OMIT_DATETIME_FUNCS
#define SQLITE_ZERO_MALLOC

#define HAVE_STRINGS_H 1
#define HAVE_SYS_STAT_H 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_UNISTD_H 1
#define STDC_HEADERS 1
#define HAVE_DLFCN_H 1  
#define HAVE_FDATASYNC 1
#define HAVE_USLEEP 1
#define HAVE_LOCALTIME_R 1
#define HAVE_GMTIME_R 1
#define HAVE_DECL_STRERROR_R 1
#define HAVE_STRERROR_R 1
#define HAVE_POSIX_FALLOCATE 1
#define HAVE_ZLIB_H 1
#define _REENTRANT 1
#define SQLITE_THREADSAFE 2

//#define SQLITE_ENABLE_RTREE
#define SQLITE_ENABLE_GEOPOLY
#define SQLITE_HAVE_ZLIB
#define SQLITE_ENABLE_EXPLAIN_COMMENTS
#define SQLITE_DQS 0
#define SQLITE_ENABLE_DBPAGE_VTAB
#define SQLITE_ENABLE_STMTVTAB
#define SQLITE_ENABLE_MEMSYS5
#define SQLITE_OS_OTHER 1
#define SQLITE_OMIT_FLOATING_POINT 1

#define FILENAME_MAX 1024
#define SQLFS_FILENAME_MAX 255

#define SQLITE_KERNEL_BUILD
#define assert(X)

#endif //KERNEL_SQLITE_SQLITE_DEFS_H
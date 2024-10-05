#ifndef _LINUX_KSQLITE_H
#define _LINUX_KSQLITE_H

#include "sqlite3.h"
#include <linux/wait.h>
#include <linux/kernel.h>
//extern int ksqlite_open_db(char *, sqlite3 **);
//extern sqlite3_int64 make_data_id(sqlite3 *db, wait_queue_head_t* read_wait_queue, wait_queue_head_t* write_wait_queue );
extern void ksqlite_init(void);
extern int ksqlite_insert_into_pid(pid_t, pid_t);
extern int ksqlite_dup_fd(pid_t, pid_t);
extern int ksqlite_set_up_pipes(int,int);
extern int ksqlite_delete_pid(pid_t);
extern int ksqlite_delete_file(int);

extern int ksqlite_setup_for_write(int *, int);
void ksqlite_close_for_write(int, bool);
// extern struct mutex ksqlite_write_mutex;
// struct sql_compact {
//     struct sqlite3 *db;
//     unsigned int mode;
// };
#define IS_UNTOUCHED    1
#define IS_NEW          2
#define SQL_LOCKED_FOR_READ 4
#define SQL_LOCKED_FOR_WRITE 8
#define SQLFS_MAGIC 0xCAFEBABE
extern int bug_on_neq(int input);
extern void warn_with_message(sqlite3 *db);
// extern int ksqlite_run_with_message(sqlite3*db, char **zErrMesg, char *stmt);
extern int ksqlite_start_transaction(sqlite3* db, char **zErrMesg);
extern int ksqlite_commit_transaction(sqlite3* db, char **zErrMesg);
#endif
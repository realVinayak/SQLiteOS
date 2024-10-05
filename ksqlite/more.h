#include <linux/sqlite3.h>
#include <linux/wait.h>
extern sqlite3_int64 make_data_id(sqlite3 *db, wait_queue_head_t* read_wait_queue, wait_queue_head_t* write_wait_queue );
//extern void warn_with_message(sqlite3 *db);
extern int ksqlite_open_db(char *, sqlite3 **);
//extern int bug_on_neq(int input);
extern struct mutex ksqlite_write_mutex;
//extern int ksqlite_start_transaction(sqlite3* db, char **zErrMesg);

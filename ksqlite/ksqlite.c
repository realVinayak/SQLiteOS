#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/module.h>
#include "sqlite_defs.h"
#include <linux/mutex.h>
#include "sqlite3.h"
#include <linux/slab.h>
#include <linux/pid.h>
#include <linux/uaccess.h>
#include <linux/syscalls.h>
#include <linux/sched.h>
#include <linux/log2.h>

// Every write needs to acquire this mutex.
DEFINE_MUTEX(ksqlite_write_mutex);

DEFINE_MUTEX(kmem);

static int callback(void *data, int argc, char **argv, char **azColName){
    int i;
    printk("%s: ", (const char*)data);
    for(i=0; i<argc; i++){
        printk("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printk("\n");
    return 0;
}

int total_bytes;

sqlite3_int64 record_pipe;
static void * ksqlite_malloc(int size){
    //printk("calling with %d, %d, %d\n", size, total_bytes, total_bytes + size);
    total_bytes += size;
    return kmalloc(size, GFP_KERNEL);
}

static void ksqlite_free(void * object){
    //int size = (int)ksize(object);
    //printk("free calling with %d, %d, %d\n", size, total_bytes, total_bytes - size);
    //total_bytes -= size;
    kfree(object);
    return;
}

static void * ksqlite_realloc(void *object, int size){
    //int size2 = ksize(object);
    //printk("relalcfree calling with %d, %d\n", size, (int)ksize(object));
    //total_bytes = total_bytes + size - (int)ksize(object);
    return krealloc(object, size, GFP_KERNEL);
}

static int ksqlite_mem_init(void * none){
    return SQLITE_OK;
}

static void ksqlite_mem_shutdown(void * none){
    return;
}

static int ksqlite_roundup(int size){
    return (int)roundup_pow_of_two(size);
}

static int ksqlite_size(void *object){
    int size_to_return = ksize(object);
    //printk("Returning size: %d", size_to_return);
    return size_to_return;
}
void __init ksqlite_init(void) {
    total_bytes = 0;
    static sqlite3 *sqlite_main_db;

    static const sqlite3_mem_methods ksqlite_mem_methods = {
        ksqlite_malloc,
        ksqlite_free,
        ksqlite_realloc,
        ksqlite_size,
        ksqlite_roundup,
        ksqlite_mem_init,
        ksqlite_mem_shutdown,
        NULL
    };

    int config_result = sqlite3_config(
        SQLITE_CONFIG_MALLOC, 
        &ksqlite_mem_methods
    );

    if (config_result){
        printk("Error configuring mem methods: %d\n", config_result);
        return;
    }
    char *zErrMesg = 0;
    int rc;
    mutex_lock(&ksqlite_write_mutex);
    // Open an in-memory database
    rc = sqlite3_open("/test", &sqlite_main_db);

    if( rc != SQLITE_OK ) {
        printk("Can't open database: %s\n", sqlite3_errmsg(sqlite_main_db));
    } else {
        printk("Opened database successfully\n");
    }
    char *sql;

    // SQL statement to create a table
    sql = "CREATE TABLE pid_store(pid INT PRIMARY KEY   NOT NULL, parent_id INT NOT NULL);";

    // Execute SQL statement to create table
    rc = sqlite3_exec(sqlite_main_db, sql, 0, 0, &zErrMesg);


    if( rc != SQLITE_OK ){
        printk("SQL error: %s\n", zErrMesg);
        sqlite3_free(zErrMesg);
    } else {
        printk("pid_store Table created successfully\n");
    }


   sql = "CREATE TABLE pid_pipe" \
            "(pid       INT NOT NULL," \
             "fd        INT NOT NULL," \
             "dataId    INT NOT NULL" \
             ");";

    // Execute SQL statement to create table
    rc = sqlite3_exec(sqlite_main_db, sql, 0, 0, &zErrMesg);

    if( rc != SQLITE_OK ){
        printk("SQL error: %s\n", zErrMesg);
        sqlite3_free(zErrMesg);
    } else {
        printk("pid_pipe Table created successfully\n");
    }

    sql = "CREATE TABLE pipe_data" \
          "(dataId   INTEGER PRIMARY KEY NOT NULL," \
          "read_wq   BLOB NOT NULL,"\
          "write_wq  BLOB NOT NULL,"
          "data      BLOB DEFAULT NULL);";

    rc = sqlite3_exec(sqlite_main_db, sql, NULL, 0, &zErrMesg);

    if( rc != SQLITE_OK ){
        printk("SQL error: %s\n", zErrMesg);
        sqlite3_free(zErrMesg);
    } else {
        printk("pipe_data Table created successfully\n");
    }

    rc = sqlite3_close(sqlite_main_db);
    printk("Closed initialization with %d\n", rc);
    mutex_unlock(&ksqlite_write_mutex);
}

int ksqlite_open_db(char *db_name, sqlite3 **db){
    int rc = sqlite3_open(db_name, db);
    if( rc != SQLITE_OK ) {
        printk("Can't open database: %s\n", sqlite3_errmsg(*db));
    } else {
        //printk("Current memory usage: %d\n", total_bytes);
        //printk("Opened database successfully. total size: %d. Pipes count: %d\n", total_bytes, record_pipe);
    }
    return rc;
}

int ksqlite_start_transaction(sqlite3* db, char **zErrMesg){
    int rc;
    rc = sqlite3_exec(db, "BEGIN TRANSACTION;", 0, 0, zErrMesg);
    if (rc != SQLITE_OK){
        printk("Couldn't start transaction; %s\n", *zErrMesg); 
    }
    return rc;
}


int ksqlite_set_up_pipes(
    int read_end,
    int write_end
){
    sqlite3 *db;
    int rc;
    mutex_lock(&ksqlite_write_mutex);
    record_pipe++;
    rc = ksqlite_open_db("/test", &db);
    if (rc != SQLITE_OK) goto sql_exit;

    wait_queue_head_t *read_wait_queue;
    read_wait_queue = (wait_queue_head_t*)kmalloc(sizeof(wait_queue_head_t), GFP_KERNEL);
    if (!read_wait_queue){
        printk("Error allocating memory for read wait queue\n");
        goto sql_exit;
    }
    wait_queue_head_t *write_wait_queue;
    write_wait_queue = (wait_queue_head_t*)kmalloc(sizeof(wait_queue_head_t), GFP_KERNEL);
    if (!write_wait_queue){
        kfree(read_wait_queue);
        printk("Error allocating memory for write wait queue\n");
        goto sql_exit;
    }
    init_waitqueue_head(read_wait_queue);
    init_waitqueue_head(write_wait_queue);
    sqlite3_stmt *stmt;
    char *zErrMesg;

    rc = sqlite3_exec(db, "BEGIN TRANSACTION;", 0, 0, &zErrMesg);
    
    if (rc != SQLITE_OK){
        printk("Couldn't start transaction; %s\n", zErrMesg);
        goto sql_exit;
    }

    rc = sqlite3_prepare(
        db,
        "INSERT INTO pipe_data (read_wq, write_wq) values (?, ?);",
        -1,
        &stmt,
        NULL
    );
    
    if (rc != SQLITE_OK){
        printk("Error preparating for pid data insert %d", rc);
        goto sql_exit;
    }

    if ((rc = sqlite3_bind_int64(stmt, 1, read_wait_queue)) != SQLITE_OK){
        printk("Couldn't bind param %d\n", rc);
        goto sql_stmt_exit;
    }

    if ((rc = sqlite3_bind_int64(stmt, 2, write_wait_queue)) != SQLITE_OK){
        printk("Couldn't bind param %d\n", rc);
        goto sql_stmt_exit;
    }

    if ((rc = sqlite3_step(stmt)) != SQLITE_DONE ){
        printk("Error executing stmt: %d\n", rc);
        goto sql_stmt_exit;
    }

    sqlite3_finalize(stmt);

    sqlite3_int64 last_row_id = sqlite3_last_insert_rowid(db);

    rc = sqlite3_prepare(
        db,
        "INSERT INTO pid_pipe (pid, fd, dataId) values (?, ?, ?), (?, ?, ?);",
        -1, 
        &stmt,
        NULL
    );

    if (rc != SQLITE_OK){
        printk("Couldn't prepare pid insert %d\n", rc);
        goto sql_exit;
    }

    pid_t insert_pid = current->pid;
    if ((rc = sqlite3_bind_int(stmt, 1, insert_pid)) != SQLITE_OK){
        printk("Couldn't bind param %d\n", rc);
        goto sql_stmt_exit;
    }
    if ((rc = sqlite3_bind_int(stmt, 2, read_end)) != SQLITE_OK){
        printk("Couldn't bind param %d\n", rc);
        goto sql_stmt_exit;
    }
    if ((rc = sqlite3_bind_int(stmt, 3, (int)last_row_id)) != SQLITE_OK){
        printk("Couldn't bind param %d\n", rc);
        goto sql_stmt_exit;
    }

    if ((rc = sqlite3_bind_int(stmt, 4, insert_pid)) != SQLITE_OK){
        printk("Couldn't bind param %d\n", rc);
        goto sql_stmt_exit;
    }
    if ((rc = sqlite3_bind_int(stmt, 5, write_end)) != SQLITE_OK){
        printk("Couldn't bind param %d\n", rc);
        goto sql_stmt_exit;
    }
    if ((rc = sqlite3_bind_int(stmt, 6, (int)last_row_id)) != SQLITE_OK){
        printk("Couldn't bind param %d\n", rc);
        goto sql_stmt_exit;
    }
    if ((rc = sqlite3_step(stmt)) != SQLITE_DONE ){
        printk("Error executing stmt: %d\n", rc);
        goto sql_stmt_exit;
    }

    rc = sqlite3_exec(db, "COMMIT;", 0, 0, &zErrMesg);

    if (rc != SQLITE_OK){
        printk("Couldn't commit transaction; %s\n", zErrMesg);
        printk("memory used: %d\n", total_bytes);
        goto sql_stmt_exit;
    }

sql_stmt_exit:
    sqlite3_finalize(stmt);

sql_exit:
    int test;
    test = sqlite3_close(db);
   // printk("return code for pipes: %d\n", test);
    mutex_unlock(&ksqlite_write_mutex);
    //BUG_ON(test != 0);
    return rc;
}

int ksqlite_insert_into_pid(pid_t insert_pid, pid_t lookup_pid){
    sqlite3 *db;
    int rc;

    mutex_lock(&ksqlite_write_mutex);
    rc = ksqlite_open_db("/test", &db);
    if (rc != SQLITE_OK) goto pid_sql_exit;
    sqlite3_stmt *stmt;
    char *zErrMesg;
    int fdlookup;
    rc = ksqlite_start_transaction(db, &zErrMesg);
    if (rc != SQLITE_OK) goto pid_sql_exit;
    if ((insert_pid != lookup_pid) && (lookup_pid != 0)){
        if ((rc = sqlite3_prepare(
            db,
            "select parent_id from pid_store where pid = ?",
            -1,
            &stmt,
            NULL
            )) != SQLITE_OK){
                printk("Couldn't prepare stmt%d\n", rc);
                goto pid_sql_exit;
            }
        if (sqlite3_bind_int(stmt, 1, lookup_pid) != SQLITE_OK){
            printk("Couldn't prepare stmt %d\n", rc);
            goto pid_sql_exit;
        }
        if ((sqlite3_step(stmt)) != SQLITE_ROW){
            printk("Couldn't step %d\n");
            goto pid_sql_exit;
        }
        fdlookup = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
    }else{
        fdlookup = insert_pid;
    }
    if ((rc = sqlite3_prepare(
            db, 
            "INSERT INTO pid_store (pid, parent_id) values (?, ?)",
            -1,
            &stmt,
            NULL)) != SQLITE_OK){
                printk("Couldn't prepare stmt%d\n", rc);
                goto pid_sql_exit;
        }
    if (sqlite3_bind_int(stmt, 1, insert_pid) != SQLITE_OK){
        printk("Couldn't bind param%d\n", rc);
        goto pid_sql_exit;
    }
    if (sqlite3_bind_int(stmt, 2, fdlookup) != SQLITE_OK){
        printk("Couldn't bind param%d\n", rc);
        goto pid_sql_exit;
    }
    if ((rc = sqlite3_step(stmt)) != SQLITE_DONE){
        printk("Error executing stmt for inserting pid: %d\n", rc);
        zErrMesg = sqlite3_errmsg(db);
        printk("Error %s\n", zErrMesg);
        printk("memory used: %d\n", total_bytes);
    }
    sqlite3_finalize(stmt);
    //printk("Inserted: %d, %d", insert_pid, fdlookup);
    rc = sqlite3_exec(db, "COMMIT;", 0, 0, &zErrMesg);

    if (rc != SQLITE_OK){
        printk("Couldn't commit transaction; %s\n", zErrMesg);
    }

pid_sql_exit:
    int test;
    test = sqlite3_close(db);
   // printk("return code for pid: %d\n", test);
    mutex_unlock(&ksqlite_write_mutex);
    //BUG_ON(test != 0);
    return rc;
}

int ksqlite_dup_fd(pid_t insert_pid, pid_t lookup_id){
    if (lookup_id == 0){
        // This is an edge case, don't handle first process
        return 0;
    }
    sqlite3 *db;
    sqlite3_stmt *stmt = NULL;
    int rc;
    mutex_lock(&ksqlite_write_mutex);
    rc = ksqlite_open_db("/test", &db);
    if (rc != SQLITE_OK) goto ksqlite_dup_fd_exit;
    rc = sqlite3_prepare(
        db,
        "INSERT INTO pid_pipe  (pid, fd, dataId) select ?, fd, dataId from pid_pipe where pid = ?",
        -1,
        &stmt,
        NULL
    );
    if (rc != SQLITE_OK){
        printk("Couldn't prepare dup pipe %d\n", rc);
        goto ksqlite_dup_fd_exit;
    }
    if ((rc = sqlite3_bind_int(stmt, 1, insert_pid)) != SQLITE_OK){
        printk("Couldn't bind param %d\n", rc);
        WARN_ON(1);
        goto ksqlite_dup_fd_exit;
    }
    if ((rc = sqlite3_bind_int(stmt, 2, lookup_id)) != SQLITE_OK){
        printk("Couldn't bind param %d\n", rc);
        WARN_ON(1);
        goto ksqlite_dup_fd_exit;
    }
    if ((rc = sqlite3_step(stmt)) != SQLITE_DONE){
        printk("Error executing stmt: %d\n", rc);
        WARN_ON(1);
        goto ksqlite_dup_fd_exit;
    }
ksqlite_dup_fd_exit:
    sqlite3_finalize(stmt);
    int test;
    test = sqlite3_close(db);
    mutex_unlock(&ksqlite_write_mutex);
    return rc;
}
int ksqlite_delete_pid(pid_t delete_pid){
    sqlite3 *db;
    int rc;
    return 0;
    mutex_lock(&ksqlite_write_mutex);
    rc = ksqlite_open_db("/test", &db);
    if (rc != SQLITE_OK) goto pid_del_sql_exit;

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare(
        db,
        "DELETE FROM pid_store WHERE pid = ?",
        -1,
        &stmt,
        NULL
    );
    if (rc != SQLITE_OK){
        printk("Couldn't prepare pid delete %d\n", rc);
        goto delete_sql_exit;
    }
    if ((rc = sqlite3_bind_int(stmt, 1, delete_pid)) != SQLITE_OK){
        printk("Couldn't delete pid delete %d\n", rc);
        goto delete_sql_exit;
    }
    if ((rc = sqlite3_step(stmt)) != SQLITE_DONE ){
        printk("Error executing stmt: %d\n", rc);
        goto delete_sql_exit;
    }
delete_sql_exit:
    sqlite3_finalize(stmt);

pid_del_sql_exit:
    int test;
    test = sqlite3_close(db);
    //printk("return code for pipes: %d\n", test);
    mutex_unlock(&ksqlite_write_mutex);
    //BUG_ON(test != 0);
    return rc;
}

int ksqlite_prepare(sqlite3* db, char *sql, sqlite3_stmt **stmt){
    int rc;
    rc = sqlite3_prepare(
        db,
        sql,
        -1,
        stmt,
        NULL
    );
    if (rc != SQLITE_OK){
        printk("Couldn't prepare statement: %d\n", rc);
    }
    return rc;
}
int ksqlite_delete_file(int fd){
    return 0;
    sqlite3 *db;
    pid_t delete_pid = (current->pid);
    int rc;
    mutex_lock(&ksqlite_write_mutex);
    rc = ksqlite_open_db("/test", &db);
    if ( rc != SQLITE_OK) goto delete_file_sql_exit;
    sqlite3_stmt *stmt;
    char *zErrMesg;
    rc = ksqlite_start_transaction(db, &zErrMesg);
    if (rc != SQLITE_OK) goto delete_file_sql_exit;

    rc = sqlite3_prepare(
        db,
        "DELETE FROM pipe_data where dataid in (select dataid from pid_pipe where pid = ? and fd = ?);",
        -1,
        &stmt,
        NULL
    );
    if (rc != SQLITE_OK){
        printk("Couldn't prepare delete statement %d\n", rc);
    }
    if ((rc = sqlite3_bind_int(stmt, 1, delete_pid)) != SQLITE_OK){
        printk("Couldn't bind param %d\n", rc);
        goto delete_file_finalize;
    }
    if ((rc = sqlite3_bind_int(stmt, 2, fd)) != SQLITE_OK){
        printk("Couldn't bind param %d\n", rc);
        goto delete_file_finalize;
    }

    if ((rc = sqlite3_step(stmt)) != SQLITE_DONE ){
        printk("Error executing stmt: %d\n", rc);
        goto delete_file_finalize;
    }

    if ((rc = sqlite3_finalize(stmt)) != SQLITE_OK){
        printk("Error finalizing stmt mid transaction!: %d\n", rc);
        // we already tried finalizing
        goto delete_file_sql_exit;
    }

    rc = ksqlite_prepare(
        db,
        "DELETE FROM pid_pipe where pid = ? and fd = ?;",
        &stmt
        );
    
    if (rc != SQLITE_OK) goto delete_file_sql_exit;

    if ((rc = sqlite3_bind_int(stmt, 1, delete_pid)) != SQLITE_OK){
        printk("Couldn't bind param %d\n", rc);
        goto delete_file_finalize;
    }
    if ((rc = sqlite3_bind_int(stmt, 2, fd)) != SQLITE_OK){
        printk("Couldn't bind param %d\n", rc);
        goto delete_file_finalize;
    }

    if ((rc = sqlite3_step(stmt)) != SQLITE_DONE ){
        printk("Error executing stmt: %d\n", rc);
        goto delete_file_finalize;
    }

    rc = sqlite3_exec(db, "COMMIT;", 0, 0, &zErrMesg);

    if (rc != SQLITE_OK){
        printk("Couldn't commit transaction; %s\n", zErrMesg);
        printk("memory used: %d\n", total_bytes);
        goto delete_file_finalize;
    }


delete_file_finalize:
    sqlite3_finalize(stmt);

delete_file_sql_exit:
    int test;
    test = sqlite3_close(db);
    mutex_unlock(&ksqlite_write_mutex);
    //printk("Successfully deleted file: %d of %d\n", fd, delete_pid);
    return rc;
}

SYSCALL_DEFINE2(ksqlite_query, char __user *, buf, unsigned long, size){
    char *temp_buff;
    temp_buff = kmalloc(sizeof(char)*size, GFP_KERNEL);
    if (copy_from_user(temp_buff, buf, size)){
        printk("Error copying data\n");
    }
    int rc;
    unsigned long flags;
    char *zErrMesg;
    sqlite3 *db;
    rc = ksqlite_open_db("/test", &db);
    if (rc != SQLITE_OK) return rc;
    // This should be read-only queries always so no mutex
    if ((rc = sqlite3_exec(db, temp_buff, callback, (void *) "Query from user space", &zErrMesg))){
            printk("SQL Error: %s\n", zErrMesg);
            sqlite3_free(zErrMesg);
        }
    kfree(temp_buff);
    return 0;
}

static int __do_sql_pipe_flags(
    int *fd,
    int flags 
){
    int fdr, fdw, error;
    error = get_unused_fd_flags(flags);
    if (error < 0)
        return error;
    fdr = error;

    error = get_unused_fd_flags(flags);
    if (error < 0)
        return error;
    fdw = error;
    fd[0] = fdr;
    fd[1] = fdw;
    return 0;
}

SYSCALL_DEFINE1(sql_pipe, int __user *, fildes)
{
    int fd[2];
    int error = 0;
    error = __do_sql_pipe_flags(fd, 0);
    if (!error){
        if(copy_to_user(fildes, fd, sizeof(fd))){
            put_unused_fd(fd[0]);
            put_unused_fd(fd[1]);
            error = -EFAULT;
        }
        error = ksqlite_set_up_pipes(fd[0], fd[1]);
    }
    return error;
}

SYSCALL_DEFINE3(sql_pipe_read, int, fd, char __user *, out_buff, size_t, size_to_read){
    int rc = 0;
    int bytes_read = 0;
    sqlite3 *db;
    char *zErrMesg;
    sqlite3_stmt *stmt;
    if (size_to_read == 0) return 0;
    mutex_lock(&ksqlite_write_mutex);
    bool wake_next_reader = false;
    wait_queue_head_t *read_wq = NULL;
    int total_chars;
    while (1){
        rc = ksqlite_open_db("/test", &db);
        if (rc != SQLITE_OK){
            WARN_ON(1);
            printk("%s", sqlite3_errmsg(db));
            goto sqlite_error_pipe_read; // Nothing to do!
        }
        if ((rc = ksqlite_start_transaction(db, &zErrMesg)) != SQLITE_OK){
            WARN_ON(1);
            printk("%s", sqlite3_errmsg(db));
            goto sqlite_error_pipe_read;
        }
        rc = sqlite3_prepare(
            db,
            "SELECT read_wq, write_wq, substr(data, 1, ?), length(data), pipe_data.dataId from pid_pipe join pipe_data on pipe_data.dataId = pid_pipe.dataId join pid_store on pid_store.parent_id = pid_pipe.pid where fd = ? and pid_store.pid = ?;",
            -1,
            &stmt,
            NULL
        );
        if (rc != SQLITE_OK){
            printk("Couldn't prepare for read;");
            WARN_ON(1);
            printk("%s", sqlite3_errmsg(db));
            goto sqlite_error_pipe_read;
        }
        if ((rc = sqlite3_bind_int(stmt, 1, size_to_read)) != SQLITE_OK){
            printk("Couldn't bind param %d\n", rc);
            WARN_ON(1);
            printk("%s", sqlite3_errmsg(db));
            goto sqlite_error_pipe_read;
        }
        if ((rc = sqlite3_bind_int(stmt, 2, fd)) != SQLITE_OK){
            printk("Couldn't bind param %d\n", rc);
            WARN_ON(1);
            printk("%s", sqlite3_errmsg(db));
            goto sqlite_error_pipe_read;
        }
        pid_t select_pid = (current->pid);
        if ((rc = sqlite3_bind_int(stmt, 3, select_pid)) != SQLITE_OK){
            printk("Couldn't bind param %d\n", rc);
            WARN_ON(1);
            printk("%s", sqlite3_errmsg(db));
            goto sqlite_error_pipe_read;
        }
        if ((rc = sqlite3_step(stmt)) != SQLITE_ROW){
            // TODO: Check if this happended because row was removed
            printk("Error executing stm: %d\n", rc);
            WARN_ON(1);
            printk("%s", sqlite3_errmsg(db));
            goto sqlite_error_pipe_read;
        }
        total_chars = sqlite3_column_int(stmt, 3);
        // total chars can be null initially
        total_chars = total_chars ? total_chars : 0;
        // possibly cache this result?
        read_wq = (wait_queue_head_t *)sqlite3_column_int64(stmt, 0);
        if (total_chars == 0){
            // TODO: Handle NOHANG case
            // we wanted to read something, but couldn't buffer was empty.
            // block in that case. This is where magic happens. We obviously don't
            // have anything to commit at this point, so we can safely close the connection
            DEFINE_WAIT(rdwait);
            prepare_to_wait(read_wq, &rdwait, TASK_INTERRUPTIBLE);
            sqlite3_finalize(stmt);
            int test = sqlite3_close(db);
            WARN_ON(test != SQLITE_OK); // This really should be true
            mutex_unlock(&ksqlite_write_mutex);
            schedule();
            finish_wait(read_wq, &rdwait);
            mutex_lock(&ksqlite_write_mutex);
            if (signal_pending(current)){
                rc = ERESTARTSYS;
                goto sqlite_error_pipe_read;
            }
            continue;
        }
        break;
    }
    int chars_left = total_chars - size_to_read;
    // Only once process gets woken up, so we might need to wake up 
    // other readers
    if (chars_left > 0){
        wake_next_reader = true;
    }
    int to_update = sqlite3_column_int(stmt, 4);
    const char *pipe_read_data = sqlite3_column_blob(stmt, 2);
    bytes_read = size_to_read > total_chars ? total_chars : size_to_read;
    const char *read_data = kmalloc(sizeof(char)*bytes_read, GFP_KERNEL);
    memcpy(read_data, pipe_read_data, bytes_read);
    printk("bytes read: %s, bytes_read: %d", read_data, bytes_read);
    sqlite3_finalize(stmt);
    rc = sqlite3_prepare(
        db,
        "UPDATE pipe_data SET data = SUBSTR(data, ?) where dataId = ?;",
        -1,
        &stmt,
        NULL
    );
    if (rc != SQLITE_OK){
        printk("Couldn't prepare for update;");
        WARN_ON(1);
        goto sqlite_error_pipe_read;
    }
    if ((rc = sqlite3_bind_int(stmt, 1, bytes_read+1)) != SQLITE_OK){
        printk("Couldn't bind param %d\n", rc);
        WARN_ON(1);
        goto sqlite_error_pipe_read;
    }
    if ((rc = sqlite3_bind_int(stmt, 2, to_update)) != SQLITE_OK){
        printk("Couldn't bind param %d\n", rc);
        WARN_ON(1);
        goto sqlite_error_pipe_read;
    }
    if ((rc = sqlite3_step(stmt)) != SQLITE_DONE){
        printk("Error executing stmt: %d\n", rc);
        WARN_ON(1);
        goto sqlite_error_pipe_read;
    }
    
    // Aborts the entire transaction still!
    // I think it is atomic, so this should work?
    if (copy_to_user(out_buff, read_data, bytes_read)){
        rc = EFAULT;
        WARN_ON(1);
        goto sqlite_error_pipe_read;
    }
    rc = sqlite3_exec(db, "COMMIT;", 0, 0, &zErrMesg);
    if (rc != SQLITE_OK){
        WARN_ON(1);
        printk("Couldn't commit transaction; %s\n", zErrMesg);
        goto sqlite_error_pipe_read;
    }
    // All good case. We committed our transaction, good to go!
    // We try waking up another process depending on the length from before
    rc = bytes_read;
    kfree(read_data);
    goto sqlite_wakeup_prepare_next;


sqlite_error_pipe_read:
    sqlite3_finalize(stmt);
    rc = -rc; // flip sqlite errors for passing up
    wake_next_reader = true; // if we had an error, trying waking up another to read

sqlite_wakeup_prepare_next:
    printk("wake next reader: %d, read queue: %lld\n", wake_next_reader, read_wq);
    if (wake_next_reader && read_wq) wake_up_interruptible_sync(read_wq);
    mutex_unlock(&ksqlite_write_mutex);
   
sql_pipe_read_exit:
    int test;
    test = sqlite3_close(db);
    return rc;
}

SYSCALL_DEFINE3(sql_pipe_write, int, fd, char __user *, in_buff, size_t, size_to_write){
    int rc;
    sqlite3 *db;
    char *zErrMesg;
    sqlite3_stmt *stmt;
    char *temp_buff;
    wait_queue_head_t *read_wq = NULL;
    if (size_to_write == 0) return 0;
    temp_buff = kmalloc(sizeof(char)*size_to_write, GFP_KERNEL);
    if (rc = copy_from_user(temp_buff, in_buff, size_to_write)){
        return rc;
    }
    mutex_lock(&ksqlite_write_mutex);
    rc = ksqlite_open_db("/test", &db);
    if (rc != SQLITE_OK){
        goto sqlite3_error_pipe_write;
    }
    if ((rc = ksqlite_start_transaction(db, &zErrMesg)) != SQLITE_OK){
        goto sqlite3_error_pipe_write;
    }
    rc = sqlite3_prepare(
        db,
        "SELECT data, length(data), pipe_data.dataId, read_wq from pid_pipe join pipe_data on pipe_data.dataId = pid_pipe.dataId join pid_store on pid_store.parent_id = pid_pipe.pid where fd = ? and pid_store.pid = ?;",
        -1,
        &stmt,
        NULL
    );
    if (rc != SQLITE_OK){
        WARN_ON(1);
        printk("%s", sqlite3_errmsg(db));
        printk("Couldn't prepare for write;");
        goto sqlite3_error_pipe_write;
    }
    if ((rc = sqlite3_bind_int(stmt, 1, fd)) != SQLITE_OK){
        WARN_ON(1);
        printk("%s", sqlite3_errmsg(db));
        printk("Couldn't bind param %d\n", rc);
        goto sqlite3_error_pipe_write;
    }
    pid_t select_pid = (current->pid);
    if ((rc = sqlite3_bind_int(stmt, 2, select_pid))){
        WARN_ON(1);
        printk("%s", sqlite3_errmsg(db));
        printk("Couldn't bind param %d\n", rc);
        goto sqlite3_error_pipe_write;
    }
    if ((rc = sqlite3_step(stmt)) != SQLITE_ROW){
        WARN_ON(1);
        printk("%s", sqlite3_errmsg(db));
        printk("Error executing stmt: %d\n", rc);
        goto sqlite3_error_pipe_write;
    }
    int total_chars = sqlite3_column_int(stmt, 1);
    char * current_blob = sqlite3_column_blob(stmt, 0);
    total_chars = total_chars ? total_chars : 0;
    char *main_buff = kmalloc(sizeof(char)*(total_chars + size_to_write), GFP_KERNEL);
    if (total_chars > 0){
        memcpy(main_buff, current_blob, total_chars);
    }
    memcpy(main_buff + total_chars, temp_buff, size_to_write);
    int new_total_size = size_to_write + total_chars;
    kfree(temp_buff); // don't need this buff anymore!
    int to_update = sqlite3_column_int(stmt, 2);
    read_wq = (wait_queue_head_t*)sqlite3_column_int64(stmt, 3);
    sqlite3_finalize(stmt);
    rc = sqlite3_prepare(
        db,
        "UPDATE pipe_data SET data=? where dataId = ?;",
        -1,
        &stmt,
        NULL
    );
    if (rc != SQLITE_OK){
        WARN_ON(1);
        printk("%s", sqlite3_errmsg(db));
        printk("Couldn't prepare for update");
        goto sqlite3_error_pipe_write;
    }
    if ((rc = sqlite3_bind_blob(stmt, 1, main_buff, new_total_size, SQLITE_TRANSIENT)) != SQLITE_OK){
        WARN_ON(1);
        printk("%s", sqlite3_errmsg(db));
        printk("Couldn't bind param %d\n", rc);
        goto sqlite3_error_pipe_write;
    }
    if ((rc = sqlite3_bind_int(stmt, 2, to_update)) != SQLITE_OK){
        WARN_ON(1);
        printk("%s", sqlite3_errmsg(db));
        printk("Couldn't bind param %d\n", rc);
        goto sqlite3_error_pipe_write;
    }
    if ((rc = sqlite3_step(stmt)) != SQLITE_DONE){
        WARN_ON(1);
        printk("%s", sqlite3_errmsg(db));
        printk("Error performing step %d\n", rc);
        goto sqlite3_error_pipe_write;
    }
    // we just need to commit now.
    rc = sqlite3_exec(db, "COMMIT;", 0, 0, &zErrMesg);
    if (rc != SQLITE_OK){
        WARN_ON(1);
        printk("%s", sqlite3_errmsg(db));
        printk("Couldn't commit transaction; %s\n", zErrMesg);
        goto sqlite3_error_pipe_write;
    }
    // Wake up a reader, any one.
    if (read_wq)wake_up_interruptible_sync(read_wq);
    kfree(main_buff);

sqlite3_error_pipe_write:
    sqlite3_finalize(stmt);
    rc = -rc;
    mutex_unlock(&ksqlite_write_mutex);
    int test;
    test = sqlite3_close(db);
    return rc;
}

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/spinlock.h>
#include "sqlite_defs.h"
#include "sqlite3.h"
#include <linux/slab.h>
#include <linux/pid.h>
#include <linux/uaccess.h>
#include <linux/syscalls.h>
#include <linux/sched.h>

DEFINE_SPINLOCK(ksqlite_main_lock); // TODO: Get rid of this

void setup_sqlite3(void);
void add_vfs(void);

#define ALLOC_SIZE 1048576
#define MIN_ALLOC_SIZE 128

static sqlite3 *sqlite_main_db;

static int callback(void *data, int argc, char **argv, char **azColName){
    int i;
    printk("%s: ", (const char*)data);
    for(i=0; i<argc; i++){
        printk("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printk("\n");
    return 0;
}


static void * ksqlite_malloc(int size){
    return kmalloc(size, GFP_KERNEL);
}

static void ksqlite_free(void * object){
    kfree(object);
    return;
}

static void * ksqlite_realloc(void *object, int size){
    return krealloc(object, size, GFP_KERNEL);
}

static int ksqlite_mem_init(void * none){
    return SQLITE_OK;
}

static void ksqlite_mem_shutdown(void * none){
    return;
}

static int ksqlite_roundup(int size){
    return size;
}

static int ksqlite_size(void *object){
    return (int)ksize(object);
}
void __init ksqlite_init(void) {
    
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
    char *zErrMsg = 0;
    int rc;

    // Open an in-memory database
    rc = sqlite3_open("/test", &sqlite_main_db);

    if( rc != SQLITE_OK ) {
        printk("Can't open database: %s\n", sqlite3_errmsg(sqlite_main_db));
    } else {
        printk("Opened database successfully\n");
    }

    // SQL statement to create a table
    char *sql_create_table = "CREATE TABLE pid_store(pid INTEGER);";

    // Execute SQL statement to create table
    rc = sqlite3_exec(sqlite_main_db, sql_create_table, 0, 0, &zErrMsg);

    if( rc != SQLITE_OK ){
        printk("SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        printk("PID Table created successfully\n");
    }

    sql_create_table = "CREATE TABLE pid_pipe_map(pid INTEGER, pipe_read_fd INTEGER, pipe_write_fd INTEGER, data BLOB);";

    // Execute SQL statement to create table
    rc = sqlite3_exec(sqlite_main_db, sql_create_table, 0, 0, &zErrMsg);

    if( rc != SQLITE_OK ){
        printk("SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        printk("PID PIPE MAP Table created successfully\n");
    }
}


void ksqlite_insert_into_pid(pid_t insert_pid){
    unsigned long flags;
    spin_lock_irqsave(&ksqlite_main_lock, flags);
    sqlite3_stmt *stmt;
    char *zErrMsg;
    int resp;
    if ((resp = sqlite3_prepare(
        sqlite_main_db, 
        "INSERT INTO pid_store (pid) values (?)",
        -1,
        &stmt,
        NULL)) != SQLITE_OK){
            printk("Couldn't prepare stmt%d\n", resp);
            goto sql_exit;
    }
    if (sqlite3_bind_int(stmt, 1, insert_pid) != SQLITE_OK){
        printk("Couldn't bind param%d\n", resp);
        goto sql_exit;
    }
    if ((resp = sqlite3_step(stmt)) != SQLITE_DONE){
        printk("Error executing stmt: %d\n", resp);
        goto sql_exit;
    }
    sqlite3_clear_bindings(stmt);

sql_exit:
    sqlite3_reset(stmt);
    spin_unlock_irqrestore(&ksqlite_main_lock, flags);
    return;
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

}

void ksqlite_insert_pid_pipe(
    int read_end,
    int write_end
){
    unsigned long flags;
    spin_lock_irqsave(&ksqlite_main_lock, flags);
    int resp;
    sqlite3_stmt *stmt;
    if ((resp = sqlite3_prepare(
        sqlite_main_db,
        "INSERT INTO pid_pipe_map (pid, pipe_read_fd, pipe_write_fd) values (?, ?, ?)",
        -1,
         &stmt,
         NULL
    )) != SQLITE_OK){
        printk("Couldn't prepare stmt %d\n", resp);
        goto sqlite_pid_pipe_exit;
    }

    pid_t insert_pid = task_pid_nr(current);

    
    if ((resp = sqlite3_bind_int(stmt, 1, insert_pid)) != SQLITE_OK){
        printk("Couldn't bind param%d\n", resp);
        goto sqlite_pid_pipe_exit;
    }
    
    if ((resp = sqlite3_bind_int(stmt, 2, read_end)) != SQLITE_OK){
        printk("Couldn't bind param%d\n", resp);
        goto sqlite_pid_pipe_exit;
    }

    if ((resp = sqlite3_bind_int(stmt, 3, write_end)) != SQLITE_OK){
        printk("Couldn't bind param%d\n", resp);
        goto sqlite_pid_pipe_exit;
    }
    if ((resp = sqlite3_step(stmt)) != SQLITE_DONE){
        printk("Error executing stmt: %d\n", resp);
        goto sqlite_pid_pipe_exit;
    }
    sqlite3_clear_bindings(stmt);
    
sqlite_pid_pipe_exit:
    sqlite3_reset(stmt);
    spin_unlock_irqrestore(&ksqlite_main_lock, flags);
}



SYSCALL_DEFINE2(ksqlite_query, char __user *, buf, unsigned long, size){
    char *temp_buff;
    temp_buff = kmalloc(sizeof(char)*size, GFP_KERNEL);
    if (copy_from_user(temp_buff, buf, size)){
        printk("Error copying data\n");
    }
    int rc;
    unsigned long flags;
    char *zErrMsg;
    spin_lock_irqsave(&ksqlite_main_lock, flags);
    if ((
        rc = sqlite3_exec(sqlite_main_db, temp_buff, callback, (void *) "Query from user space", &zErrMsg))){
            printk("SQL Error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        }
    spin_unlock_irqrestore(&ksqlite_main_lock, flags);
    return 0;
}
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/module.h>
#include "sqlite_defs.h"
#include "sqlite3.h"
#include <linux/slab.h>
void setup_sqlite3(void);
void add_vfs(void);
#define ALLOC_SIZE 1048576
#define MIN_ALLOC_SIZE 128

static int callback(void *data, int argc, char **argv, char **azColName){
    int i;
    printk("%s: ", (const char*)data);
    for(i=0; i<argc; i++){
        printk("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printk("\n");
    return 0;
}



static int __init ksqlite_init(void) {
void * pBuf = kmalloc(ALLOC_SIZE, GFP_ATOMIC);
    void * dataBuf = kmalloc(ALLOC_SIZE, GFP_ATOMIC);

    int config_result = sqlite3_config(SQLITE_CONFIG_HEAP, pBuf, ALLOC_SIZE, MIN_ALLOC_SIZE);



    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);

    if(rc) {
        printk("Can't open database: %s\n", sqlite3_errmsg(db));
        return(0);
    } else {
        printk("Opened database successfully\n");
    }

    // SQL statement to create a table
    const char *sql_create_table ="CREATE TABLE t0(x INTEGER PRIMARY KEY, y TEXT);";

    // Execute SQL statement to create table
    rc = sqlite3_exec(db, sql_create_table, 0, 0, &zErrMsg);

    if( rc != SQLITE_OK ){
        printk("SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        printk("Table created successfully\n");
    }

    // SQL statement to insert data
    const char *sql_insert_data =
            "INSERT INTO t0 VALUES (1, 'aaa'), (2, 'ccc'), (3, 'bbb');";

    // Execute SQL statement to insert data
    rc = sqlite3_exec(db, sql_insert_data, 0, 0, &zErrMsg);

    if( rc != SQLITE_OK ){
        printk("SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        printk("Records created successfully\n");
    }

    // SQL statement for select query
    const char *sql_select_query =
            "SELECT x, y, row_number() OVER (ORDER BY y) AS row_number FROM t0 ORDER BY x;";

    // Execute select query
    printk("Select Query Result:\n");
    rc = sqlite3_exec(db, sql_select_query, callback, (void*) "Select Query Result", &zErrMsg);

    if( rc != SQLITE_OK ){
        printk("SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }

    // Close database connection
    sqlite3_close(db);

    return 0;
}
static void __exit ksqlite_exit(void) {
}

module_init(ksqlite_init)
module_exit(ksqlite_exit)
MODULE_LICENSE("GPL and additional rights");

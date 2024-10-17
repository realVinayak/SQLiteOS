#include <linux/fs.h>
#include "sqlite_defs.h"
#include <linux/mutex.h>
#include <linux/ksqlite.h>
#include <linux/time.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/vmalloc.h>
#include "more.h"

#define KSQLITE_VFS_BLOB_SIZE 4096

static const struct inode_operations sqlfs_inode_ops;
struct file_operations sqlfs_file_ops;

// rollbacks ugh
struct inode_size {
    struct inode *inode;
    ssize_t size;
};

int commit_size_change(void *size_struct){
    struct inode_size * size_struct_nice = (struct inode_size *)size_struct;
    size_struct_nice->inode->i_size = size_struct_nice->size;
    return 0;
}

int insert_zero_blob(int order_no, int inode_no, sqlite3 *db);


void add_ops(struct inode *inode, int mode){
    if (S_ISDIR(mode)){
        inode->i_fop = &simple_dir_operations;
        inode->i_op = &sqlfs_inode_ops;
    }else{
        inode->i_fop =  &sqlfs_file_ops;
    }
}

int prepare_regular_blob(int order_no, int inode_no, const char *sqlstr, sqlite3_stmt **stmt, sqlite3 *db){
    int rc = SQLITE_OK;
    if ((rc = sqlite3_prepare(
        db,
        sqlstr,
        -1,
        stmt,
        NULL))){
            printk("Error preparing stmt");
            warn_with_message(db);
            return rc;
        }
    if ((rc = sqlite3_bind_int(*stmt, bug_on_neq(sqlite3_bind_parameter_index(*stmt, ":inode_no")), inode_no))){
        printk("Error binding stmt");
        warn_with_message(db);
        return rc;
    }

    if ((rc = sqlite3_bind_int(*stmt, bug_on_neq(sqlite3_bind_parameter_index(*stmt, ":order_no")), order_no))){
        printk("Error binding stmt");
        warn_with_message(db);
        return rc;
    }
    return SQLITE_OK;
}


int insert_zero_blob(int order_no, int inode_no, sqlite3 *db){
    sqlite3_stmt *stmt = NULL;

    int rc = SQLITE_OK;

    const char *sql_str =  "INSERT INTO partitioned_data (inode_no, order_no, data) values (:inode_no, :order_no, :blob_to_insert)";

    if ((rc = prepare_regular_blob(order_no, inode_no, sql_str, &stmt, db)) != SQLITE_OK){
        goto revert;
    }

    if ((rc = sqlite3_bind_zeroblob(
        stmt,
        bug_on_neq(sqlite3_bind_parameter_index(stmt, ":blob_to_insert")),
        KSQLITE_VFS_BLOB_SIZE
        ))){
        printk("Error binding stmt");
        warn_with_message(db);
        goto revert;
    }

    if ((rc = sqlite3_step(stmt)) != SQLITE_DONE){
        printk("Error stepping");
        warn_with_message(db);
    }else{
        rc = SQLITE_OK;
    }

revert:
    sqlite3_finalize(stmt);
    return rc;
}


int insert_blob(
    int order_no,
    int inode_no,
    sqlite3 *db,
    const char *buf
    ){
    
    int rc;
    const char *sql_str =  "INSERT INTO partitioned_data (inode_no, order_no, data) values (:inode_no, :order_no, :blob_to_insert)";

    sqlite3_stmt *stmt = NULL;

    if ((rc = prepare_regular_blob(order_no, inode_no, sql_str, &stmt, db)) != SQLITE_OK){
        goto revert;
    }

    if ((rc = sqlite3_bind_blob(
        stmt,
        bug_on_neq(sqlite3_bind_parameter_index(stmt, ":blob_to_insert")),
        buf,
        KSQLITE_VFS_BLOB_SIZE,
        SQLITE_TRANSIENT
        ))){
        printk("Error binding stmt");
        warn_with_message(db);
        goto revert;
    }

    if ((rc = sqlite3_step(stmt)) != SQLITE_DONE){
        printk("Error stepping");
        warn_with_message(db);
    } else{
        rc = SQLITE_OK;
    }

revert:
    sqlite3_finalize(stmt);
    return rc;
}

int insert_user_blob(
    int order_no,
    int inode_no,
    sqlite3 *db,
    const char __user *buf
    ){
    char *temp_buff = kmalloc((sizeof(char))*KSQLITE_VFS_BLOB_SIZE, GFP_KERNEL);
    int rc = SQLITE_OK;
    if (copy_from_user(temp_buff, buf, KSQLITE_VFS_BLOB_SIZE)){
        kfree(temp_buff);
        return EFAULT;
    }
    rc =  insert_blob(order_no, inode_no, db, temp_buff);
    kfree(temp_buff);
    return rc;
}


int update_size(int size, int inode_no, sqlite3 *db){
    int rc = SQLITE_OK;

    const char *sql_str = "UPDATE inode set size = :mod_size where id =:inode_no";

    sqlite3_stmt *stmt = NULL;

    if ((rc = sqlite3_prepare(db, sql_str,-1,&stmt,NULL))){
        printk("error binding blob");
        warn_with_message(db);
        goto revert;
         }
    
    if ((rc = sqlite3_bind_int(stmt, bug_on_neq(sqlite3_bind_parameter_index(stmt, ":mod_size")), size))){
        printk("error binding blob");
        warn_with_message(db);
        goto revert;
    }
    
    if ((rc = sqlite3_bind_int(stmt, bug_on_neq(sqlite3_bind_parameter_index(stmt, ":inode_no")), inode_no))){
        printk("error binding blob");
        warn_with_message(db);
        goto revert;
    }

    if ((rc = sqlite3_step(stmt)) != SQLITE_DONE){
        printk("Error stepping");
        warn_with_message(db);
    }else{
        rc = SQLITE_OK;
    }

revert:
    sqlite3_finalize(stmt);
    return rc;

}

int update_blob(int order_no, int inode_no, sqlite3 *db, const char *buf){
    int rc = SQLITE_OK;
    //printk("updating blob %s at %d, %d", buf, inode_no, order_no);
    const char *sql_str = "UPDATE partitioned_data set data = :blob_to_insert where inode_no =:inode_no and order_no =:order_no";

    sqlite3_stmt *stmt = NULL;

    if ((rc = prepare_regular_blob(order_no, inode_no, sql_str, &stmt, db)) != SQLITE_OK){
        return rc;
    }

    if ((rc = sqlite3_bind_blob(
        stmt,
        bug_on_neq(sqlite3_bind_parameter_index(stmt, ":blob_to_insert")),
        buf,
        KSQLITE_VFS_BLOB_SIZE,
        SQLITE_TRANSIENT
        ))){
            printk("error binding blob");
            warn_with_message(db);
            goto revert;
        }
    
    if ((rc = sqlite3_step(stmt)) != SQLITE_DONE){
        printk("Error stepping");
        warn_with_message(db);
    }else{
        rc = SQLITE_OK;
    }

revert:
    sqlite3_finalize(stmt);
    return rc;
}

int update_user_blob(int order_no, int inode_no, sqlite3 *db, const char __user *buf){
    // char temp_buff[KSQLITE_VFS_BLOB_SIZE];
    char *temp_buff = kmalloc((sizeof(char))*KSQLITE_VFS_BLOB_SIZE, GFP_KERNEL);
    if (!temp_buff) return ENOMEM;
    int rc = SQLITE_OK;
    if (copy_from_user(temp_buff, buf, KSQLITE_VFS_BLOB_SIZE)){
        kfree(temp_buff);
        return EFAULT;
    }
    rc = update_blob(order_no, inode_no, db, temp_buff);
    kfree(temp_buff);
    return rc;
}

int get_blob_at_order(int order_no, int inode_no, sqlite3 *db, char *temp_buff){
    int rc = SQLITE_OK;
    sqlite3_stmt *stmt = NULL;
    const char *sqlstr = "SELECT data from partitioned_data where inode_no = :inode_no and order_no = :order_no";
    if ((rc = prepare_regular_blob(order_no, inode_no, sqlstr, &stmt, db))){
        goto sql_revert;
    }
    
    if ((rc = sqlite3_step(stmt)) != SQLITE_ROW) {
        warn_with_message(db);
        rc = EFAULT;
    }else{
        const char *read_blob = sqlite3_column_blob(stmt, 0);
        memcpy(temp_buff, read_blob, KSQLITE_VFS_BLOB_SIZE);
        rc = SQLITE_OK;
    }
    
sql_revert:
    sqlite3_finalize(stmt);
    return rc;
}

struct inode *sqlfs_make_inode(struct super_block *sb, int mode, const char *name, struct inode *dir, sqlite3 *db){
    struct inode *inode=NULL;
    int inode_no;
    int name_length;
    int rc;
    char *zErrMesg;
    sqlite3_stmt *stmt;
    sqlite3_int64 dataId;

    if (!(S_ISDIR(mode) || S_ISREG(mode))){
        pr_err("Unsupported filetype %d\n");
        return ERR_PTR(-EINVAL);
    }
    name_length = name == NULL ? 0 : strlen(name);

    if (name_length > SQLFS_FILENAME_MAX){
        pr_err("Got larger filesizename\n");
        return ERR_PTR(-ENAMETOOLONG);
    }

    inode = new_inode(sb);
    if (!inode){
        return ERR_PTR(-ENOMEM);
    }

    inode->i_mode = mode;
    i_uid_write(inode, 0);
    i_gid_write(inode, 0);
    inode->i_size = 0;
    inode->i_atime = inode->i_mtime = inode->i_ctime = current_time(inode);

    inode->i_op = &simple_dir_inode_operations;
    inode_init_owner(&init_user_ns, inode, dir, mode);

    if (S_ISDIR(mode)){         
        inode->i_size = 4096;
        set_nlink(inode, 2);
    }else {
        inode->i_size = 0;
        set_nlink(inode, 1);
    }

    add_ops(inode, mode);

    // if ((rc = ksqlite_open_db("/test", &db)) != SQLITE_OK) goto eit;

    // if ((rc = ksqlite_start_transaction(db, &zErrMesg)) != SQLITE_OK) goto conn_close_eit;

    if ((rc = sqlite3_prepare(
        db,
        "INSERT INTO inode (mode, uid, gid, size, ctime, atime, mtime, name, parentId, nlink) values (:mode, :uid, :gid, :size, :ctime, :atime, :mtime, :name, :parentId, :nlink)",
        -1,
        &stmt, 
        NULL
    ))){
        printk("Error preparing statement %d\n", rc);
        warn_with_message(db);
        goto conn_close_eit;
    }
    if ((rc = sqlite3_bind_int(stmt, bug_on_neq(sqlite3_bind_parameter_index(stmt, ":mode")), mode))){
        printk("Error binding stmt %d\n", rc);
        warn_with_message(db);
        goto conn_stmt_eit;
    }

    if ((rc = sqlite3_bind_int(stmt, bug_on_neq(sqlite3_bind_parameter_index(stmt, ":uid")), i_uid_read(inode)))){
        printk("Error binding stmt %d\n", rc);
        warn_with_message(db);
        goto conn_stmt_eit;
    }

    if ((rc = sqlite3_bind_int(stmt, bug_on_neq(sqlite3_bind_parameter_index(stmt, ":gid")), i_gid_read(inode)))){
        printk("Error binding stmt %d\n", rc);
        warn_with_message(db);
        goto conn_stmt_eit;
    }

    if ((rc = sqlite3_bind_int(stmt, bug_on_neq(sqlite3_bind_parameter_index(stmt, ":size")), inode->i_size))){
        printk("Error binding stmt %d\n", rc);
        warn_with_message(db);
        goto conn_stmt_eit;
    }

    if ((rc = sqlite3_bind_int(stmt, bug_on_neq(sqlite3_bind_parameter_index(stmt, ":ctime")), inode->i_ctime.tv_sec))){
        printk("Error binding stmt %d\n", rc);
        warn_with_message(db);
        goto conn_stmt_eit;
    }

    if ((rc = sqlite3_bind_int(stmt, bug_on_neq(sqlite3_bind_parameter_index(stmt, ":atime")), inode->i_atime.tv_sec))){
        printk("Error binding stmt %d\n", rc);
        warn_with_message(db);
        goto conn_stmt_eit;
    }

    if ((rc = sqlite3_bind_int(stmt, bug_on_neq(sqlite3_bind_parameter_index(stmt, ":mtime")), inode->i_mtime.tv_sec))){
        printk("Error binding stmt %d\n", rc);
        warn_with_message(db);
        goto conn_stmt_eit;
    }

    if ((rc = sqlite3_bind_text(stmt, bug_on_neq(sqlite3_bind_parameter_index(stmt, ":name")), name, -1, SQLITE_TRANSIENT))){
        printk("Error binding stmt %d\n", rc);
        warn_with_message(db);
        goto conn_stmt_eit;
    }  

    if (dir){
        inode_no = dir->i_ino;
    }else{
        inode_no = NULL;
    }

    // parent can be null... for, say the root.
    if ((rc = sqlite3_bind_int(stmt, bug_on_neq(sqlite3_bind_parameter_index(stmt, ":parentId")), inode_no))){
        printk("Error binding stmt %d\n", rc);
        warn_with_message(db);
        goto conn_stmt_eit;
    }
    
    if ((rc = sqlite3_bind_int(stmt, bug_on_neq(sqlite3_bind_parameter_index(stmt, ":nlink")), inode->i_nlink))){
        printk("Error binding stmt %d\n", rc);
        warn_with_message(db);
        goto conn_stmt_eit;
    }


    if ((rc = sqlite3_step(stmt)) != SQLITE_DONE){
        printk("Error executing stmt: %d\n", rc);
        warn_with_message(db);
        goto conn_stmt_eit;
    }

    // rc = sqlite3_exec(db, "COMMIT;", 0, 0, &zErrMesg);

    // if (rc != SQLITE_OK){
    //     printk("Couldn't commit transaction: %s\n", zErrMesg);
    //     goto conn_stmt_eit;
    // }
    sqlite3_int64 inode_id = sqlite3_last_insert_rowid(db);
    inode->i_ino = inode_id;

    rc = SQLITE_OK;

conn_stmt_eit:
    sqlite3_finalize(stmt);

conn_close_eit:
   // rc = sqlite3_close(db);

eit:
    if (rc != SQLITE_OK){
        iput(inode);
        return ERR_PTR(-rc);
    }
    return inode;
}

static int sqlfs_create(
    struct user_namespace *username,
    struct inode *dir,
    struct dentry *dentry,
    umode_t mode,
    bool excl 
){
    struct super_block *sb;
    struct inode *new_inode;
    int ret = 0;
    sb = dir->i_sb;
    sqlite3 *db;
    char *zErrMesg;
    sqlite3_stmt *stmt;
    struct timespec64 time;

    int previous_flags = 0;

    if ((ret = ksqlite_setup_for_write(&previous_flags, SQL_LOCKED_FOR_WRITE))){
        goto sql_revert;
    }
    db = (sqlite3*)(current->sql_ref->db);

    new_inode = sqlfs_make_inode(sb, mode, dentry->d_name.name, dir, db);

    if (IS_ERR(new_inode)){
        ret = EINVAL;
        goto sql_revert;
    }

    time = current_time(dir);
    if (S_ISDIR(mode)) inc_nlink(dir);

    if (dir->i_ino){
        if (ret = sqlite3_prepare(
            db, 
            "UPDATE inode set atime = :atime, mtime = :mtime, nlink = :nlink where id = :id",
            -1,
            &stmt,
            NULL
        )){
            printk("Error preparing statement %d\n", ret);
            warn_with_message(db);
            goto sql_revert;
        }

        if ((ret = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":atime"), time.tv_sec)) != SQLITE_OK){
            printk("Error binding stmt %d\n", ret);
            warn_with_message(db);
            goto sql_revert;
        }
        if ((ret = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":mtime"), time.tv_nsec)) != SQLITE_OK){
            printk("Error binding stmt %d\n", ret);
            warn_with_message(db);
            goto sql_revert;
        }
        if ((ret = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":nlink"), dir->i_nlink)) != SQLITE_OK){
            printk("Error binding stmt %d\n", ret);
            warn_with_message(db);
            goto sql_revert;
        }
        if ((ret = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":id"), dir->i_ino)) != SQLITE_OK){
            printk("Error binding stmt %d\n", ret);
            warn_with_message(db);
            goto sql_revert;
        }
        if ((ret = sqlite3_step(stmt)) != SQLITE_DONE){
            printk("Error stepping %d\n", ret);
            warn_with_message(db);
            goto sql_revert;
        }
        ret = SQLITE_OK;
    }

    BUG_ON(new_inode == NULL);
    d_add(dentry, new_inode);

sql_revert:
    sqlite3_finalize(stmt);
    ksqlite_close_for_write(previous_flags, ret == SQLITE_OK);

    if (ret != SQLITE_OK){
        if (new_inode != NULL){
            iput(new_inode);
        }
        return ERR_PTR(-ret);
    }
    return 0;
}

static int sqlfs_mkdir(
    struct user_namespace *username, struct inode *dir, struct dentry *dentry, umode_t mode
) {
    return sqlfs_create(username, dir, dentry, mode | S_IFDIR | 0755, 0);
}

static struct dentry *sqlfs_lookup(struct inode *dir, struct dentry *dentry, unsigned int flags){
    struct super_block *sb;
    struct inode *found_inode = NULL;
    char *zErrMesg;
    int rc;
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int did_find;

    BUG_ON(dir->i_ino == 0);

    if (dentry->d_name.len > SQLFS_FILENAME_MAX){
        return ERR_PTR(-ENAMETOOLONG);
    }

    // printk("trying to lookup!");

    if ((rc = ksqlite_open_db("/test", &db)) != SQLITE_OK ) goto eit;

    if ((rc = ksqlite_start_transaction(db, &zErrMesg)) != SQLITE_OK ) goto econn_eit;
    
    if ((rc = sqlite3_prepare(
        db,
        "SELECT id from inode where parentId= :parentId and name = :name limit 1",
        -1,
        &stmt,
        NULL
        ))){
            printk("Error preparing stmt %d\n", rc);
            warn_with_message(db);
            goto econn_eit;
        }
    
    if ((rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":parentId"), dir->i_ino))){
        printk("Error binding stmt %d\n", rc);
        warn_with_message(db);
        goto e_stmt_eit;
    }

    if ((rc = sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, ":name"), dentry->d_name.name, -1, SQLITE_TRANSIENT))){
        printk("Error binding stmt %d\n", rc);
        warn_with_message(db);
        goto e_stmt_eit;
    }

    if ((rc = sqlite3_step(stmt)) != SQLITE_ROW){
        did_find = -1;
        rc = SQLITE_OK;
    }else{
        BUG_ON(rc != SQLITE_ROW);
        did_find = sqlite3_column_int(stmt, 0);
    }
    
    if (did_find > 0){
        sb = dir->i_sb;
        found_inode = iget_locked(sb, did_find);
        if (!(found_inode->i_state & I_NEW)){
            printk("Found in the cache for ino: %d", found_inode);
            goto search_end;
        }
        printk("initializing inode from the database!");
        // Now, we need to read everything for that inode from the database;
        sqlite3_finalize(stmt);
        if ((rc = sqlite3_prepare(
            db,
            "SELECT mode, uid, gid, size, ctime, atime, mtime, nlink from inode where id = :inode_id",
            -1,
            &stmt,
            NULL
        ))){
            printk("failed with %d", rc);
            warn_with_message(db);
            iget_failed(found_inode);
            found_inode = NULL;
            goto e_stmt_eit;
        }
        if ((rc = sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":inode_id"), did_find))){
            printk("failed with %d", rc);
            warn_with_message(db);
            iget_failed(found_inode);
            found_inode = NULL;
            goto e_stmt_eit;
        }
        if ((rc = sqlite3_step(stmt)) != SQLITE_ROW){
            // we just checked the row was there...
            BUG();
        }
        found_inode->i_mode = sqlite3_column_int(stmt, 0);
        i_uid_write(found_inode, sqlite3_column_int(stmt, 1));
        i_gid_write(found_inode, sqlite3_column_int(stmt, 2));

        found_inode->i_size = sqlite3_column_int(stmt, 3);

        found_inode->i_ctime.tv_sec = sqlite3_column_int(stmt, 4);
        found_inode->i_ctime.tv_nsec = 0;

        found_inode->i_atime.tv_sec = sqlite3_column_int(stmt, 5);
        found_inode->i_atime.tv_nsec = 0;

        found_inode->i_mtime.tv_sec = sqlite3_column_int(stmt, 6);
        found_inode->i_mtime.tv_nsec = 0;

        set_nlink(found_inode, sqlite3_column_int(stmt, 7));
        inode_init_owner(&init_user_ns, found_inode, dir, found_inode->i_mode);
        unlock_new_inode(found_inode);
    }

search_end:
    d_add(dentry, found_inode);
    if (found_inode) add_ops(found_inode, found_inode->i_mode);

e_stmt_eit:
    sqlite3_finalize(stmt);

econn_eit:
    if (sqlite3_close(db) != SQLITE_OK){
        printk("Error closing connection!\n");
        BUG();
    }
eit:
    if ((rc != SQLITE_OK) && (rc != SQLITE_ROW)){
        return ERR_PTR(-rc);
    }
    printk("finished lookup!");
    return NULL;
}

static const struct inode_operations sqlfs_inode_ops = {
    .mkdir = sqlfs_mkdir,
    .create = sqlfs_create,
    .lookup = sqlfs_lookup
};


static struct kmem_cache *sqlfs_inode_cache;

int sqlfs_init_inode_cache(void){
    sqlfs_inode_cache = kmem_cache_create_usercopy(
        "sqlfs_cache", sizeof(struct inode), 0, 0, 0,
        sizeof(struct inode), NULL
    );
    if (!sqlfs_inode_cache) return -ENOMEM;
    return 0;
}

void sqlfs_destroy_inode_cache(void){
    kmem_cache_destroy(sqlfs_inode_cache);
}

static struct inode *sqlfs_alloc_inode(struct super_block *sb){
    struct inode *vfs_inode = kmem_cache_alloc(sqlfs_inode_cache, GFP_KERNEL);
    inode_init_once(vfs_inode);
    return vfs_inode;
}

static void sqlfs_destroy_inode(struct inode *inode){
    kmem_cache_free(sqlfs_inode_cache, inode);
}

static struct super_operations sqlfs_super_ops = {
    .alloc_inode = sqlfs_alloc_inode,
    .destroy_inode = sqlfs_destroy_inode
};


int sqlfs_fill_super(struct super_block *sb, void *data, int silent){
    struct inode *root_inode;
    int ret = 0;
    char *zErrMesg;
    sb->s_blocksize = VMACACHE_SIZE;
    sb->s_blocksize_bits = VMACACHE_SIZE;
    sb->s_magic = SQLFS_MAGIC;

    sb->s_op = &sqlfs_super_ops;

    sqlite3 *db;
    mutex_lock(&ksqlite_write_mutex);
    if ((ret = ksqlite_open_db("/test", &db)) != SQLITE_OK ) goto eit;

    if ((ret = ksqlite_start_transaction(db, &zErrMesg)) != SQLITE_OK ) goto econn_eit;

    root_inode = sqlfs_make_inode(sb, S_IFDIR | 0755, "root", NULL, db);

    if (IS_ERR(root_inode) || root_inode == 0){
        ret = PTR_ERR(root_inode);
        goto econn_eit;
    }

    sb->s_root = d_make_root(root_inode);

    if (!sb->s_root){
        ret = -ENOMEM;

        iput(root_inode);
        goto econn_eit;
    }

    ret = sqlite3_exec(db, "COMMIT;", 0, 0, &zErrMesg);
    if (ret != SQLITE_OK){
        printk("Couldn't commit transaction: %s\n", zErrMesg);
        BUG();
    }
    printk("successfully made the root!");

econn_eit:
    if (sqlite3_close(db) != SQLITE_OK){
        printk("Error closing connection!\n");
        BUG();
    }
eit:
    mutex_unlock(&ksqlite_write_mutex);
    return ret;
}

static int sqlfs_open(struct inode *inode, struct file *filp){
    bool wronly = filp->f_flags & O_WRONLY;
    bool rdwr = filp->f_flags & O_RDWR;
    bool trunc = filp->f_flags & O_TRUNC;

    int previous_flags = 0;

    int ret = SQLITE_OK;
    sqlite3 *db;
    sqlite3_stmt *stmt = NULL;

    struct timespec64 time;

    if ((wronly || rdwr) && trunc && inode->i_size){
        if ((ret = ksqlite_setup_for_write(&previous_flags, SQL_LOCKED_FOR_WRITE))){
            goto sql_revert;
        }
        db = (sqlite3*)(current->sql_ref->db);
        if ((ret = sqlite3_prepare(
            db,
            "DELETE FROM partitioned_data where inode_no = :inode_id",
            -1,
            &stmt,
            NULL
        ))){
            printk("Error preparing statement %d\n", ret);
            warn_with_message(db);
            goto sql_revert;
        }

        if ((ret = sqlite3_bind_int(stmt, bug_on_neq(sqlite3_bind_parameter_index(stmt, ":inode_id")), inode->i_ino))){
            printk("Error binding stmt %d\n", ret);
            warn_with_message(db);
            goto sql_revert;
        }

        if ((ret = sqlite3_step(stmt)) != SQLITE_DONE){
            printk("Error stepping %d\n", ret);
            warn_with_message(db);
            goto sql_revert;
        }

        sqlite3_finalize(stmt);
        time = current_time(inode);
        if ((ret = sqlite3_prepare(
            db,
            "UPDATE inode SET size = 0, atime = :atime, mtime = :mtime where id = :inode_id",
            -1,
            &stmt,
            NULL
        ))){
            printk("Error preparing stmt %d\n", ret);
            warn_with_message(db);
            goto sql_revert;
        }

        if ((ret = sqlite3_bind_int(stmt, bug_on_neq(sqlite3_bind_parameter_index(stmt, ":atime")), time.tv_sec)) != SQLITE_OK){
            printk("Error binding stmt %d\n", ret);
            warn_with_message(db);
            goto sql_revert;
        }

        if ((ret = sqlite3_bind_int(stmt, bug_on_neq(sqlite3_bind_parameter_index(stmt, ":mtime")), time.tv_sec)) != SQLITE_OK){
            printk("Error binding stmt %d\n", ret);
            warn_with_message(db);
            goto sql_revert;
        }


        if ((ret = sqlite3_bind_int(stmt, bug_on_neq(sqlite3_bind_parameter_index(stmt, ":inode_id")), inode->i_ino))){
            printk("Error binding stmt %d\n", ret);
            warn_with_message(db);
            goto sql_revert;
        }

        if ((ret = sqlite3_step(stmt)) != SQLITE_DONE){
            printk("Error stepping %d\n", ret);
            warn_with_message(db);
            goto sql_revert;
        }
        
        inode->i_atime = time;
        inode->i_mtime = time;
        inode->i_size = 0;
        ret = SQLITE_OK;
    }

sql_revert:
    sqlite3_finalize(stmt);
    ksqlite_close_for_write(previous_flags, ret == SQLITE_OK);

    if (ret != SQLITE_OK){
        return -ret;
    }

    return 0;
}


int actual_s(int inode_no, sqlite3 *db){
    int rc = SQLITE_OK;

    const char *sql_str = "Select size from inode where id =:inode_no";

    sqlite3_stmt *stmt = NULL;
    int s = 0;
    if ((rc = sqlite3_prepare(db, sql_str,-1,&stmt,NULL))){

        printk("error binding blob");
        warn_with_message(db);
        goto revert;
            
         }

    if ((rc = sqlite3_bind_int(stmt, bug_on_neq(sqlite3_bind_parameter_index(stmt, ":inode_no")), inode_no))){
        printk("error binding blob");
        warn_with_message(db);
        goto revert;
    }

    if ((rc = sqlite3_step(stmt)) != SQLITE_ROW){
        printk("Error stepping");
        warn_with_message(db);
    }else{
        rc = SQLITE_OK;
        s =  sqlite3_column_int(stmt, 0);
    }


revert:
    sqlite3_finalize(stmt);
    return rc == SQLITE_OK ? s : -rc;
}

static ssize_t sqlfs_part_read(
    struct file *file,
    char __user *buf,
    size_t len,
    loff_t *ppos
){
    loff_t offset = *ppos;
    int ret = SQLITE_OK;
    int previous_flags = 0;
    struct inode *inode = file_inode(file);
    size_t actually_read = 0;
    sqlite3_stmt *stmt;
    sqlite3 *db;

    if ((ret = ksqlite_setup_for_write(&previous_flags, SQL_LOCKED_FOR_READ)) != SQLITE_OK){
        goto sql_revert;
    }

    db = (sqlite3*)(current->sql_ref->db);

    ssize_t current_size = actual_s(inode->i_ino, db);
    if (current_size < 0){
        ret = -current_size;
        goto sql_revert;
    }
    if (offset >= current_size) {
        ret = SQLITE_OK;
        actually_read = 0;
        goto sql_revert;
    };
    if (offset + len > current_size){
        len = current_size - offset;
    }

    char *temp_buff = NULL;

    int start_no = (int) offset / KSQLITE_VFS_BLOB_SIZE;
    int last_no = (int) (offset + len) / KSQLITE_VFS_BLOB_SIZE;

    int init_offset = (start_no + 1)*KSQLITE_VFS_BLOB_SIZE - offset;

    int order_iter = 0;

    temp_buff = vmalloc(sizeof(char)*KSQLITE_VFS_BLOB_SIZE);
    for (order_iter = start_no+1; order_iter < last_no; order_iter++){
        if ((ret = get_blob_at_order(order_iter, inode->i_ino, db, temp_buff)) != SQLITE_OK){
            goto sql_revert;
        }
        if ((copy_to_user(buf + init_offset, temp_buff, KSQLITE_VFS_BLOB_SIZE))){
            ret = EFAULT;
            goto sql_revert;
        }
        init_offset += KSQLITE_VFS_BLOB_SIZE;
    }
    memset(temp_buff, 0, KSQLITE_VFS_BLOB_SIZE);
    if ((ret = get_blob_at_order(start_no, inode->i_ino, db, temp_buff)) != SQLITE_OK){
        goto sql_revert;
    }
    int write_start_offset = offset % KSQLITE_VFS_BLOB_SIZE;
    int read_at_most = min((start_no + 1)*KSQLITE_VFS_BLOB_SIZE, offset + len);
    int to_read = read_at_most - offset;
    if ((copy_to_user(buf, temp_buff + write_start_offset, to_read))){
        ret = EFAULT;
        goto sql_revert;
    }
    int last_id = (offset + len) % KSQLITE_VFS_BLOB_SIZE;
    if ((last_no != start_no) && (last_id != 0)){
        memset(temp_buff, 0, KSQLITE_VFS_BLOB_SIZE);
        if ((ret = get_blob_at_order(last_no, inode->i_ino, db, temp_buff))){
            goto sql_revert;
        }
        if ((copy_to_user(buf + len - last_id, temp_buff, last_id))){
            ret = EFAULT;
            goto sql_revert;
        }
    }

    ret = SQLITE_OK;

    *ppos = offset + len;
    actually_read = len;
sql_revert:
    sqlite3_finalize(stmt);
    ksqlite_close_for_write(previous_flags, ret == SQLITE_OK);
    if (ret != SQLITE_OK) return -ret;
    return actually_read;
}

static ssize_t sqlfs_read(
    struct file *file,
    char __user *buf,
    size_t len,
    loff_t *ppos
){
    struct inode *inode = file_inode(file);
    ssize_t bytes_read = 0;
    loff_t pos = *ppos;
    int previous_flags = 0;

    int ret = SQLITE_OK;
    
    int start_order = 0;
    int end_order = 0;

    int start_offset = 0;
    int end_offset = 0;

    int final_row_count = 0;
    int curr = 0;

    int curr_bytes_read = 0;
    sqlite3 *db;
    sqlite3_stmt *stmt = NULL;
    char *current_blob = NULL;
    if (pos > inode->i_size || inode->i_size == 0) return 0;

    if (pos + len > inode->i_size){
        len = inode->i_size - pos;
    }

    if ((ret = ksqlite_setup_for_write(&previous_flags, SQL_LOCKED_FOR_READ)) != SQLITE_OK){
        goto sql_revert;
    }

    printk("trying to read\n");
    db = (sqlite3*)(current->sql_ref->db);

    // we need to start reading from here
    start_order = ((int) pos) / KSQLITE_VFS_BLOB_SIZE;

    start_offset = ((int) pos) % KSQLITE_VFS_BLOB_SIZE;
    // we need to end reading heree
    end_order = ((int) (pos + len)) / KSQLITE_VFS_BLOB_SIZE;

    end_offset = ((int) (pos + len)) % KSQLITE_VFS_BLOB_SIZE;

    final_row_count = end_order - start_order;
    
    BUG_ON(final_row_count < 0);

    if ((ret = sqlite3_prepare(
        db,
        "SELECT IIF(order_no = :last_order, substring(data, 0, :last_offset), data) FROM partitioned_data where partitioned_data.inode_no = :inode_no and partitioned_data.order_no >= :start_id and partitioned_data.order_no <= :end_id order by order_no",
        -1,
        &stmt,
        NULL
    ))){
        printk("Error preparing stmt %d\n", ret);
        warn_with_message(db);
        goto sql_revert;
    }

    if ((ret = sqlite3_bind_int(stmt, bug_on_neq(sqlite3_bind_parameter_index(stmt, ":inode_no")), inode->i_ino)) != SQLITE_OK){
        printk("Error binding stmt %d\n", ret);
        warn_with_message(db);
        goto sql_revert;
    }

    if ((ret = sqlite3_bind_int(stmt, bug_on_neq(sqlite3_bind_parameter_index(stmt, ":start_id")), start_order)) != SQLITE_OK){
        printk("Error binding stmt %d\n", ret);
        warn_with_message(db);
        goto sql_revert;
    }

    if ((ret = sqlite3_bind_int(stmt, bug_on_neq(sqlite3_bind_parameter_index(stmt, ":end_id")), end_order)) != SQLITE_OK){
        printk("Error binding stmt %d\n", ret);
        warn_with_message(db);
        goto sql_revert;
    }

    if ((ret = sqlite3_bind_int(stmt, bug_on_neq(sqlite3_bind_parameter_index(stmt, ":last_order")), end_order)) != SQLITE_OK){
        printk("Error binding stmt %d\n", ret);
        warn_with_message(db);
        goto sql_revert;
    }

    if ((ret = sqlite3_bind_int(stmt, bug_on_neq(sqlite3_bind_parameter_index(stmt, ":last_offset")), end_offset)) != SQLITE_OK){
        printk("Error binding stmt %d\n", ret);
        warn_with_message(db);
        goto sql_revert;
    }

    while ((ret = sqlite3_step(stmt)) == SQLITE_ROW){

        if ((curr++)==final_row_count){
            curr_bytes_read =  end_offset > 0 ? end_offset : KSQLITE_VFS_BLOB_SIZE;
        }else{
            curr_bytes_read = KSQLITE_VFS_BLOB_SIZE;
        }

        curr_bytes_read -= start_offset;

        BUG_ON(curr_bytes_read < 0);

        current_blob = sqlite3_column_blob(stmt, 0);
        //printk("reading current blob: %s\n\n", current_blob);
        if (copy_to_user(buf + bytes_read, current_blob + start_offset, curr_bytes_read)){
            bytes_read = -EFAULT;
            ret = EFAULT;
            goto sql_revert;
        }else{
            bytes_read += curr_bytes_read;
        }
        if (start_offset){
            // we want to read all the other ones "full"
            start_offset = 0;
        }
    }

    BUG_ON(ret != SQLITE_DONE);

    // if ((ret = sqlite3_step(stmt)) != SQLITE_ROW){
    //     // at this point, there should always be a row...
    //     BUG();
    //     printk("Error stepping stmt %d\n", ret);
    //     warn_with_message(db);
    //     goto sql_revert;
    // }

    ret = SQLITE_OK;

    *ppos = pos + len;

sql_revert:
    sqlite3_finalize(stmt);
    ksqlite_close_for_write(previous_flags, ret == SQLITE_OK);
    BUG_ON((ret == SQLITE_OK && bytes_read < 0) || (bytes_read >= 0 && ret != SQLITE_OK));
    return bytes_read;
}


int insert_from_buff(int inode_no, int order_no, const char *buf, sqlite3 *db, sqlite3 **stmt, bool is_update){
    int ret = SQLITE_OK;
    sqlite3_stmt *temp_stmt;

    char *sql;

    if (is_update){
        sql = "UPDATE paritioned_data set data = :data_blob where inode_no = :inode_no and order_no = :order_no";
    }else{
        sql = "INSERT INTO partitioned_data (inode_no, order_no, data) values (:inode_no, :order_no, :data_blob)";
    }

    if ((ret = sqlite3_prepare(
        db,
        sql,
        -1,
        &temp_stmt,
        NULL
    ))){
        printk("Error preparing stmt");
        warn_with_message(db);
        return ret;
    }

    if ((ret = sqlite3_bind_int(temp_stmt, bug_on_neq(sqlite3_bind_parameter_index(temp_stmt, ":inode_no")), inode_no))){
        printk("Error binding stmt: %d", ret);
        warn_with_message(db);
        return ret;
    }
    if ((ret = sqlite3_bind_int(temp_stmt, bug_on_neq(sqlite3_bind_parameter_index(temp_stmt, ":order_no")), order_no))){
        printk("Error binding stmt: %d", ret);
        warn_with_message(db);
        return ret;
    }
    if ((ret = sqlite3_bind_blob(
        temp_stmt,
        bug_on_neq(sqlite3_bind_parameter_index(temp_stmt, ":data_blob")),
        buf,
        KSQLITE_VFS_BLOB_SIZE,
        SQLITE_TRANSIENT
        ))){
            printk("Error binding stmt %d\n", ret);
            warn_with_message(db);
            return ret;
        }

    if ((ret = sqlite3_step(temp_stmt)) != SQLITE_DONE){
        printk("Error stepping %d\n", ret);
        warn_with_message(db);
        return ret;
    }
    return SQLITE_OK;
}

static ssize_t sqlfs_part_write(
    struct file *file,
    const char __user *buff,
    size_t len,
    loff_t *ppos
){
    if (len == 0) return 0;
    struct inode *inode = file_inode(file);
    int ret = SQLITE_OK;

    loff_t pos = *ppos;
    int previous_flags = 0;

    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    struct inode_size * i_size;
    struct inode_size * prev_i_size;

    char *temp_buff = NULL;
    //printk("trying to write");
    if ((ret = ksqlite_setup_for_write(&previous_flags, SQL_LOCKED_FOR_WRITE)) != SQLITE_OK){
        goto sql_revert;
    }

    db = (sqlite3*)(current->sql_ref->db);

    const int actual_size = actual_s(inode->i_ino, db);
    if (actual_size < 0){
        ret = -actual_size;
        goto sql_revert;
    }
    const int current_pos_no = ((int) pos) / KSQLITE_VFS_BLOB_SIZE;
    const int size_no = ((int) actual_size) / KSQLITE_VFS_BLOB_SIZE;
    const int last_no = ((int) (len + pos)) / KSQLITE_VFS_BLOB_SIZE;

    int adjusted_size;
    if ((actual_size % KSQLITE_VFS_BLOB_SIZE) == 0){
        adjusted_size = size_no - 1;
    }else{
        adjusted_size = size_no;
    }

    int start_no = min(adjusted_size, current_pos_no);
    int init_offset = (current_pos_no + 1)*KSQLITE_VFS_BLOB_SIZE - pos;

    int order_iter;
    for (order_iter = start_no + 1; order_iter < last_no; order_iter++){
        if (order_iter == current_pos_no) continue;
        if (order_iter < current_pos_no){
            if ((ret = insert_zero_blob(order_iter, inode->i_ino, db)) != SQLITE_OK){
                goto sql_revert;
            }
            continue;
        }
        if (order_iter > adjusted_size){
            if ((ret = insert_user_blob(order_iter, inode->i_ino, db, buff + init_offset))){
                goto sql_revert;
            }
        } else {
            if ((ret = update_user_blob(order_iter, inode->i_ino, db, buff + init_offset))){
                goto sql_revert;
            }
        }
        init_offset += KSQLITE_VFS_BLOB_SIZE;
    }

    char *first_block = NULL;
    temp_buff = vmalloc(sizeof(char)*KSQLITE_VFS_BLOB_SIZE);
    int needs_insert = 0;
    if (current_pos_no > adjusted_size){
        memset(temp_buff, 0, KSQLITE_VFS_BLOB_SIZE);
        needs_insert = 1;
    }else{
        if ((ret = get_blob_at_order(current_pos_no, inode->i_ino, db, temp_buff))){
            goto sql_revert;
        }
    }
    int to_write_offset = pos % KSQLITE_VFS_BLOB_SIZE;
    int write_at_most = min((current_pos_no + 1)*KSQLITE_VFS_BLOB_SIZE, pos + len);
    // printk("asking to write these many bytes: %d at the position: %d", write_at_most-pos, to_write_offset);
    if(copy_from_user(temp_buff+to_write_offset, buff, (write_at_most - pos))){
        ret = EFAULT;
        goto sql_revert;
    }
    //printk("buff to write: %s\n", temp_buff);

    if (needs_insert){
        if ((ret = insert_blob(current_pos_no, inode->i_ino, db, temp_buff))){
            goto sql_revert;
        }
    }else{
        if ((ret = update_blob(current_pos_no, inode->i_ino, db, temp_buff))){
            goto sql_revert;
        }
    }
    
    const int last_id = (pos + len) % KSQLITE_VFS_BLOB_SIZE;

    // printk("current: %d, size_no: %d, last no: %d adjusted no: %d\n", current_pos_no, size_no, last_no, adjusted_size);
    if ((last_no != current_pos_no) && last_id != 0){
        needs_insert = 0;
        if (last_no > adjusted_size){
            needs_insert = 1;
            memset(temp_buff, 0, KSQLITE_VFS_BLOB_SIZE);
        }else{
            if ((ret = get_blob_at_order(last_no, inode->i_ino, db, temp_buff))){
                goto sql_revert;
            }
        }
        if (copy_from_user(temp_buff, buff + len - last_id, last_id)){
            ret = EFAULT;
            goto sql_revert;
        }
        // printk("buffer now at end: %s", temp_buff);
        if (needs_insert){
            // printk("inserting at end\n");
            if ((ret = insert_blob(last_no, inode->i_ino, db, temp_buff))){
                goto sql_revert;
            }
        }else{
            // printk("ipdating at end\n");
            if ((ret = update_blob(last_no, inode->i_ino, db, temp_buff))){
                goto sql_revert;
            }
        }
    }
    int mod_s;
    if (ret == SQLITE_OK){
        mod_s = max(actual_size, pos + len);
        ret = update_size( mod_s,inode->i_ino, db);
    }

    if (ret == SQLITE_OK){
        // inode->i_size = mod_s;
        *ppos = *ppos + len;
    }

    // if (temp_buff) vfree(temp_buff);
sql_revert:
    sqlite3_finalize(stmt);
    // if we are going to rollback, we just set the on-commit to be null --- since we'll be rolling back the entire transaction anyways.
    if (db){
        if (ret == SQLITE_OK){
            i_size = kmalloc(sizeof(struct inode_size), GFP_KERNEL);
            if (!i_size) goto failed_hook_register;
            i_size->inode = inode;
            i_size->size = mod_s;
            prev_i_size = (struct inode_size *)sqlite3_commit_hook(db, commit_size_change, i_size);
            if (prev_i_size) kfree(prev_i_size);
        }else{
            prev_i_size = (struct inode_size *)sqlite3_commit_hook(db, NULL, NULL);
            if (prev_i_size) kfree(prev_i_size);
        }
    }
failed_hook_register:
    ksqlite_close_for_write(previous_flags, ret == SQLITE_OK);
    if (ret != SQLITE_OK) return -ret;
    return len;
}

// static int sqlfs_part_read(
//     struct file *file,
//     char __user *buf,
//     size_t len,
//     loff_t *ppos
// ){
//     loff_t pos = *ppos;
//     struct inode *inode = file_inode(file);
//     if (pos >= inode->i_size) return 0;

//     if (pos + len > inode->i_size){
//         len = inode->i_size - pos;
//     }

//     const int size_no = ((int) inode->i_size) / KSQLITE_VFS_BLOB_SIZE;
//     const int last_no = ((int) (inode->i_size + pos)) / KSQLITE_VFS_BLOB_SIZE;

//     int init_offset = (size_no + 1)*KSQLITE_VFS_BLOB_SIZE - pos;


// }


// static ssize_t sqlfs_write(
//     struct file *file,
//     const char __user *buf,
//     size_t len,
//     loff_t *ppos
// ){
//     struct inode *inode = file_inode(file);
//     loff_t pos = *ppos;
//     int previous_flags = 0;
//     int ret = SQLITE_OK;

//     sqlite3 *db;
//     sqlite3_stmt *stmt = NULL;

//     int new_size;
//     int bytes_written = 0;

//     int start_order = 0;
//     int end_order = 0;

//     int start_offset = 0;
//     int end_offset = 0;

//     int instl_counter = 0;

//     int size_order = 0;

//     int instl_offset = 0;

//     char temp_buff[KSQLITE_VFS_BLOB_SIZE];

//     if ((ret = ksqlite_setup_for_write(&previous_flags, SQL_LOCKED_FOR_WRITE)) != SQLITE_OK){
//         goto sql_revert;
//     }

//     size_order = ((int) inode->i_size) / KSQLITE_VFS_BLOB_SIZE;

//     start_order = ((int) pos) / KSQLITE_VFS_BLOB_SIZE;
    
//     end_order = ((int) (pos + len)) / KSQLITE_VFS_BLOB_SIZE;

//     db = (sqlite3*)(current->sql_ref->db);

//     // initial empty gap...
//     instl_offset = (KSQLITE_VFS_BLOB_SIZE - (((int) pos ) % KSQLITE_VFS_BLOB_SIZE));

//     for (instl_counter = start_order + 1; instl_counter < end_order; instl_counter++){

//         if (copy_from_user(temp_buff, buf + instl_offset, KSQLITE_VFS_BLOB_SIZE)){
//             ret = EFAULT;
//             goto sql_revert;
//         }
//         if ((ret = insert_from_buff(inode->i_ino, instl_counter, temp_buff, db, &stmt, instl_counter <= size_order))){
//             goto sql_revert;
//         }
//         instl_offset += (KSQLITE_VFS_BLOB_SIZE);
//     }

//     // okay, now we have filled-up and/or entered stuff.
//     // now, let's prepare the first and last buffs

//     if (inode->i_size == 0){
        
//     }else{
        
//     }


//     if ((ret = sqlite3_prepare(
//         db,
//         "SELECT data FROM pipe_data JOIN inode on inode.dataId = pipe_data.dataId where inode.id = :inode_no",
//         -1,
//         &stmt,
//         NULL
//     ))){
//         printk("Error preparing stmt %d\n", ret);
//         warn_with_message(db);
//         goto sql_revert;
//     }

//     if ((ret = sqlite3_bind_int(stmt, bug_on_neq(sqlite3_bind_parameter_index(stmt, ":inode_no")), inode->i_ino)) != SQLITE_OK){
//         printk("Error binding stmt %d\n", ret);
//         warn_with_message(db);
//         goto sql_revert;
//     }

//     if ((ret = sqlite3_step(stmt)) != SQLITE_ROW){
//         printk("Error stepping stmt %d\n", ret);
//         warn_with_message(db);
//         // there should _always_ be a row
//         BUG();
//         goto sql_revert;
//     }

//     ret = SQLITE_OK;

//     const char *current_blob = sqlite3_column_blob(stmt, 0);

//     new_size = inode->i_size + (((pos + len) > inode->i_size) || (inode->i_size == 0) ? pos + len : 0);

//     if (new_size == 0){
//         // nothing to do?
//         goto sql_revert;
//     }

//     temp_buff = vmalloc(new_size);
    
//     if (temp_buff == NULL){
//         ret = ENOMEM;
//         goto sql_revert;
//     }

//     if (inode->i_size) memcpy(temp_buff, current_blob, pos);

//     if (copy_from_user(temp_buff + pos, buf, len)){
//         ret = EFAULT;
//         goto sql_revert;
//     }else{
//         ret = SQLITE_OK;
//     }

//     int remaining = inode->i_size - (pos + len);
//     if (remaining > 0){
//         memcpy(temp_buff + pos + len, current_blob, remaining);
//     }

//     sqlite3_finalize(stmt);

//     if ((ret = sqlite3_prepare(
//         db,
//         "UPDATE pipe_data set data = :data_blob FROM (select dataId from inode where id = :inode_id) as anon where anon.dataId = pipe_data.dataId;",
//         -1,
//         &stmt,
//         NULL
//     ))){
//         printk("Error preparing stmt %d\n", ret);
//         warn_with_message(db);
//         goto sql_revert;
//     }

//     if ((ret = sqlite3_bind_int(stmt, bug_on_neq(sqlite3_bind_parameter_index(stmt, ":inode_id")), inode->i_ino))){
//         printk("Error binding stmt %d\n", ret);
//         warn_with_message(db);
//         goto sql_revert;
//     }

//     if ((ret = sqlite3_bind_blob(
//         stmt, 
//         bug_on_neq(sqlite3_bind_parameter_index(stmt, ":data_blob")),
//         temp_buff,
//         new_size,
//         SQLITE_TRANSIENT
//         ))){
//         printk("Error binding stmt %d\n", ret);
//         warn_with_message(db);
//         goto sql_revert;
//         }
    
//     if ((ret = sqlite3_step(stmt)) != SQLITE_DONE){
//         printk("Error stepping %d\n", ret);
//         warn_with_message(db);
//         goto sql_revert;
//     }

//     ret = SQLITE_OK;
//     bytes_written = len;
//     *ppos = pos + len;
//     inode->i_size += len;

// sql_revert:
//     sqlite3_finalize(stmt);
//     if (temp_buff) vfree(temp_buff);
//     ksqlite_close_for_write(previous_flags, bytes_written > 0);
//     if (ret != SQLITE_OK) return -ret;
//     return bytes_written;
// }

struct file_operations sqlfs_file_ops = {
    .owner  =   THIS_MODULE,
    .open   =   sqlfs_open,
    .read   =   sqlfs_part_read,
    .write  =   sqlfs_part_write,
    .llseek =   generic_file_llseek,
    .fsync  =   noop_fsync
};

static struct dentry *sqlfs_get_super(struct file_system_type *fst, int flags, const char *devname, void *data){
    return mount_nodev(fst, flags, data, sqlfs_fill_super);
}

static struct file_system_type sqlfs_type = {
    .owner      = THIS_MODULE,
    .name       = "sqlfs",
    .mount      = sqlfs_get_super,
    .kill_sb    = kill_litter_super
};

static int __init sqlfs_init(void){
    sqlfs_init_inode_cache();
    return register_filesystem(&sqlfs_type);
}

static void __exit sqlfs_exit(void){
    sqlfs_destroy_inode_cache();
    unregister_filesystem(&sqlfs_type);
}

module_init(sqlfs_init);
module_exit(sqlfs_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vinayak Jha");

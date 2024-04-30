extern void ksqlite_init(void);
extern void ksqlite_insert_into_pid(pid_t, pid_t);
extern void ksqlite_insert_into_pid(pid_t, pid_t);
extern void ksqlite_dup_fd(pid_t, pid_t);
extern void ksqlite_set_up_pipes(int,int);
extern int ksqlite_delete_pid(pid_t);
extern int ksqlite_delete_file(int);
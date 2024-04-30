#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <string.h>

void perf_query(char query[], int size){
    long int amma = syscall(549, query, size);
    printf("query exit: %ld\n", amma);
}
int main()
{
    int fd[2];
    //return 0;
    //printf("current pid: %d\n", getpid());
//    long int pipe_call = syscall(552, fd);
//    printf("Read %d\n", fd[0]);
 //   printf("Write %d\n", fd[1]);
   char test[] = "select * from pid_store where parent_id = 1012 or parent_id = 1013;";
//  char test2[] = "SELECT read_wq, write_wq, data, substr(data, 1, 4), length(data), pipe_data.dataId from pid_pipe join pipe_data on pipe_data.dataId = pid_pipe.dataId"; 
    perf_query(test, sizeof(test));
   return 0;
 // return 0;
    //char test2[] = "select count(*) from pid_pipe;";
    //perf_query(test2, sizeof(test2));
    //char test3[] = "select count(*) from pipe_data;";
    //perf_query(test3, sizeof(test3));
//    printf("System call pipe returned %ld\n", pipe_call);
 //   printf("Read %d\n", fd[0]);
 //   printf("Write %d\n", fd[1]);
/*    char test[5] = "HELLO";
    long int pipe_write = syscall(551, fd[1], test, 5);
    //char buff[] = "Select * from pipe_data;";
    //long int amma = syscall(549, buff, sizeof(buff));
    //char buff2[] = "Select * from pid_pipe;";
    //long int amma = syscall(549, buff2, sizeof(buff2));
    //printf("%ld", amma);
    printf("System call pipe write returned %ld\n", pipe_write);
    char read_back[5];
    long int pipe_read = syscall(550, fd[0], read_back, 4);
    read_back[4] = '\0';
    printf("System call pipe read returned %ld: string %s\n", pipe_read, read_back);
    long int pipe_read2 = syscall(550, fd[0], read_back, 4);
    read_back[4] = '\0';
    printf("Second System call pipe read returned %ld: string %s\n", pipe_read2, read_back);
  */  return 0;
}

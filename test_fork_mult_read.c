#include <stdio.h> 
#include <stdlib.h> 
#include <sys/syscall.h>
#include <unistd.h>  //Header file for sleep(). man 3 sleep for details. 
#include <pthread.h> 

int fd[2];

void reader() 
{ 
    int read_end = fd[0];
    char read_back[9];
    printf("Making read call!\n");
    long int pipe_read = syscall(550, read_end, read_back, 8);
    printf("finished reading with %ld\n", pipe_read);
    read_back[8] = '\0';
    printf("Read str: %s\n", read_back);
    return NULL;
} 

/*
void *reader2(void *vargp) 
{ 
    int read_end = fd[0];
    char read_back[5];
    printf("Making first read call!\n");
    long int pipe_read = syscall(550, read_end, read_back, 4);
    printf("finished reading with %ld bytes\n", pipe_read);
    read_back[4] = '\0';
    printf("Read str: %s\n", read_back);
    return NULL; 
} 
*/
void writer(){
    printf("writer sleeping!\n");
    sleep(1);
    printf("writer wokeup!\n");
    int write_end = fd[1];
    char write_str[9] = "HELLBROK\0";
    long int pipe_write =syscall(551, write_end, write_str, 8);
    printf("Writer writing: %s bytes\n", write_str);
    printf("System call pipe write returned %ld\n", pipe_write);
}
   
int main() 
{ 
    long int pipe_call = syscall(552, fd);
    printf("pipe returned %d\n", pipe_call);
    fflush(stdout);
    int pid;
    pid = fork();
    if (pid < 0){
        printf("fork failed!");
        exit(pid);
    }else if(pid == 0){
        reader();
        int pid2;
        pid2 = fork();
        if (pid2 == 0){reader(); exit(0);}
        writer();
        exit(0);
    }
    writer();
    wait(NULL);
}

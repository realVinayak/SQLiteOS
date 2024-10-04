#include <stdio.h> 
#include <stdlib.h> 
#include <sys/syscall.h>
#include <unistd.h>
#include <pthread.h> 

int fd[2];

void *reader(void *vargp) 
{ 
    int read_end = fd[0];
    char read_back[5];
    long int pipe_read;
    printf("Making read call!\n");
    fflush(stdout);
    pipe_read = syscall(550, read_end, read_back, 4);
    printf("finished reading with %ld\n", pipe_read);
    read_back[4] = '\0';
    printf("Read str: %s\n", read_back);
    pipe_read = syscall(550, read_end, read_back, 4);
    printf("finished reading with %ld\n", pipe_read);
    read_back[4] = '\0';
    printf("Read str: %s\n", read_back);
    char read_back2[9];
    printf("making a request again!\n");
    pipe_read = syscall(550, read_end, read_back2, 9);
    printf("finished reading again with %ld\n", pipe_read);
    read_back2[9] = '\0';
    printf("Read str: %s\n", read_back2);
    return NULL; 
} 

void *writer(void *vargp){
    printf("writer sleeping!\n");
    sleep(5);
    printf("writer wokeup!\n");
    int write_end = fd[1];
    char write_str[5] = "HELLO";
    long int pipe_write =syscall(551, write_end, write_str, 5);
    printf("System call pipe write returned %ld\n", pipe_write);
    printf("writer sleeping again\n");
    sleep(3);
    char write_str2[10] = "whatisdown";
    long int pipe_write2 =syscall(551, write_end, write_str2, 5);
    printf("System call pipe write returned %ld\n", pipe_write2);
}
   
int main() 
{ 
    pthread_t reader_id;
    pthread_t writer_id;

    long int pipe_call = syscall(552, fd);
    if (pipe_call) exit(pipe_call);
    
    pthread_create(&reader_id, NULL, reader, NULL); 
    pthread_create(&writer_id, NULL, writer, NULL);
    pthread_join(reader_id, NULL); 
    pthread_join(writer_id, NULL); 
    exit(0); 
}
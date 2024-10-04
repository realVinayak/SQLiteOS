#include <stdio.h> 
#include <stdlib.h> 
#include <sys/syscall.h>
#include <unistd.h>  //Header file for sleep(). man 3 sleep for details. 
#include <pthread.h> 

int fd[2];

void *reader(void *vargp) 
{ 
    int read_end = fd[0];
    char read_back[5];
    printf("Making second read call!\n");
    long int pipe_read = syscall(550, read_end, read_back, 4);
    printf("finished reading with %ld\n", pipe_read);
    read_back[4] = '\0';
    printf("Read str: %s\n", read_back);
    return NULL; 
} 

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

void *writer(void *vargp){
    printf("writer sleeping!\n");
    sleep(3);
    printf("writer wokeup!\n");
    int write_end = fd[1];
    char write_str[9] = "HELLBROK\0";
    long int pipe_write =syscall(551, write_end, write_str, 8);
    printf("Writer writing: %s bytes\n", write_str);
    printf("System call pipe write returned %ld\n", pipe_write);
}
   
int main() 
{ 
    pthread_t reader_id;
    pthread_t reader_id2;
    pthread_t writer_id;

    long int pipe_call = syscall(552, fd);
    if (pipe_call) exit(pipe_call);
    
    pthread_create(&reader_id, NULL, reader, NULL); 
    pthread_create(&reader_id2, NULL, reader2, NULL); 
    pthread_create(&writer_id, NULL, writer, NULL);
    pthread_join(reader_id, NULL); 
    pthread_join(reader_id2, NULL); 
    pthread_join(writer_id, NULL); 
    exit(0); 
}
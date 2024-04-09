#include <stdlib.h> 
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
int main()
{
 
    // make two process which run same
    // program after this instruction
    pid_t p = fork();
    if(p<0){
      perror("fork fail");
      exit(1);
    }
    printf("Hello world!, process_id(pid) = %d \n",getpid());
    return 0;
}

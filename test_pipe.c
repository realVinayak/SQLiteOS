#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int main() {
  int fd[2];
  pid_t childpid;

  pipe(fd);

  if ((childpid = fork()) == -1) {
    perror("fork");
    exit(1);
  }

  if (childpid == 0) {
    // Child process
    close(fd[1]); // Close the write end of the pipe
    char buf[1024];
    read(fd[0], buf, sizeof(buf)); // Read from the read end of the pipe
    printf("Child read: %s\n", buf);
    close(fd[0]); // Close the read end of the pipe
  } else {
    // Parent process
    close(fd[0]); // Close the read end of the pipe
    char buf[] = "Hello from the parent!\n";
    write(fd[1], buf, sizeof(buf)); // Write to the write end of the pipe
    close(fd[1]); // Close the write end of the pipe
  }

  return 0;
}

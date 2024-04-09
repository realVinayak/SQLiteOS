#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <string.h>
int main()
{
    char buff[] = "Select count(*) from pid_store;";
    long int amma = syscall(549, buff, sizeof(buff));
    printf("System call sys_hello returned %ld\n", amma);
    return 0;
}

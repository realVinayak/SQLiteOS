#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

static char * global_buff;

void __init hello_world_init(void){
        global_buff = kmalloc(8*sizeof(char), GFP_KERNEL);
        global_buff[0] = 'H';
        global_buff[1] = 'E';
        global_buff[2] = 'L';
        global_buff[3] = 'L';
        global_buff[4] = 'O';
        global_buff[5] = 'W';
        global_buff[6] = 'O';
        global_buff[7] = '\0';

}


SYSCALL_DEFINE2(hello_world, char __user *, buf, unsigned long, size)
{
        if (size < 8) return -1;
        if (copy_to_user(buf, global_buff, 8)) return -1;
        printk("Hello world\n");
        return 0;
}
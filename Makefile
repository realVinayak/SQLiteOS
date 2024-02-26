obj-m += ksqlite2.o
ksqlite2-objs := ksqlitemod.o sqlite3.o
PWD := $(CURDIR)
LINUX_KERNEL := ~/WSL2-Linux-Kernel

all:
	make -Wall -C $(LINUX_KERNEL) M=$(shell pwd) modules

clean:
	make -C $(LINUX_KERNEL) M=$(shell pwd) clean
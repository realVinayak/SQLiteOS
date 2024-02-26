obj-m += ksqlite.o
ksqlite-objs := sqlite3.o
PWD := $(CURDIR)
LINUX_KERNEL := ~/WSL2-Linux-Kernel

all:
	make -C $(LINUX_KERNEL) M=$(shell pwd) modules

clean:
	make -C $(LINUX_KERNEL) M=$(shell pwd) clean
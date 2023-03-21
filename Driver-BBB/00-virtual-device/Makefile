obj-m += virtual-driver.o

ARCH := arm;
BBB_KERNEL := /home/duc/workspace/beagle-bone-black/bb-kernel/KERNEL/

TOOLCHAIN := /home/duc/workspace/beagle-bone-black/gcc-linaro-6.5.0-2018.12-x86_64_arm-linux-gnueabihf/bin

PREFIX := arm-linux-gnueabihf-

HOST_KERN_DIR := /lib/modules/`uname -r`/build

all :
	make ARCH=arm CROSS_COMPILE=${TOOLCHAIN}/${PREFIX} -C ${BBB_KERNEL} M=$(PWD) modules

clean :
	make -C ${BBB_KERNEL} M=$(PWD) clean
host:
	make -C $(HOST_KERN_DIR) M=$(PWD) modules
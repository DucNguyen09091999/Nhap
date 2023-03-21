obj-m += procfs.o

ARCH := arm;
BBB_KERNEL := /home/duc/workspace/beagle-bone-black/bb-kernel/KERNEL/

TOOLCHAIN := /home/duc/workspace/beagle-bone-black/gcc-linaro-6.5.0-2018.12-x86_64_arm-linux-gnueabihf/bin

PREFIX := arm-linux-gnueabihf-

HOST_KERN_DIR :=/lib/modules/`uname -r`/build
XCC :=/home/duc/workspace/beagle-bone-black/gcc-linaro-6.5.0-2018.12-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc
all :
	make ARCH=arm CROSS_COMPILE=${TOOLCHAIN}/${PREFIX} -C ${BBB_KERNEL} M=$(PWD) modules
# ${XCC} -o test-app test-app.c

clean :
	make -C ${BBB_KERNEL} M=$(PWD) clean
# rm -rf test-app
host:
	make -C $(HOST_KERN_DIR) M=$(PWD) modules
# gcc -o test-app test-app.c
scp:
	scp  ioctl.ko debian@192.168.7.2:/home/debian/
# scp test-app debian@192.168.7.2:/home/debian/
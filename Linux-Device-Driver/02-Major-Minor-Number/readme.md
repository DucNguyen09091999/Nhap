I. Compile 
    1. Build .ko file
        1.1 run "make" in terminal if arm
        1.2 run "make host" in terminal if x86
    2.run "sudo insmod major-minor.ko" to insert device driver with input parameter.
        dmesg:
            [ 6389.004630] Kernel Module Inserted Successfully...
            [ 6389.011339] Major = 240 Minor = 0
    3.run "sudo rmmod hello_world" to remove device driver
            [ 6405.388993] Kernel Module Removed Successfully...

II. Addition

    1. How Applications will communicate with Hardware devices?

    Application -> Device file/ Device note -> Major and minor number -> Device driver -> Hardware

Initially, the Application will open the device file. This device file is created by the device driver).
Then this device file will find the corresponding device driver using major and minor numbers.
Then that Device driver will talk to the Hardware device.

cat /proc/devices to see list of major number


    2. Character Device Driver Major Number and Minor Number
        - One of the basic features of the Linux kernel is that it abstracts the handling of devices.
        - All hardware devices look like regular files; they can be opened, closed, read, and written using the same, standard, system calls that are used to manipulate files.
        - To Linux, everything is a file. To write to the hard disk, you write to a file.
        - To read from the keyboard is to read from a file.
        - To store backups on a tape device is to write to a file. Even to read from memory is to read from a file. If the file from which you are trying to read or to which you are trying to write is a “normal” file, the process is fairly easy to understand: the file is opened and you read or write data. So the device driver also likes the file.
        - The driver will create a special file for every hardware device. We can communicate with the hardware using those special files (device files).

        - If you want to create a special file, we should know about the major number and minor number in the device driver. 

    3. Major Number and Minor Number
        - The Linux kernel represents character and block devices as pairs of numbers <major>:<minor>.
        3.1 Major number
            - Traditionally, the major number identifies the driver associated with the device. A major number can also be shared by multiple device drivers. See /proc/devices to find out how major numbers are assigned on a running Linux instance.

duc@ubuntu:~/workspace/Linux-Device-Driver/02-Major-Minor-Number$ cat /proc/devices 
Character devices:
  1 mem
  4 /dev/vc/0
  4 tty
  4 ttyS
  5 /dev/tty
  5 /dev/console
  5 /dev/ptmx
These numbers are major numbers.

    3.2 Minor Number
        - The major number is to identify the corresponding driver. Many devices may use the same major number. So we need to assign the number to each device that is using the same major number. So, this is a minor number. In other words, The device driver uses the minor number <minor> to distinguish individual physical or logical devices.

    3.3 Allocating Major and Minor Number
        - We can allocate the major and minor numbers in two ways.
            3.3.1 Statically allocating
                - If you want to set a particular major number for your driver, you can use this method. This method will allocate that major number if it is available. Otherwise, it won’t.

            int register_chrdev_region(dev_t first, unsigned int count, char *name);

        - first: beginning device number of the range you would like to allocate.

        - count: the total number of contiguous device numbers you are requesting.
            Note that, if the count is large, the range you request could spill over to the next major number; but everything will still work properly as long as the number range you request is available.

        - name: the name of the device that should be associated with this number range; it will appear in /proc/devices and sysfs.

- The return value from register_chrdev_region will be 0 if the allocation was successfully performed. In case of error, a negative error code will be returned, and you will not have access to the requested region.

- The dev_t type (defined in <linux/types.h>) is used to hold device numbers—both the major and minor parts. dev_t is a 32-bit quantity with 12 bits set aside for the major number and 20 for the minor number.

- If you want to create the dev_t structure variable for your major and minor number, please use the below function.

        MKDEV(int major, int minor);

- If you want to get your major number and minor number from dev_t, use the below method.
        MAJOR(dev_t dev);
        MINOR(dev_t dev);
- If you pass the dev_t structure to this MAJOR or MONOR function, it will return that major/minor number of your driver.

Example,

    dev_t dev = MKDEV(235, 0);
    register_chrdev_region(dev, 1, "m-dev");
    
    3.3.2 Dynamically Allocating
If we don’t want the fixed major and minor numbers please use this method. This method will allocate the major number dynamically to your driver which is available.

int alloc_chrdev_region(dev_t *dev, unsigned int firstminor, unsigned int count, char *name);

- dev: an output-only parameter that will, on successful completion, hold the first number in your allocated range.

- firstminor should be the requested first minor number to use; it is usually 0.

- count is the total number of contiguous device numbers you are requesting.

- name: the name of the device that should be associated with this number range; it will appear in /proc/devices and sysfs.

3.4 Difference between static and dynamic method
    - A static method is only really useful if you know in advance which major number you want to start with. With the Static method, you are telling the kernel that, what device numbers you want (the start major/minor number and count) and it either gives them to you or not (depending on availability).

    - With the Dynamic method, you are telling the kernel that how many device numbers you need (the starting minor number and count) and it will find a starting major number for you, if one is available, of course.

    - Partially to avoid conflict with other device drivers, it’s considered preferable to use the Dynamic method function, which will dynamically allocate the device numbers for you.

    - The disadvantage of dynamic assignment is that you can’t create the device nodes in advance, because the major number assigned to your module will vary. For normal use of the driver, this is hardly a problem, because once the number has been assigned, you can read it from /proc/devices.

3.5 Unregister the Major and Minor Number
- Regardless of how you allocate your device numbers, you should free them when they are no longer in use. Device numbers are freed with:

    void unregister_chrdev_region(dev_t first, unsigned int count);

The usual place to call unregister_chrdev_region would be in your module’s cleanup function (Exit Function).
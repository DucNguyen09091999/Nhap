I. Compile 
    1. Build .ko file
        1.1 run "make" in terminal if arm
        1.2 run "make host" in terminal if x86
    2.run "sudo insmod device-file.ko" to insert device driver with input parameter.
        dmesg:
            [ 8635.343857] Hello kernel module with passing argument
            [ 8635.349099] Major = 240 Minor = 0
            [ 8635.362388] Kernel Module Inserted Successfully...
    3.run "sudo rmmod hello_world" to remove device driver
            [ 9513.597713] Kernel Module Removed Successfully...
    4. 
        ls -l /dev/m-device 
            crw------- 1 root root 240, 0 Jun 21 19:26 /dev/m-device
            debian@arm:~$ ls -l /dev/ | grep "m-device"
            crw------- 1 root   root    240,   0 Jun 21 19:26 m-device
        debian@arm:~$ ls /sys/class/m-class/m-device
                    dev  power  subsystem  uevent
II. Addition

    1. Automatically Creating Device File
        - The automatic creation of device files can be handled with udev. Udev is the device manager for the Linux kernel that creates/removes device nodes in the /dev directory dynamically. Just follow the below steps.

            - Include the header file linux/device.h and linux/kdev_t.h
            - Create the struct Class
            - Create Device with the class which is created by the above step
            - Create the class

This will create the struct class for our device driver. It will create a structure under/sys/class/.

        struct class * class_create(struct module *owner, const char *name);

            owner – pointer to the module that is to “own” this struct class

            name – pointer to a string for the name of this class

- This is used to create a struct class pointer that can then be used in calls to class_device_create. The return value can be checked using IS_ERR() macro.

Note, the pointer created here is to be destroyed when finished by making a call to class_destroy.

        void class_destroy (struct class * cls);
Create Device
This function can be used by char device classes. A struct device will be created in sysfs, registered to the specified class.

struct device *device_create(struct *class, struct device *parent, dev_t dev, void * drvdata, const char *fmt, ...);
class – pointer to the struct class that this device should be registered to

parent – pointer to the parent struct device of this new device, if any
devt – the dev_t for the char device to be added

drvdata – the data to be added to the device for callbacks

fmt – string for the device’s name

... – variable arguments

A “dev” file will be created, showing the dev_t for the device, if the dev_t is not 0,0. If a pointer to a parent struct device is passed in, the newly created struct device will be a child of that device in sysfs. The pointer to the struct device will be returned from the call. Any further sysfs files that might be required can be created using this pointer. The return value can be checked using IS_ERR() macro.

Note, you can destroy the device using device_destroy().

void device_destroy (struct class * class, dev_t devt);
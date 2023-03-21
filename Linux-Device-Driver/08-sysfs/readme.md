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

   1. Introduction
- The operating system segregates virtual memory into kernel space and userspace.  Kernel space is strictly reserved for running the kernel, kernel extensions, and most device drivers. In contrast, user space is the memory area where all user-mode applications work, and this memory can be swapped out when necessary. There are many ways to Communicate between the Userspace and Kernel Space, they are:
        IOCTL
        Procfs
        Sysfs
        Configfs
        Debugfs
        Sysctl
        UDP Sockets
        Netlink Sockets


    2. SysFS in Linux Kernel Tutorial
- Sysfs is a virtual filesystem exported by the kernel, similar to /proc. The files in Sysfs contain information about devices and drivers. Some files in Sysfs are even writable, for configuration and control of devices attached to the system. Sysfs is always mounted on /sys.

- The directories in Sysfs contain the hierarchy of devices, as they are attached to the computer.

- Sysfs is the commonly used method to export system information from the kernel space to the user space for specific devices. The sysfs is tied to the device driver model of the kernel. The procfs is used to export the process-specific information and the debugfs is used to use for exporting the debug information by the developer.

Before getting into the sysfs we should know about the Kernel Objects.

    2.1 Kernel Objects
- The heart of the sysfs model is the kobject. Kobject is the glue that binds the sysfs and the kernel, which is represented by struct kobject and defined in <linux/kobject.h>. A struct kobject represents a kernel object, maybe a device or so, such as the things that show up as directory in the sysfs filesystem.

Kobjects are usually embedded in other structures.

It is defined as,

    #define KOBJ_NAME_LEN 20 
    struct kobject {
    char *k_name;
    char name[KOBJ_NAME_LEN];
    struct kref kref;
    struct list_head entry;
    struct kobject *parent;
    struct kset *kset;
    struct kobj_type *ktype;
    struct dentry *dentry;
    };
Some of the important fields are:

struct kobject
    |– name (Name of the kobject. Current kobject is created with this name in sysfs.)
    |– parent (This is kobject’s parent. When we create a directory in sysfs for the current kobject, it will create under this parent directory)
    |– ktype (the type associated with a kobject)
    |– kset (a group of kobjects all of which are embedded in structures of the same type)
    |– sd (points to a sysfs_dirent structure that represents this kobject in sysfs.)
    |– kref (provides reference counting)

It is the glue that holds much of the device model and its sysfs interface together.

So kobject is used to create kobject directory in /sys. This is enough. We will not go deep into the kobjects.
    2.2 SysFS in Linux device driver
There are several steps to creating and using sysfs.

    Create a directory in /sys
    Create Sysfs file
    Create a directory in /sys
We can use this function (kobject_create_and_add) to create a directory.

struct kobject * kobject_create_and_add ( const char * name, struct kobject * parent);

Where,

<name> – the name for the kobject

<parent> – the parent kobject of this kobject, if any.

- If you pass kernel_kobj to the second argument, it will create the directory under /sys/kernel/. If you pass firmware_kobj to the second argument, it will create the directory under /sys/firmware/. If you pass fs_kobj to the second argument, it will create the directory under /sys/fs/. If you pass NULL to the second argument, it will create the directory under /sys/.

- This function creates a kobject structure dynamically and registers it with sysfs. If the kobject was not able to be created, NULL will be returned.

When you are finished with this structure, call kobject_put and the structure will be dynamically freed when it is no longer being used.
Example
    struct kobject *kobj_ref;
    /*Creating a directory in /sys/kernel/ */
    kobj_ref = kobject_create_and_add("etx_sysfs",kernel_kobj); //sys/kernel/etx_sysfs
    /*Freeing Kobj*/
    kobject_put(kobj_ref);
    2.2.1 Create Sysfs file
- Using the above function we will create a directory in /sys. Now we need to create a sysfs file, which is used to interact user space with kernel space through sysfs. So we can create the sysfs file using sysfs attributes.
Attributes are represented as regular files in sysfs with one value per file. There are loads of helper functions that can be used to create the kobject attributes. They can be found in the header file sysfs.h

    2.2.2 Create attribute
Kobj_attribute is defined as,

    struct kobj_attribute {
    struct attribute attr;
    ssize_t (*show)(struct kobject *kobj, struct kobj_attribute *attr, char *buf);
    ssize_t (*store)(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count);
    };
Where,

attr – the attribute representing the file to be created,

show – the pointer to the function that will be called when the file is read in sysfs,

 store – the pointer to the function which will be called when the file is written in sysfs.

We can create an attribute using __ATTR macro.

__ATTR(name, permission, show_ptr, store_ptr);

    2.3.3 Store and Show functions
Then we need to write show and store functions.

    ssize_t (*show)(struct kobject *kobj, struct kobj_attribute *attr, char *buf);
    ssize_t (*store)(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count);
Store function will be called whenever we are writing something to the sysfs attribute. See the example.

Show function will be called whenever we are reading the sysfs attribute. See the example.

    2.3.4 Create sysfs file
To create a single file attribute we are going to use ‘sysfs_create_file’.

    int sysfs_create_file ( struct kobject *  kobj, const struct attribute * attr);

Where,

kobj – object we’re creating for.

attr – attribute descriptor.

One can use another function ‘ sysfs_create_group ‘ to create a group of attributes.

Once you have done with the sysfs file, you should delete this file using sysfs_remove_file

    void sysfs_remove_file ( struct kobject *  kobj, const struct attribute * attr);

Where,

kobj – object we’re creating for.

attr – attribute descriptor.

Example
    struct kobj_attribute etx_attr = __ATTR(etx_value, 0660, sysfs_show, sysfs_store);
    static ssize_t sysfs_show(struct kobject *kobj, 
                    struct kobj_attribute *attr, char *buf)
    {
        return sprintf(buf, "%d", etx_value);
    }
    static ssize_t sysfs_store(struct kobject *kobj, 
                    struct kobj_attribute *attr,const char *buf, size_t count)
    {
            sscanf(buf,"%d",&etx_value);
            return count;
    }
    //This Function will be called from Init function
    /*Creating a directory in /sys/kernel/ */
    kobj_ref = kobject_create_and_add("etx_sysfs",kernel_kobj);
    
    /*Creating sysfs file for etx_value*/
    if(sysfs_create_file(kobj_ref,&etx_attr.attr)){
        printk(KERN_INFO"Cannot create sysfs file......\n");
        goto r_sysfs;
    }
    //This should be called from exit function
    kobject_put(kobj_ref); 
    sysfs_remove_file(kernel_kobj, &etx_attr.attr);
    Now we will see the complete driver code. Try this code.

    3. Sysfs example driver Code
In this driver, I have created one integer variable (etx_value). The initial value of that variable is 0. Using sysfs, I can read and modify that variable.



/***************************************************************************//**
*  \file       driver.c
*
*  \details    Simple Linux device driver (sysfs)
*
*  \author     EmbeTronicX
*
*  \Tested with Linux raspberrypi 5.10.27-v7l-embetronicx-custom+
*
*******************************************************************************/
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include<linux/slab.h>                 //kmalloc()
#include<linux/uaccess.h>              //copy_to/from_user()
#include<linux/sysfs.h> 
#include<linux/kobject.h> 
 #include <linux/err.h>
 
volatile int etx_value = 0;
 
 
dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;
struct kobject *kobj_ref;
/*
** Function Prototypes
*/
static int      __init etx_driver_init(void);
static void     __exit etx_driver_exit(void);
 
/*************** Driver functions **********************/
static int      etx_open(struct inode *inode, struct file *file);
static int      etx_release(struct inode *inode, struct file *file);
static ssize_t  etx_read(struct file *filp, 
                        char __user *buf, size_t len,loff_t * off);
static ssize_t  etx_write(struct file *filp, 
                        const char *buf, size_t len, loff_t * off);
 
/*************** Sysfs functions **********************/
static ssize_t  sysfs_show(struct kobject *kobj, 
                        struct kobj_attribute *attr, char *buf);
static ssize_t  sysfs_store(struct kobject *kobj, 
                        struct kobj_attribute *attr,const char *buf, size_t count);
struct kobj_attribute etx_attr = __ATTR(etx_value, 0660, sysfs_show, sysfs_store);
/*
** File operation sturcture
*/
static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .read           = etx_read,
        .write          = etx_write,
        .open           = etx_open,
        .release        = etx_release,
};
/*
** This function will be called when we read the sysfs file
*/
static ssize_t sysfs_show(struct kobject *kobj, 
                struct kobj_attribute *attr, char *buf)
{
        pr_info("Sysfs - Read!!!\n");
        return sprintf(buf, "%d", etx_value);
}
/*
** This function will be called when we write the sysfsfs file
*/
static ssize_t sysfs_store(struct kobject *kobj, 
                struct kobj_attribute *attr,const char *buf, size_t count)
{
        pr_info("Sysfs - Write!!!\n");
        sscanf(buf,"%d",&etx_value);
        return count;
}
/*
** This function will be called when we open the Device file
*/ 
static int etx_open(struct inode *inode, struct file *file)
{
        pr_info("Device File Opened...!!!\n");
        return 0;
}
/*
** This function will be called when we close the Device file
*/ 
static int etx_release(struct inode *inode, struct file *file)
{
        pr_info("Device File Closed...!!!\n");
        return 0;
}
 
/*
** This function will be called when we read the Device file
*/
static ssize_t etx_read(struct file *filp, 
                char __user *buf, size_t len, loff_t *off)
{
        pr_info("Read function\n");
        return 0;
}
/*
** This function will be called when we write the Device file
*/
static ssize_t etx_write(struct file *filp, 
                const char __user *buf, size_t len, loff_t *off)
{
        pr_info("Write Function\n");
        return len;
}
 
/*
** Module Init function
*/
static int __init etx_driver_init(void)
{
        /*Allocating Major number*/
        if((alloc_chrdev_region(&dev, 0, 1, "etx_Dev")) <0){
                pr_info("Cannot allocate major number\n");
                return -1;
        }
        pr_info("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
 
        /*Creating cdev structure*/
        cdev_init(&etx_cdev,&fops);
 
        /*Adding character device to the system*/
        if((cdev_add(&etx_cdev,dev,1)) < 0){
            pr_info("Cannot add the device to the system\n");
            goto r_class;
        }
 
        /*Creating struct class*/
        if(IS_ERR(dev_class = class_create(THIS_MODULE,"etx_class"))){
            pr_info("Cannot create the struct class\n");
            goto r_class;
        }
 
        /*Creating device*/
        if(IS_ERR(device_create(dev_class,NULL,dev,NULL,"etx_device"))){
            pr_info("Cannot create the Device 1\n");
            goto r_device;
        }
 
        /*Creating a directory in /sys/kernel/ */
        kobj_ref = kobject_create_and_add("etx_sysfs",kernel_kobj);
 
        /*Creating sysfs file for etx_value*/
        if(sysfs_create_file(kobj_ref,&etx_attr.attr)){
                pr_err("Cannot create sysfs file......\n");
                goto r_sysfs;
    }
        pr_info("Device Driver Insert...Done!!!\n");
        return 0;
 
r_sysfs:
        kobject_put(kobj_ref); 
        sysfs_remove_file(kernel_kobj, &etx_attr.attr);
 
r_device:
        class_destroy(dev_class);
r_class:
        unregister_chrdev_region(dev,1);
        cdev_del(&etx_cdev);
        return -1;
}
/*
** Module exit function
*/
static void __exit etx_driver_exit(void)
{
        kobject_put(kobj_ref); 
        sysfs_remove_file(kernel_kobj, &etx_attr.attr);
        device_destroy(dev_class,dev);
        class_destroy(dev_class);
        cdev_del(&etx_cdev);
        unregister_chrdev_region(dev, 1);
        pr_info("Device Driver Remove...Done!!!\n");
}
 
module_init(etx_driver_init);
module_exit(etx_driver_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("EmbeTronicX <embetronicx@gmail.com>");
MODULE_DESCRIPTION("Simple Linux device driver (sysfs)");
MODULE_VERSION("1.8");
    5. MakeFile
    obj-m += driver.o
    
    KDIR = /lib/modules/$(shell uname -r)/build
    
    
    all:
        make -C $(KDIR)  M=$(shell pwd) modules
    
    clean:
        make -C $(KDIR)  M=$(shell pwd) clean
    6. Building and Testing Driver
Build the driver by using Makefile (sudo make)
Load the driver using sudo insmod driver.ko
Check the directory in /sys/kernel/ using ls -l /sys/kernel
    linux@embetronicx-VirtualBox: ls -l /sys/kernel/
    drwxr-xr-x 2 root root 0 Dec 17 14:11 boot_params
    drwx------ 26 root root 0 Dec 17 12:19 debug
    drwxr-xr-x 2 root root 0 Dec 17 16:29 etx_sysfs
    drwxr-xr-x 2 root root 0 Dec 17 14:11 fscache
    -r--r--r-- 1 root root 4096 Dec 17 14:11 fscaps
    drwxr-xr-x 2 root root 0 Dec 17 14:11 iommu_groups
    -r--r--r-- 1 root root 4096 Dec 17 14:11 kexec_crash_loaded
    -rw-r--r-- 1 root root 4096 Dec 17 14:11 kexec_crash_size
    -r--r--r-- 1 root root 4096 Dec 17 14:11 kexec_loaded
    drwxr-xr-x 2 root root 0 Dec 17 14:11 livepatch
    drwxr-xr-x 6 root root 0 Dec 17 14:11 mm
    -r--r--r-- 1 root root 516 Dec 17 14:11 notes
    -rw-r--r-- 1 root root 4096 Dec 17 14:11 profiling
    -rw-r--r-- 1 root root 4096 Dec 17 14:11 rcu_expedited
    drwxr-xr-x 4 root root 0 Dec 17 12:19 security
    drwxr-xr-x 117 root root 0 Dec 17 12:19 slab
    dr-xr-xr-x 2 root root 0 Dec 17 14:11 tracing
    -rw-r--r-- 1 root root 4096 Dec 17 12:19 uevent_helper
    -r--r--r-- 1 root root 4096 Dec 17 12:19 uevent_seqnum
    -r--r--r-- 1 root root 4096 Dec 17 14:11 vmcoreinfo
Now our sysfs entry is there under /sys/kernel directory.
Now check the sysfs file in etx_sysfs using ls -l /sys/kernel/etx_sysfs
    linux@embetronicx-VirtualBox: ls -l /sys/kernel/etx_sysfs
    -rw-rw----    1    root    root    4096    Dec 17 16:37    etx_value
Our sysfs file also there. Now go under root permission using sudo su.
Now read that file using cat /sys/kernel/etx_sysfs/etx_value
    linux@embetronicx-VirtualBox#cat /sys/kernel/etx_sysfs/etx_value
    0

So Value is 0 (initial value is 0). Now, modify the value using the echo command.
linux@embetronicx-VirtualBox#echo 123 > /sys/kernel/etx_sysfs/etx_value
Now again read that file using cat /sys/kernel/etx_sysfs/etx_value
    linux@embetronicx-VirtualBox#cat /sys/kernel/etx_sysfs/etx_value
    123

So our sysfs is working fine.
Unload the module using sudo rmmod driver
This is a simple example using sysfs in the device drivers. This is just a basic. I hope this might help you. In our next tutorial, we will discuss interrupts in Linux.
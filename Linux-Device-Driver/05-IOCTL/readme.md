I. Compile 
    1. Build .ko file
        1.1 run "make" in terminal if arm
        1.2 run "make host" in terminal if x86
    2.run "sudo insmod ioctl.ko" to insert device driver with input parameter.
        dmesg:
            [  770.273624] ioctl: loading out-of-tree module taints kernel.
            [  770.273653] ioctl: module verification failed: signature and/or required key missing - tainting kernel
            [  770.275209] [INFO] Major = 243, Minor = 0
            [  770.275478] Hello world kernel module
    3.run "sudo rmmod ioctl" to remove device driver
            [ 1253.117758] goodbye


    4. With test-app
        duc@ubuntu:~/workspace/Linux-Device-Driver/05-IOCTL$ ./test-app
            *********************************
            *******    Duc Nguyen     *******

            Opening Driver
            Enter the Value to send
            23456
            Writing Value to Driver
            Reading Value from Driver
            Value is 23456
            Closing Driver
    ==>>
            [  792.677290] [INFO] System call open() called...
            [  795.983160] Value = 23456
            [  795.983190] [INFO] System call release() called...


        
II. Addition

    1. Introduction
        - The operating system segregates virtual memory into kernel space and userspace.  Kernel space is strictly reserved for running the kernel, kernel extensions, and most device drivers. In contrast, user space is the memory area where all user-mode applications work, and this memory can be swapped out when necessary. 
There are many ways to Communicate between the Userspace and Kernel Space, they are:
            IOCTL
            Procfs
            Sysfs
            Configfs
            Debugfs
            Sysctl
            UDP Sockets
            Netlink Sockets
In this tutorial, we will see IOCTL.

    2. IOCTL in Linux
        2.1 IOCTL
- IOCTL is referred to as Input and Output Control, which is used to talk to device drivers. This system call is available in most driver categories.  The major use of this is in case of handling some specific operations of a device for which the kernel does not have a system call by default.

- Some real-time applications of ioctl are Ejecting the media from a “cd” drive, changing the Baud Rate of Serial port, Adjusting the Volume, Reading or Writing device registers, etc. We already have the write and read function in our device driver. But it is not enough for all cases.
        2.2 Steps involved in IOCTL
There are some steps involved to use IOCTL.

        Create IOCTL command in the driver
        Write IOCTL function in the driver
        Create IOCTL command in a Userspace application
        Use the IOCTL system call in a Userspace
        Create IOCTL Command in the Driver
To implement a new ioctl command we need to follow the following steps.

2.3.1 Define the ioctl command

#define "ioctl name" __IOX("magic number","command number","argument type")
where IOX can be :
“IO“: an ioctl with no parameters
“IOW“: an ioctl with write parameters (copy_from_user)
“IOR“: an ioctl with read parameters (copy_to_user)
“IOWR“: an ioctl with both write and read parameters

- The Magic Number is a unique number or character that will differentiate our set of ioctl calls from the other ioctl calls. some times the major number for the device is used here.
Command Number is the number that is assigned to the ioctl. This is used to differentiate the commands from one another.
The last is the type of data.
2.3.2. Add the header file linux/ioctl.h to make use of the above-mentioned calls.

Example
    #include <linux/ioctl.h>
    #define WR_VALUE _IOW('a','a',int32_t*)
    #define RD_VALUE _IOR('a','b',int32_t*)
Write IOCTL function in the driver
- The next step is to implement the ioctl call we defined into the corresponding driver. We need to add the ioctl function to our driver. Find the prototype of the function below.

int  ioctl(struct inode *inode,struct file *file,unsigned int cmd,unsigned long arg)
Where

    <inode>: is the inode number of the file being worked on.
    <file>: is the file pointer to the file that was passed by the application.
    <cmd>: is the ioctl command that was called from the userspace.
    <arg>: are the arguments passed from the userspace

- Within the function “ioctl” we need to implement all the commands that we defined above (WR_VALUE, RD_VALUE). We need to use the same commands in the switch statement which is defined above.

- Then we need to inform the kernel that the ioctl calls are implemented in the function “etx_ioctl“. This is done by making the fops pointer “unlocked_ioctl” to point to “etx_ioctl” as shown below.

Example
/*
** This function will be called when we write IOCTL on the Device file
*/
static long etx_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
         switch(cmd) {
                case WR_VALUE:
                        if( copy_from_user(&value ,(int32_t*) arg, sizeof(value)) )
                        {
                                pr_err("Data Write : Err!\n");
                        }
                        pr_info("Value = %d\n", value);
                        break;
                case RD_VALUE:
                        if( copy_to_user((int32_t*) arg, &value, sizeof(value)) )
                        {
                                pr_err("Data Read : Err!\n");
                        }
                        break;
                default:
                        pr_info("Default\n");
                        break;
        }
        return 0;
}
/*
** File operation sturcture
*/
static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .read           = etx_read,
        .write          = etx_write,
        .open           = etx_open,
        .unlocked_ioctl = etx_ioctl,
        .release        = etx_release,
};
Now we need to call the ioctl command from a user application.

Create IOCTL command in a Userspace application
Just define the ioctl command like how we defined it in the driver.

Example:

#define WR_VALUE _IOW('a','a',int32_t*)
#define RD_VALUE _IOR('a','b',int32_t*)
Use IOCTL system call in Userspace
Include the header file <sys/ioctl.h>.Now we need to call the new ioctl command from a user application.

long ioctl( "file descriptor","ioctl command","Arguments");
Where,

<file descriptor>: This the open file on which the ioctl command needs to be executed, which would generally be device files.
<ioctl command>: ioctl command which is implemented to achieve the desired functionality
<arguments>: The arguments need to be passed to the ioctl command.

Example
ioctl(fd, WR_VALUE, (int32_t*) &number); 
ioctl(fd, RD_VALUE, (int32_t*) &value);

Now we will see the complete driver and application.
IOCTL in Linux – Device Driver Source Code
In this example we only implemented IOCTL. In this driver, I’ve defined one variable (int32_t value). Using ioctl command we can read or change the variable. So other functions like open, close, read, and write, We simply left empty. 
   
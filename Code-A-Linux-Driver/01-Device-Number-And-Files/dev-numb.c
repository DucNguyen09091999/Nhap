#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h> //chr_dev - file ops
// #define pr_fmt(fmt) "%s:%s: " fmt, KBUILD_MODNAME, __func__

#define DRIVER_AUTHOR       "DucNguyen Nguyenchiduc090999@gmail.com"
#define DRIVER_DESCRIPTION  "Register a device driver and implement some callback functions"
#define KERNEL_VERSION      "1.0"
#define LICENSE             "GPL"
#define BOARD_INFO          "Beaglebone black Rev. C"

/*
 *@brief this function is called when the device file is opened
 */

static int driver_open(struct inode *device_file, struct file *instance)
{
        pr_info("device - open was called\n");
        return 0;
}

/*
 *@brief this function is called when the device file is closed
 */

static int driver_close(struct inode *device_file, struct file *instance)
{
        pr_info("device - closed was called\n");
        return 0;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = driver_open,
    .release = driver_close
};

/*
 *@brief this function is called when the module is loaded into the kernel
 */

#define MYMAJOR 64


static int
__init Hello_World_LKM_Init(void)
{
    int ret_val;
    /*Register file operations*/
    ret_val = register_chrdev(MYMAJOR, "my_dev_nr", &fops);
	if(ret_val == 0) {
		printk("dev_nr - registered Device number Major: %d, Minor: %d\n", MYMAJOR, 0);
	}
	else if(ret_val > 0) {
		printk("dev_nr - registered Device number Major: %d, Minor: %d\n", ret_val>>20, ret_val&0xfffff);
	}
	else {
		printk("Could not register device number!\n");
		return -1;
	}
    pr_info("Hello World LKM loaded\n");
    return 0;
}
/*
 *@brief this function is called when the module is removed from the kernel
 */
static void
__exit Hello_World_LKM_Exit(void)
{
    unregister_chrdev(MYMAJOR, "my_dev_nr");
    pr_info("Hello World LKM unloaded\n");
}

module_init(Hello_World_LKM_Init);
module_exit(Hello_World_LKM_Exit);

/*META INFORMATION*/
MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESCRIPTION);  
MODULE_VERSION(KERNEL_VERSION);
MODULE_INFO(board, BOARD_INFO);

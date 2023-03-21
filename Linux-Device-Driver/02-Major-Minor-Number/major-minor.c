/***************************************************************************//**
*  \file       major-minor.c
*
*  \details    Create major and minor number using dynamic method
*
*  \author     Duc Nguyen
*
* *******************************************************************************/
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>


#define AUTHOR_INFO     "DucNguyen nguyenchiduc090999@gmail.com"
#define AUTHOR_DESC     "Create major and minor "
#define BOARD_NAME      "Beaglebone black rev. C"

#define DEV_NAME "m_cdev"

struct m_private_dev
{
    dev_t dev_num;
}mdev;

/*
** Module Init function
*/
static int __init hello_world_init(void)
{
    
    pr_info("Hello kernel module with passing argument\n");
    pr_info("Kernel Module Inserted Successfully...\n");
    /*Allocating Major number*/
    if((alloc_chrdev_region(&mdev.dev_num, 0, 1, DEV_NAME)) <0){
        pr_info("Cannot allocate major number for device 1\n");
        return -1;
    }
        pr_info("Major = %d Minor = %d \n",MAJOR(mdev.dev_num), MINOR(mdev.dev_num));
    return 0;
}
/*
** Module Exit function
*/
static void __exit hello_world_exit(void)
{
    unregister_chrdev_region(mdev.dev_num, 1);
    pr_info("Kernel Module Removed Successfully...\n");
}
 
module_init(hello_world_init);
module_exit(hello_world_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR(AUTHOR_INFO);
MODULE_DESCRIPTION(AUTHOR_DESC);
MODULE_VERSION("1.0");
MODULE_INFO(board, BOARD_NAME);
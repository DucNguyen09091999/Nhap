/***************************************************************************//**
*  \file       driver.c
*
*  \details    first linux device driver
*
*  \author     Duc Nguyen
*
* *******************************************************************************/
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>

#define AUTHOR_INFO     "DucNguyen nguyenchiduc090999@gmail.com"
#define AUTHOR_DESC     "First linux device driver"
#define BOARD_NAME      "Beaglebone black rev. C"

/*
** Module Init function
*/
static int __init hello_world_init(void)
{
    
    printk(KERN_INFO "This is the first linux device driver\n");
    printk(KERN_INFO "Kernel Module Inserted Successfully...\n");
    return 0;
}
/*
** Module Exit function
*/
static void __exit hello_world_exit(void)
{
    printk(KERN_INFO "Kernel Module Removed Successfully...\n");
}
 
module_init(hello_world_init);
module_exit(hello_world_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR(AUTHOR_INFO);
MODULE_DESCRIPTION(AUTHOR_DESC);
MODULE_VERSION("1.0");
MODULE_INFO(board, BOARD_NAME);
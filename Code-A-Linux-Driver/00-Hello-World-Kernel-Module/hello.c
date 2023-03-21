#include <linux/module.h>
#include <linux/init.h>

// #define pr_fmt(fmt) "%s:%s: " fmt, KBUILD_MODNAME, __func__

#define DRIVER_AUTHOR       "DucNguyen Nguyenchiduc090999@gmail.com"
#define DRIVER_DESCRIPTION  "A hello world linux kernel module"
#define KERNEL_VERSION      "1.0"
#define LICENSE             "GPL"
#define BOARD_INFO          "Beaglebone black Rev. C"
/*
 *@brief this function is called when the module is loaded into the kernel
 */

static int
__init Hello_World_LKM_Init(void)
{
    pr_info("Hello World LKM loaded\n");
    return 0;
}
/*
 *@brief this function is called when the module is removed from the kernel
 */
static void
__exit Hello_World_LKM_Exit(void)
{
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

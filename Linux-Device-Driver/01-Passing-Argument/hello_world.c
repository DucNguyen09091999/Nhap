/***************************************************************************//**
*  \file       hello_world.c
*
*  \details    Passing argument using param
*
*  \author     Duc Nguyen
*
* *******************************************************************************/
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/moduleparam.h>

int ext_value = 0;
int ext_arr[4]; //Declare external variable and array
char *ext_name;
int ext_callback_value = 0;

int notify_param(const char *val, const struct kernel_param *kp)
{
        int res = param_set_int(val, kp); // Use helper for write variable
        if(res==0) {
               pr_info("Call back function called...\n");
                pr_info("New value of cb_valueETX = %d\n", ext_callback_value);
                return 0;
        }
        return -1;
}

const struct kernel_param_ops ext_param_ops = 
{
        .set = &notify_param, // Use our setter ...
        .get = &param_get_int, // .. and standard getter
};

module_param(ext_value,int,S_IRUSR | S_IWUSR); //declare module param for int: ext_value with Read Write permission
module_param(ext_name,charp,S_IRUSR | S_IWUSR);
module_param_array(ext_arr,int,NULL,S_IRUSR | S_IWUSR);
module_param_cb(ext_callback_value, &ext_param_ops, &ext_callback_value, S_IRUSR|S_IWUSR );



#define AUTHOR_INFO     "DucNguyen nguyenchiduc090999@gmail.com"
#define AUTHOR_DESC     "First linux device driver"
#define BOARD_NAME      "Beaglebone black rev. C"

/*
** Module Init function
*/
static int __init hello_world_init(void)
{
    int i;
    pr_info("Hello kernel module with passing argument\n");
    pr_info("Kernel Module Inserted Successfully...\n");
    pr_info("External value = %d\n",ext_value);
    pr_info("External callback value = %d\n",ext_callback_value);
    pr_info("External character = %s\n",ext_name);
    
    for( i = 0; i < (sizeof(ext_arr)/sizeof(int));i++)
    {
        pr_info("arr[%d] = %d\n",i,ext_arr[i]);
    }
    return 0;
}
/*
** Module Exit function
*/
static void __exit hello_world_exit(void)
{
    pr_info("Kernel Module Removed Successfully...\n");
}
 
module_init(hello_world_init);
module_exit(hello_world_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR(AUTHOR_INFO);
MODULE_DESCRIPTION(AUTHOR_DESC);
MODULE_VERSION("1.0");
MODULE_INFO(board, BOARD_NAME);
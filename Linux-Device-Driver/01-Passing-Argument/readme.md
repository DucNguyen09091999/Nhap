I. Compile 
    1. Build .ko file
        1.1 run "make" in terminal if arm
        1.2 run "make host" in terminal if x86
    2.run "sudo insmod hello_world.ko ext_value=14 ext_name="Nguyen" ext_arr=100,101,102,103" to insert device driver with input parameter.
        dmesg:
            [ 3564.054246] Hello kernel module with passing argument
            [ 3564.059445] Kernel Module Inserted Successfully...
            [ 3564.065603] External value = 14
            [ 3564.069653] External callback value = 0
            [ 3564.074378] External character = Nguyen
            [ 3564.079033] arr[0] = 100
            [ 3564.081580] arr[1] = 101
            [ 3564.085402] arr[2] = 102
            [ 3564.088769] arr[3] = 103
        sudo su
        echo 20 > /sys/module/hello_world/parameters/ext_callback_value
            [ 3854.666718] Call back function called...
            [ 3854.678355] New value of cb_valueETX = 20

    3.run "sudo rmmod hello_world" to remove device driver
            [ 3961.725396] Kernel Module Removed Successfully...

II. Addition
    1. Type of permission
        S_IWUSR
        S_IRUSR
        S_IXUSR
        S_IRGRP
        S_IWGRP
        S_IXGRP
    In this S_I is a common header.
    R = read ,W =write ,X= Execute.
    USR =user ,GRP =Group
    Using OR ‘|’ (or operation) we can set multiple permissions at a time.


    

    2. Module Parameters Macros
        module_param()
        module_param_array()
        module_param_cb()
    
        2.1 module_param()
            - Module_param() macros is used to define the normal variable that we are going to pass to the driver.
               
                syntax: module_param(name, type, perm);
            
            - Module_param takes three parameters:
                - the name of the variable
                - its type <goto type_note in the end of file>
                - its permissions mask
                
            - The macro should be placed outside of any function and is typically found near the head of the source file.
            - module_param() macro is defined in linux/moduleparam.h.
            - module_param() macro creates the sub-directory under /sys/module.
            For example,
                module_param(valueETX, int, S_IWUSR|S_IRUSR);
                    -> This will create a sysfs entry. (/sys/module/<module name>/parameters/valueETX)

        2.2 module_param_array()
            - This macro is used to send the array as an argument to the Linux device driver. 
            - Array parameters, where the values are supplied as a comma-separated list, are also supported by the module loader.

            Syntax: module_param_array(name,type,num,perm);

            - module_param_array() takes four parameter:
                - The name of array (and of the parameter),
                - The type of the array elements,
                - Num is an integer variable (optional) otherwise NULL,
                - The usual permissions value.

        2.3 module_param_cb()
            - This macro is used to register the callback.
            - Whenever the argument (parameter) got changed, this callback function will be called. 
                For Example,
                    I have created one parameter by using module_param().
                    module_param(valueETX, int, S_IWUSR|S_IRUSR);
                    This will create a sysfs entry. (/sys/module/<module name>/parameters/valueETX)
                    You can change the value of valueETX from the command line by
                        sudo su
                        echo 1 > /sys/module/<module name>/parameters/valueETX
                        This will update the valueETX variable. But there is no way to notify your module that “valueETX” has changed.

                    By using this module_param_cb() macro, we can get a notification.

            If you want to get a notification whenever the value got to change, we need to register our handler function to its file operation structure first.

            struct kernel_param_ops 
            {
            int (*set)(const char *val, const struct kernel_param *kp);
            int (*get)(char *buffer, const struct kernel_param *kp);
            void (*free)(void *arg);
            };



type_note:

- Numerous types are supported for module parameters:
    bool
    invbool
A boolean (true or false) value (the associated variable should be of type int).
The invbool type inverts the value, so that true values become false and vice versa.
    charp
A char pointer value. Memory is allocated for user-provided strings, and the pointer is set accordingly.
    int
    long
    short
    uint
    ulong
    ushort
Basic integer values of various lengths. The versions starting with u are for unsigned values.
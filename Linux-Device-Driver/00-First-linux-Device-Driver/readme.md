I. Compile 
    1. Build .ko file
        1.1 run "make" in terminal if arm
        1.2 run "make host" in terminal if x86
    2.run "sudo insmod ./driver.c" to insert device driver.
    3.run "sudo rmmod driver" to remove device driver 
II. Addition

pr_info – Print an info-level message. (ex. pr_info("test info message\n")).
pr_cont – Continues a previous log message in the same line.
pr_debug – Print a debug-level message conditionally.
pr_err – Print an error-level message. (ex. pr_err(“test error message\n”)).
pr_warn – Print a warning-level message.


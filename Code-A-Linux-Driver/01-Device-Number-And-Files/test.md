duc@ubuntu:~/workspace/Code-A-Linux-Driver/01-Device-Number-And-Files$ cat /proc/devices | grep my_dev_nr
duc@ubuntu:~/workspace/Code-A-Linux-Driver/01-Device-Number-And-Files$ sudo insmod dev-numb.ko
duc@ubuntu:~/workspace/Code-A-Linux-Driver/01-Device-Number-And-Files$ cat /proc/devices | grep my_dev_nr
 64 my_dev_nr
duc@ubuntu:~/workspace/Code-A-Linux-Driver/01-Device-Number-And-Files$ sudo mknod /dev/my_device c 90 0
duc@ubuntu:~/workspace/Code-A-Linux-Driver/01-Device-Number-And-Files$ ls /dev/my_device -al
crw-r--r-- 1 root root 90, 0 Nov 17 03:32 /dev/my_device
duc@ubuntu:~/workspace/Code-A-Linux-Driver/01-Device-Number-And-Files$ dmesg | tail -n 4
[  468.728905] Hello World LKM loaded
[ 1781.025405] Hello World LKM unloaded
[ 1937.468292] dev_nr - registered Device number Major: 64, Minor: 0
[ 1937.468293] Hello World LKM loaded
duc@ubuntu:~/workspace/Code-A-Linux-Driver/01-Device-Number-And-Files$ sudo rmmod dev_numb
duc@ubuntu:~/workspace/Code-A-Linux-Driver/01-Device-Number-And-Files$ dmesg | tail -n 2
[ 1937.468293] Hello World LKM loaded
[ 2164.947049] Hello World LKM unloaded
duc@ubuntu:~/workspace/Code-A-Linux-Driver/01-Device-Number-And-Files$

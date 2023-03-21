duc@ubuntu:~/workspace/Code-A-Linux-Driver/00-Hello-World-Kernel-Module$ sudo insmod hello.ko
[sudo] password for duc:
duc@ubuntu:~/workspace/Code-A-Linux-Driver/00-Hello-World-Kernel-Module$ sudo rmmod hello
duc@ubuntu:~/workspace/Code-A-Linux-Driver/00-Hello-World-Kernel-Module$ dmesg -n 10
dmesg: unknown level '10'
duc@ubuntu:~/workspace/Code-A-Linux-Driver/00-Hello-World-Kernel-Module$ dmesg | tail
[   16.251725] IPv6: ADDRCONF(NETDEV_UP): ens33: link is not ready
[   16.257065] e1000: ens33 NIC Link is Up 1000 Mbps Full Duplex, Flow Control: None
[   16.258289] IPv6: ADDRCONF(NETDEV_CHANGE): ens33: link becomes ready
[   26.155210] Bluetooth: RFCOMM TTY layer initialized
[   26.155215] Bluetooth: RFCOMM socket layer initialized
[   26.155220] Bluetooth: RFCOMM ver 1.11
[ 3029.120746] hello: loading out-of-tree module taints kernel.
[ 3029.120956] hello: module verification failed: signature and/or required key missing - tainting kernel
[ 3029.125184] Hello World LKM loaded
[ 3033.992570] Hello World LKM unloaded
duc@ubuntu:~/workspace/Code-A-Linux-Driver/00-Hello-World-Kernel-Module$ objdump -h hello.ko

hello.ko:     file format elf64-x86-64

Sections:
Idx Name          Size      VMA               LMA               File off  Algn
  0 .note.gnu.build-id 00000024  0000000000000000  0000000000000000  00000040  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  1 .text         00000000  0000000000000000  0000000000000000  00000064  2**0
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
  2 .init.text    00000019  0000000000000000  0000000000000000  00000064  2**0
                  CONTENTS, ALLOC, LOAD, RELOC, READONLY, CODE
  3 .exit.text    00000012  0000000000000000  0000000000000000  0000007d  2**0
                  CONTENTS, ALLOC, LOAD, RELOC, READONLY, CODE
  4 .rodata.str1.1 00000036  0000000000000000  0000000000000000  0000008f  2**0
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  5 .modinfo      0000010c  0000000000000000  0000000000000000  000000c8  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  6 __mcount_loc  00000008  0000000000000000  0000000000000000  000001d8  2**3
                  CONTENTS, ALLOC, LOAD, RELOC, READONLY, DATA
  7 .data         00000000  0000000000000000  0000000000000000  000001e0  2**0
                  CONTENTS, ALLOC, LOAD, DATA
  8 .gnu.linkonce.this_module 00000340  0000000000000000  0000000000000000  00000200  2**6
                  CONTENTS, ALLOC, LOAD, RELOC, DATA, LINK_ONCE_DISCARD
  9 .bss          00000000  0000000000000000  0000000000000000  00000540  2**0
                  ALLOC
 10 .comment      0000006c  0000000000000000  0000000000000000  00000540  2**0
                  CONTENTS, READONLY
 11 .note.GNU-stack 00000000  0000000000000000  0000000000000000  000005ac  2**0
                  CONTENTS, READONLY
duc@ubuntu:~/workspace/Code-A-Linux-Driver/00-Hello-World-Kernel-Module$ objdump hello.ko -d -j .modinfo

hello.ko:     file format elf64-x86-64


Disassembly of section .modinfo:

0000000000000000 <__UNIQUE_ID_board31>:
   0:   62 6f 61 72 64 3d 42 65 61 67 6c 65 62 6f 6e 65     board=Beaglebone
  10:   20 62 6c 61 63 6b 20 52 65 76 2e 20 43 00            black Rev. C.

000000000000001e <__UNIQUE_ID_version30>:
  1e:   76 65 72 73 69 6f 6e 3d 31 2e 30 00                 version=1.0.

000000000000002a <__UNIQUE_ID_description29>:
  2a:   64 65 73 63 72 69 70 74 69 6f 6e 3d 41 20 68 65     description=A he
  3a:   6c 6c 6f 20 77 6f 72 6c 64 20 6c 69 6e 75 78 20     llo world linux
  4a:   6b 65 72 6e 65 6c 20 6d 6f 64 75 6c 65 00           kernel module.

0000000000000058 <__UNIQUE_ID_author28>:
  58:   61 75 74 68 6f 72 3d 44 75 63 4e 67 75 79 65 6e     author=DucNguyen
  68:   20 4e 67 75 79 65 6e 63 68 69 64 75 63 30 39 30      Nguyenchiduc090
  78:   39 39 39 40 67 6d 61 69 6c 2e 63 6f 6d 00           999@gmail.com.

0000000000000086 <__UNIQUE_ID_license27>:
  86:   6c 69 63 65 6e 73 65 3d 47 50 4c 00 00 00 00 00     license=GPL.....
        ...

0000000000000098 <__UNIQUE_ID_srcversion30>:
  98:   73 72 63 76 65 72 73 69 6f 6e 3d 38 41 37 32 43     srcversion=8A72C
  a8:   36 35 44 37 34 36 30 38 39 31 32 43 45 33 32 46     65D74608912CE32F
  b8:   32 36 00 00 00 00 00 00                             26......

00000000000000c0 <__module_depends>:
  c0:   64 65 70 65 6e 64 73 3d 00                          depends=.

00000000000000c9 <__UNIQUE_ID_retpoline29>:
  c9:   72 65 74 70 6f 6c 69 6e 65 3d 59 00                 retpoline=Y.

00000000000000d5 <__UNIQUE_ID_name28>:
  d5:   6e 61 6d 65 3d 68 65 6c 6c 6f 00                    name=hello.

00000000000000e0 <__UNIQUE_ID_vermagic27>:
  e0:   76 65 72 6d 61 67 69 63 3d 34 2e 31 35 2e 30 2d     vermagic=4.15.0-
  f0:   31 34 32 2d 67 65 6e 65 72 69 63 20 53 4d 50 20     142-generic SMP
 100:   6d 6f 64 5f 75 6e 6c 6f 61 64 20 00                 mod_unload .
duc@ubuntu:~/workspace/Code-A-Linux-Driver/00-Hello-World-Kernel-Module$ modinfo ./hello.ko
filename:       /home/duc/workspace/Code-A-Linux-Driver/00-Hello-World-Kernel-Module/./hello.ko
board:          Beaglebone black Rev. C
version:        1.0
description:    A hello world linux kernel module
author:         DucNguyen Nguyenchiduc090999@gmail.com
license:        GPL
srcversion:     8A72C65D74608912CE32F26
depends:
retpoline:      Y
name:           hello
vermagic:       4.15.0-142-generic SMP mod_unload

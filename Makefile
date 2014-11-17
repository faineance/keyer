obj-m += keyer.o
<<<<<<< HEAD
KERNEL_DIR = /lib/modules/$(shell uname -r)/build
all:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules
clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) clean
	rm -rf *.o *.ko *.symvers *.mod.* *.order
=======

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
>>>>>>> ab4d8dc23bb672531a641dd19de9ec8c759f8d08

obj-m := HoneyPot.o
	KERNELBUILD := /lib/modules/`uname -r`/build
default:
	make -C $(KERNELBUILD) M=$(shell pwd) modules
clean:
	rm -rf *.o .*.cmd *.ko *.mod.c .tmp_versions

obj-m += mr.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

load: all unload
	sync
	sync
	sudo insmod mr.ko

unload:
	if lsmod | grep -q '^mr\>' ; then \
		sudo rmmod mr ; \
	fi


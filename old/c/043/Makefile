KDIR:=/lib/modules/$(shell uname -r)/build
KARCH:=$(shell uname -m)

NAME=ztest
SRC=${NAME}.c
APP=${NAME}
MOD=${NAME}.o
KMOD=${NAME}.ko

obj-m:=${MOD}

# ------------

default: ${APP} ${MOD}

${APP}: ${SRC}
	gcc -O2 -Wall -o ${APP} -lz ${SRC}

run: ${APP}
	./${APP}

mod: ${MOD} ${KMOD}

${MOD} ${KMOD}: ${SRC}
	$(MAKE) -C $(KDIR) ARCH=$(KARCH) SUBDIRS=$(PWD) modules

insmod: ${KMOD}
	sudo insmod ${KMOD}

rmmod:
	sudo rmmod ${NAME}

clean:
	-rm -rf *~ ${APP} .*.cmd *.mod.c *.o *.ko .tmp* *.symvers .${MOD}.d

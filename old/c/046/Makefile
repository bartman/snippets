A=a
ASRC=a.c cyg.c
AOBJ=$(ASRC:%.c=%.o)

CC      = gcc
CFLAGS  = -O1 -Wall -g -ggdb
LDFLAGS = -g -rdynamic

TARGETS = ${A}

all: ${TARGETS}

#${A}: CFLAGS=-std=c99
${A}: ${AOBJ}

$(AOBJ): Makefile
$(filter-out cyg.o, ${AOBJ}): CFLAGS += -finstrument-functions

run: ${A}
	./${A}

clean:
	-rm -f *~ *.o ${TARGETS}

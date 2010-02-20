
A=a
ASRC=a.c
AOBJ=a.o

CFLAGS=-O -Wall
TARGETS=${A}

all: ${TARGETS}

${A}: CFLAGS= -std=c99
${A}: ${AOBJ}

run:
	./${A}

clean:
	-rm -f *~ *.o ${TARGETS}


A=a
ASRC=a.c
AOBJ=$(patsubs %.c,%.o,${ASRC})

B=b
BSRC=b.c
BOBJ=$(patsubs %.c,%.o,${BSRC})

CFLAGS=-O2 -m64 -ggdb
TARGETS=${A} ${B}

all: ${TARGETS}

${A}: ${AOBJ}

${B}: ${BOBJ}

test: ${A} ${B}
	./${A}
	./${B}

clean:
	-rm -f *~ *.o ${TARGETS}

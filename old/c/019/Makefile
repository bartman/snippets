
A=a
ASRC=a.c
AOBJ=$(patsubs %.c,%.o,${ASRC})

B=b
BSRC=b.c
BOBJ=$(patsubs %.c,%.o,${BSRC})

CFLAGS=-O1 -Wall -ggdb
TARGETS=${A} ${B}

all: ${TARGETS}

${A}: ${AOBJ}

${B}: ${BOBJ}

clean:
	-rm -f *~ *.o ${TARGETS}

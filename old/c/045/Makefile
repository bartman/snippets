
A=a
ASRC=a.c
AOBJ=$(patsubs %.cpp,%.o,${ASRC})

CFLAGS=-O1 -Wall -ggdb
TARGETS=${A} ${A}

all: ${TARGETS}

#${A}: CFLAGS=-std=c99
${A}: ${AOBJ}

run: ${A}
	./${A}

clean:
	-rm -f *~ *.o ${TARGETS}

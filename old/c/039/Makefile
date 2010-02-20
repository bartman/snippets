
A=a
ASRC=a.c
AOBJ=$(patsubs %.cpp,%.o,${ASRC})

CFLAGS=-O1 -Wall -ggdb
TARGETS=${A} ${A}

all: ${TARGETS}

${A}: CFLAGS=-lpthread -std=c99
${A}: ${AOBJ}

run:
	./${A}

clean:
	-rm -f *~ *.o ${TARGETS}

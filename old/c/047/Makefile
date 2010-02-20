
A=a
ASRC=a.c
AOBJ=$(patsubs %.cpp,%.o,${ASRC})

CFLAGS=-O0 -Wall
TARGETS=${A} ${A}

all: ${TARGETS}

${A}: CFLAGS= -std=c99
${A}: ${AOBJ}

run:
	./${A}

clean:
	-rm -f *~ *.o ${TARGETS}

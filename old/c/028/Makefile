
A=a
ASRC=a.cpp
AOBJ=$(patsubs %.cpp,%.o,${ASRC})

CFLAGS=-O1 -Wall -ggdb
TARGETS=${A} ${A}

all: ${TARGETS}

${A}: CFLAGS=-lreadline $(TERMCAP_LIB) -std=c99
${A}: ${AOBJ}

run:
	./${A}

clean:
	-rm -f *~ *.o ${TARGETS}


A=a
ASRC=a.c
AOBJ=$(patsubs %.c,%.o,${ASRC})

B=b
BSRC=b.cpp
BOBJ=$(patsubs %.cpp,%.o,${BSRC})

CFLAGS=-O1 -Wall -ggdb
TARGETS=${A} ${B}

all: ${TARGETS}

${A}: CFLAGS=-lreadline $(TERMCAP_LIB) -std=c99
${A}: ${AOBJ}

${B}: CFLAGS=-lreadline $(TERMCAP_LIB)
${B}: ${BOBJ}

${ASRC} ${BSRC}: i.h

clean:
	-rm -f *~ *.o ${TARGETS}

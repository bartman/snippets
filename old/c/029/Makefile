
A=a
ASRC=a.c
AOBJ=$(patsubs %.c,%.o,${ASRC})

CFLAGS=-O1 -Wall -ggdb
TARGETS=${A}

all: ${TARGETS}

#${A}: CFLAGS=-lreadline $(TERMCAP_LIB)
${A}: ${AOBJ}

run: ${A}
	./${A}

clean:
	-rm -f *~ *.o ${TARGETS}

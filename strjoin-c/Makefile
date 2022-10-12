
APP=strjoin
ASRC=${APP}.c
AOBJ=${APP}.o

CFLAGS=-O -Wall
TARGETS=${APP}

all: ${TARGETS}

${APP}: CFLAGS= -std=c99
${APP}: ${AOBJ}

run:
	./${APP}

clean:
	-rm -f *~ *.o ${TARGETS}

print-% : ; @echo "$* = $($*)"

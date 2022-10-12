
APP=stdarg
ASRC=${APP}.c
AOBJ=${APP}.o

CFLAGS=-O2 -Wall -g
TARGETS=${APP}

all: ${TARGETS}

${APP}: ${AOBJ}

run:
	./${APP}

clean:
	-rm -f *~ *.o ${TARGETS}

print-% : ; @echo "$* = $($*)"

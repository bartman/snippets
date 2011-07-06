
BIN=primes
SRC=primes.c
OBJ=$(SRC:.c=.o)

CC = 3gcc
LFLAGS =
LIBS = 
CFLAGS=-O3 -ffast-math -funroll-loops -fomit-frame-pointer -march=athlon -mcpu=athlon

ifdef DEBUG
CFLAGS+=-DDEBUG -ggdb
endif

all: ${BIN}

# --- build

${BIN}: ${OBJ}
	${CC} ${LFLAGS} -o ${BIN} ${LIBS} ${OBJ}

${OBJ}: %.o: %.c .depend
	${CC} ${CFLAGS} -c -o $@ $<

# --- profiling

PBI=${BIN}.p
REP=${BIN}.rep
POB=$(SRC:.c=.p.o)
PFLAGS=-DPROFILE -pg -a
DFLAGS=-DDEBUG -ggdb
PLIBS=${LIBS} -lm

prof: ${REP}
	less ${REP}

${PBI}: ${POB}
	${CC} ${LFLAGS} -o ${PBI} ${PLIBS} ${POB} ${PFLAGS}

gmon.out: ${PBI}
	time ./${BIN}.p 1000000 > /dev/null

${REP}: gmon.out
	gprof -b -p -q -A -s ${PBI} gmon.out > ${REP}

${POB}: %.p.o: %.c .depend
	${CC} ${DFLAGS} -c -o $@ $< ${PFLAGS}

# ---- maintenance

depend: ; rm .depend ; ${MAKE} .depend

.depend: ${SRC} Makefile
	${CC} ${CFLAGS} -MM ${SRC} >> .depend

clean:
	rm -f ${BIN} ${OBJ} ${PBI} ${POB} *.out gmon.sum ${REP} core *~

include .depend


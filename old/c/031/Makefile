
A=a
ASRC=a.cpp
AOBJ=$(patsubs %.cpp,%.o,${ASRC})

CFLAGS=-O2 -Wall -Werror -ggdb
CXXFLAGS=-O2 -Wall -Werror -ggdb -Wno-conversion -Wno-pmf-conversions
TARGETS=${A} ${A}

all: ${TARGETS}

${A}: CFLAGS=-lreadline $(TERMCAP_LIB) -std=c99
${A}: ${AOBJ}

run:
	./${A}

clean:
	-rm -f *~ *.o ${TARGETS}

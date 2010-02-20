
CXX=g++-3.2
CC=${CXX}

A=a
ASRC=a.cpp ac.cpp
AOBJ=$(patsubst %.cpp,%.o,${ASRC})

CFLAGS=-O2 -Wall -Werror -ggdb
CXXFLAGS=-O2 -Wall -Werror -ggdb -Wno-conversion -Wno-pmf-conversions
TARGETS=${A}

all: ${TARGETS}

${A}: CFLAGS=-lreadline $(TERMCAP_LIB) -std=c99
${A}: ${AOBJ}

run:
	./${A}

clean:
	-rm -f *~ *.o ${TARGETS}

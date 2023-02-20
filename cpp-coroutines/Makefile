CSRCS = $(wildcard *.c)
CEXES = ${CSRCS:%.c=%}

CXXSRCS = $(wildcard *.cpp)
CXXEXES = ${CXXSRCS:%.cpp=%}

RSSRCS = $(wildcard *.rs)
RSEXES = ${RSSRCS:%.rs=%}

EXES = ${CEXES} ${CXXEXES} ${RSEXES}

ASAN = -g -fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined

CFLAGS = -Os --std=c11 -Wall -Werror ${ASAN} -ggdb
CPPFLAGS = -Os --std=c++20 -fcoroutines -Wall -Werror ${ASAN} -ggdb -traditional-cpp
LDFLAGS = ${ASAN} -pthread -ldl -lzmq -lczmq

Q = $(if ${V},,@)

all: ${EXES}

exe:
	@echo "CEXES=${CEXES}"
	@echo "RSEXES=${RSEXES}"

${CEXES}: %: %.c Makefile
	@echo " BUILD $@ ($<)"
	${Q}${CC} ${CFLAGS} -o $@ $< $(LDFLAGS)

${CXXEXES}: %: %.cpp Makefile
	@echo " BUILD $@ ($<)"
	${Q}${CXX} ${CPPFLAGS} -o $@ $< $(LDFLAGS)

slp: LDFLAGS += -lm

sr: LDFLAGS += -lm

${RSEXES}: %: %.rs Makefile
	@echo " BUILD $@ ($<)"
	${Q}rustc -o $@ $<

clean:
	rm -f *~
	rm -f ${EXES}

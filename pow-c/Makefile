CSRCS = $(wildcard *.c)
CEXES = ${CSRCS:%.c=%}

EXES = ${CEXES}

CPPFLAGS =
CFLAGS = -g -O3 -Wall -Werror -march=native -mtune=native

Q = $(if ${V},,@)

all: ${EXES}

${CEXES}: %: %.c Makefile
	@echo " BUILD $@ ($<)"
	${Q}${CC} ${CPPFLAGS} ${CFLAGS} -o $@ $< $(LDFLAGS)

print-% : ; @echo "$* = $($*)"

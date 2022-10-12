TARGET = foo
SRCS = foo.c
OBJS = ${SRCS:%.c=%.o}

#CPPFLAGS = 
CFLAGS = -g -O1 -Wall -Werror -finstrument-functions
LDFLAGS = -pthread

Q = $(if ${V},,@)

all: ${TARGET}

${TARGET}: %: %.o Makefile
	@echo " BUILD $@ ($<)"
	${Q}${CC} ${CPPFLAGS} ${CFLAGS} $(LDFLAGS) -o $@ $<

${OBJS}: %.o: %.c Makefile
	@echo " CC $@ ($<)"
	${Q}${CC} ${CPPFLAGS} ${CFLAGS} $(LDFLAGS) -o $@ -c $<

print-% : ; @echo "$* = $($*)"

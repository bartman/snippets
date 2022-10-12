
PYTHON3_CFLAGS = $(shell pkg-config --cflags python3)

CFLAGS = -Wall -O2 -ggdb ${PYTHON3_CFLAGS}

HDRS = example.h
SRCS = example.c example_wrap.c
OBJS = ${SRCS:.c=.o}
LIB  = _example.so

.PHONY: all clean
all: ${LIB}

example_wrap.c example.py: example.i example.h Makefile
	swig3.0 -python -py3 $<

ifeq (0,1)
#
# NOTE: this did not work for some reason, I need to build and link in
# one gcc invocatiion, otherwise it complains about duplicate definitions
# of all exported methods.
#
${OBJS}: ${SRCS} Makefile
	gcc ${CFLAGS} -c -fPIC -o $@ $<

${LIB}: ${OBJS}
	ld -shared $^ -o $@
else
${LIB}: ${SRCS} ${HDRS} Makefile
	gcc ${CFLAGS} -shared -fPIC -o $@ ${SRCS}
endif

clean:
	rm -f *.o *~ *.so *.pyc
	rm -f example_wrap.c example.py
	rm -rf __pycache__

print-% : ; @echo "$* = $($*)"

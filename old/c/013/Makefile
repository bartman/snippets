
FINAL = t

OBJS = heap.o minheap.o main.o

SRCS = $(OBJS:.o=.c)

CC=gcc
CFLAGS=-O0 -ggdb


all: $(FINAL)

clean:
	@rm -f $(FINAL) $(OBJS) *~

$(OBJS): heap.h

tags:
	ctags $(SRCS) heap.h

$(FINAL): $(OBJS)
	$(CC) -o $@ $^


FINAL = t

OBJS = heap.o minheap.o minheap2.o main.o

SRCS = $(OBJS:.o=.c)

CC=gcc
CFLAGS=-O0 -ggdb


all: $(FINAL)

clean:
	@rm -f $(FINAL) $(OBJS) *~

$(OBJS): heap.h

.PHONY: tags clean
tags:
	ctags $(SRCS) heap.h

$(FINAL): $(OBJS)
	$(CC) -o $@ $^

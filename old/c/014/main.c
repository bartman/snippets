#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "heap.h"

const int no_enqueue_a = 100;
const int no_enqueue_b = 0;
const int no_dequeue_a = 100;
const int no_dequeue_b = 0;

#define enq_tot 100
int enq[enq_tot];
int enq_cnt = 0;

#define HEAP_ADD minheap2_add
#define HEAP_GET minheap2_get_first
#define HEAP_DUMP heap2_dump

static inline void enq_init (void);
static inline void do_enqueue (heap_t *heap, int no_enqueue);
static inline void do_dequeue (heap_t *heap, int no_dequeue);

int 
main (int argc, char *argv)
{
	heap_t heap;

	printf ("min heap of size %u\n\n", HEAP_SIZE);

	heap_init (&heap);

	enq_init ();

	if (no_enqueue_a)
		do_enqueue (&heap, no_enqueue_a);

	if (no_enqueue_b)
		do_enqueue (&heap, no_enqueue_b);

//	debug|=2;

	if (no_dequeue_a)
		do_dequeue (&heap, no_dequeue_a);

	if (no_dequeue_b)
		do_dequeue (&heap, no_dequeue_b);

	return 0;
}

static inline void
do_enqueue (heap_t *heap, int no_enqueue)
{
	int i, rc;

	printf ("--------------------------------------------------\n");
	printf ("enqueue %u...\n", no_enqueue);

	for (i=0; i<no_enqueue; i++) {
		elem_t value = enq[(enq_cnt++) % enq_tot];

		printf (" @%2u ++ %2d", i, value);
		rc = HEAP_ADD (heap, value);
		if (rc)
			printf (" !! failed\n");
		else 
			printf ("\n");
	}

	printf ("\n");
	HEAP_DUMP (heap);
	printf ("\n");
}

static inline void
do_dequeue (heap_t *heap, int no_dequeue)
{
	int i, rc;

	printf ("--------------------------------------------------\n");
	printf ("dequeue %u...\n", no_dequeue);
	for (i=0; i<no_dequeue; i++) {
		elem_t value; 
		printf (" @%2u", i);
		rc = HEAP_GET (heap, &value);
		if (rc)
			printf (" !! failed\n");
		else 
			printf (" -- %2d\n", value);
	}

	printf ("\n");
	HEAP_DUMP (heap);
	printf ("\n");
}

static inline void 
enq_init (void)
{
	int i;

	enq_cnt = 0;
	for (i=0; i<enq_tot; i++) {
		if (i&1)
			enq[i] = enq_tot - i;
		else
			enq[i] = i;

	}
}

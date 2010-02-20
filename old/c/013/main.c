#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "heap.h"

int no_enqueue = HEAP_SIZE;
int no_dequeue_a = (HEAP_SIZE/2);
int no_dequeue_b = (HEAP_SIZE/2);

int 
main (int argc, char *argv)
{
	heap_t heap;
	int i, rc;

	printf ("min heap of size %u\n\n", HEAP_SIZE);

	heap_init (&heap);

	printf ("enqueue %u...\n", no_enqueue);

	for (i=0; i<no_enqueue; i++) {
		elem_t value = ((i * 11) + (i * 13)) % no_enqueue;
		printf (" @%2u ++ %2d", i, value);
		rc = minheap_add (&heap, value);
		if (rc)
			printf (" !! failed\n");
		else 
			printf ("\n");
	}

	printf ("\n");
	heap_dump (&heap);
	printf ("\n");

	printf ("dequeue %u...\n", no_dequeue_a);
	for (i=0; i<no_dequeue_a; i++) {
		elem_t value; 
		printf (" @%2u", i);
		rc = minheap_get_first (&heap, &value);
		if (rc)
			printf (" !! failed\n");
		else 
			printf (" -- %2d\n", value);
	}

	printf ("\n");
	heap_dump (&heap);
	printf ("\n");

//	debug=1;

	printf ("dequeue %u...\n", no_dequeue_b);
	for (i=0; i<no_dequeue_b; i++) {
		elem_t value; 
		printf (" @%2u", i+no_dequeue_a);
		rc = minheap_get_first (&heap, &value);
		if (rc)
			printf (" !! failed\n");
		else 
			printf (" -- %2d\n", value);
	}

	printf ("\n");
	heap_dump (&heap);
	printf ("\n");



	return 0;
}


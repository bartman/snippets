#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "heap.h"

int debug = 0;

void 
heap_init (heap_t *heap)
{
	int i;
	heap->size = HEAP_SIZE;
	heap->cnt = 0;
	heap->min = ((unsigned)-1) >> 2;
	heap->max = 0;
	for (i=0; i<HEAP_SIZE; i++)
		heap->data[i] = -1;
}


ofs_t 
heap_parent_of (heap_t *heap, ofs_t ofs)
{
	ofs_t ret;

	if (!ofs || ofs >= heap->cnt) {
		//BUG("ofs=%u, cnt=%u", ofs, heap->cnt);
		return OFS_INVAL;
	}

	ret = (ofs-1) / 2;

	if (ret >= heap->cnt) {
		BUG("ret=%u, cnt=%u", ret, heap->cnt);
		return OFS_INVAL;
	}

	return ret;
}


ofs_t 
heap_left_of (heap_t *heap, ofs_t ofs)
{
	ofs_t ret = (ofs*2) + 1;

	if (ret >= heap->cnt) {
		BUG("left(%u) ret=%u, cnt=%u", ofs, ret, heap->cnt);
		BP();
		return OFS_INVAL;
	}

	return ret;
}

ofs_t 
heap_right_of (heap_t *heap, ofs_t ofs)
{
	ofs_t ret = (ofs*2) + 2;

	if (ret >= heap->cnt) {
		BUG("right(%u) ret=%u, cnt=%u", ofs, ret, heap->cnt);
		BP();
		return OFS_INVAL;
	}

	return ret;
}


int 
heap_is_empty (heap_t *heap)
{
	return heap->cnt == 0;
}

int 
heap_is_full (heap_t *heap)
{
	return heap->cnt >= heap->size;
}


void 
heap_swap (heap_t *heap, ofs_t a, ofs_t b)
{
	elem_t t = heap->data[a];
	heap->data[a] = heap->data[b];
	heap->data[b] = t;
}

void 
__heap_dump (const char *prefix, heap_t *heap)
{
	int i, l, w, p;
	printf ("%scnt=%u/%u  min=%u  max=%u\n", prefix,
			heap->cnt, heap->size, heap->min, heap->max);

	if (0 == heap->cnt) {
		printf ("%sdata = { }\n", prefix);
		return;
	}

	/* compute the prefix for first line */
	for (p=1, i=heap->cnt; i; p<<=1, i>>=1);

	printf ("%sdata =\n", prefix);
	for (l=0,w=1; l<heap->cnt; l+=w, w*=2, p/=2) {

		printf ("%s    %2u:  ", prefix, l);
		for (i=0; i<w && (l+i)<heap->cnt; i++) {
			printf ("%*d", p, heap->data[l+i]);
			printf ("%*s", p, "");
		}
		printf ("\n");
	}
}


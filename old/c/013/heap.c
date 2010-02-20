#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "heap.h"

int debug = 0;

void 
heap_init (heap_t *heap)
{
	int i;
	heap->cnt = 0;
	for (i=0; i<HEAP_DEPTH; i++)
		heap->data[i] = -1;
}

ofs_t 
heap_parent_of (heap_t *heap, ofs_t ofs)
{
	ofs_t ret;

	if (!ofs || ofs >= heap->cnt) {
		BUG("ofs=%u, cnt=%u", ofs, heap->cnt);
		return OFS_INVAL;
	}

	ret = ofs / 2;

	if (ret >= heap->cnt) {
		BUG("ret=%u, cnt=%u", ret, heap->cnt);
		return OFS_INVAL;
	}

	return ret;
}

ofs_t 
heap_left (heap_t *heap, ofs_t ofs)
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
heap_right (heap_t *heap, ofs_t ofs)
{
	ofs_t ret = (ofs*2) + 2;

	if (ret >= heap->cnt) {
		BUG("right(%u) ret=%u, cnt=%u", ofs, ret, heap->cnt);
		BP();
		return OFS_INVAL;
	}

	return ret;
}

void 
heap_swap (heap_t *heap, ofs_t a, ofs_t b)
{
	elem_t t = heap->data[a];
	heap->data[a] = heap->data[b];
	heap->data[b] = t;
}

void 
heap_dump (heap_t *heap)
{
	int i, l, w, p;
	printf ("cnt = %u\n", heap->cnt);

	if (0 == heap->cnt) {
		printf ("data = { }\n");
		return;
	}

	/* compute the prefix for first line */
	for (p=1, i=heap->cnt; i; p<<=1, i>>=1);

	printf ("data =\n");
	for (l=0,w=1; l<heap->cnt; l+=w, w*=2, p/=2) {

		printf ("\t%2u:  ", l);
		for (i=0; i<w && (l+i)<heap->cnt; i++) {
			printf ("%*d", p, heap->data[l+i]);
			printf ("%*s", p, "");
		}
		printf ("\n");
	}
}


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>

#include "heap.h"


int
minheap_add (heap_t *heap, elem_t elem)
{
	ofs_t loc, par;

	if (heap->cnt >= HEAP_SIZE)
		return -ENOMEM;

	loc = heap->cnt;
	heap->cnt ++;

	/* place new element in the next spot */
	heap->data[loc] = elem;

	/* while we have a parent move to it and see if order is maintained */
	while ((par = heap_parent_of (heap, loc)) != OFS_INVAL) {

		key_t par_key = elem_key (heap->data[par]);
		key_t loc_key = elem_key (heap->data[loc]);

		/* parent must be smaller */
		if (par_key < loc_key)
			goto done;

		/* if not then swap the elements */
		heap_swap (heap, par, loc);

		/* now move up */
		loc = par;
	}

done:
	return 0;
}

int
minheap_get_first (heap_t *heap, elem_t *elem)
{
	elem_t ret;
	ofs_t loc, end;
	key_t key;

	if (heap->cnt == 0)
		return -ENOENT;

	/* this is what we are returning */
	ret = heap->data[0];

	/* move the last element into the first location, shrinking the heap
	 * by one */
	heap->data[0] = heap->data[--heap->cnt];
	heap->data[heap->cnt] = -1;

	/* now move down fixing the order */
	loc = 0;
	key = elem_key (heap->data[loc]);
	end = (heap->cnt-1)/2;
pd ("cnt = %d\n", heap->cnt);
pd ("end = %d\n", end);
	while (loc < end) {
		ofs_t min_ofs;
		ofs_t left_ofs = heap_left (heap, loc);
		ofs_t right_ofs = heap_right (heap, loc);

		key_t min_key;
		key_t left_key = elem_key (heap->data[left_ofs]);
		key_t right_key = elem_key (heap->data[right_ofs]);

pd ("loc   = @%d [%d]\n", loc, key);
pd ("left  = @%d [%d]\n", left_ofs, left_key);
pd ("right = @%d [%d]\n", right_ofs, right_key);

		if (left_key < right_key) {
			min_key = left_key;
			min_ofs = left_ofs;
		} else {
			min_key = right_key;
			min_ofs = right_ofs;
		}

		/* parent must be smaller */
		if (key < min_key)
			goto done;

		/* if not then swap the elements */
pd (">> swap   = @%d [%d] <-> @%d [%d]\n", loc, key, min_ofs, min_key);
		heap_swap (heap, loc, min_ofs);

		/* move to swapped child */
		loc = min_ofs;
		//key = min_key;
pd ("loc   = @%d [%d]\n", loc, key);
pd ("\n");
	}

	/* it's possible that we have a node on our path with only one child; 
	 * in this case the tree count is even and if it is then we must only
	 * compare to the left subtree */
	if ((loc == end) && !(heap->cnt & 1)) {
		ofs_t left_ofs = heap_left (heap, loc);
		key_t left_key;
		
pd ("bonus\n");

		if (left_ofs == OFS_INVAL) {
pd ("...bailed\n");
			goto done;
		}

		left_key = elem_key (heap->data[left_ofs]);

pd ("loc   = @%d [%d]\n", loc, key);
pd ("left  = @%d [%d]\n", left_ofs, left_key);
		/* parent must be smaller */
		if (key < left_key)
			goto done;

pd (">> swap   = @%d [%d] <-> @%d [%d]\n", loc, key, left_ofs, left_key);
		/* if not then swap the elements */
		heap_swap (heap, loc, left_ofs);
	}

done:
	*elem = ret;
	return 0;
}

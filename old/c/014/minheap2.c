#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>

#include "heap.h"

/* level 1 heap 
 * 	is a minheap that stores all elements
 *
 * level 2 heap 
 * 	stores level 1 heaps in order such that all elements of a level 1 heap
 * 	are smaller then then the next level 1 heap.
 */

static inline heap_t* minheap2_create_level1_heap (heap_t *level2_heap,
		uint index);
static inline void minheap2_delete_level1_heap (heap_t *level2_heap, 
		uint index);

static inline int minheap2_add_first (heap_t *level2_heap, elem_t elem, 
		key_t elem_key);

static inline int minheap2_add_try_before (heap_t *level2_heap, 
		ofs_t loc, heap_t *level1_heap, elem_t elem, key_t elem_key);
static inline int minheap2_add_try_after (heap_t *level2_heap, 
		ofs_t loc, heap_t *level1_heap, elem_t elem, key_t elem_key);
static inline int minheap2_add_try_harder (heap_t *level2_heap, 
		ofs_t *first, ofs_t *last, ofs_t loc, heap_t *level1_heap,
		elem_t *elem, key_t *elem_key);

int
minheap2_add (heap_t *level2_heap, elem_t elem)
{
	int rc;
	ofs_t first, last;
	key_t elem_key = elem_get_key (elem);

	/* if there are no elements in the heap then we have a special case */
	if (heap_is_empty (level2_heap)) {
pd (2, "adding first level1 heap\n");
		rc = minheap2_add_first (level2_heap, elem, elem_key);
		return rc;
	}

	/* locate level1 heap for insertion */
	first = 0;
	last = level2_heap->cnt;
pd (2, "searching [%u,%u] for level1 fit\n", first, last);
	while (first <= last) {
		ofs_t mid = (first + last) / 2;
		heap_t *level1_heap = (void*)level2_heap->data[mid];

pd (2, "{%u,%u} testing %u level1 {cnt=%u/%u [%u,%u]} for %u\n", 
		first, last, mid,
		level1_heap->cnt, level1_heap->size,
		level1_heap->min, level1_heap->max,
		elem_key);

		/* figure out where we fit in respect to the level1_heap */
		if (elem_key < level1_heap->min) {
			/* elem fits before the level1_heap */

pd (2, "  adding before #%u level1 heap\n", mid);
			rc = minheap2_add_try_before (level2_heap, 
					mid, level1_heap,
					elem, elem_key);
pd (2, "  -> %d\n", rc);
			/* if it was able to add before */
			if (!rc) {
				goto success_update_min_max;

			} else if (rc != -EAGAIN) {
				return rc;
			}

			/* proceed with search */
			last = mid - 1;

		} else if (elem_key > level1_heap->max) {
			/* elem fits after the level1_heap */

pd (2, "  adding after #%u level1 heap\n", mid);
			rc = minheap2_add_try_after (level2_heap, 
					mid, level1_heap,
					elem, elem_key);
pd (2, "  -> %d\n", rc);
			/* if it was able to add after */
			if (!rc) {
				goto success_update_min_max;

			} else if (rc != -EAGAIN) {
				return rc;
			}

			/* proceed with search */
			first = mid + 1;

		} else if (! heap_is_full (level1_heap)) {
			/* elem fits in level1_heap and there is room */

pd (2, "  adding into #%u level1 heap\n", mid);
			rc = minheap_add (level1_heap, elem);
pd (2, "  -> %d\n", rc);
			if (!rc)
				goto success_update_min_max;

			/* failed to add */
			return rc;

		} else {
pd (2, "  would fit into %u level1 heap, there is no room, will work harder\n",mid);
			rc = minheap2_add_try_harder (level2_heap, 
					&first, &last, mid, level1_heap,
					&elem, &elem_key);
pd (2, "  -> %d\n", rc);
			if (!rc) {
				goto success_update_min_max;

			} else if (rc != -EAGAIN) {
				return rc;
			}
		}
	}

	BUG("not sure what to do here");

	return -1;

success_update_min_max:
	/* update min/max */
	if (level2_heap->min > elem_key)
		level2_heap->min = elem_key;
	if (level2_heap->max < elem_key)
		level2_heap->max = elem_key;
	/* succes */
	return 0;
}

int
minheap2_get_first (heap_t *level2_heap, elem_t *elem)
{
	int rc;
	heap_t *level1_heap;

	if (heap_is_empty (level2_heap))
		return -ENOENT;

	/* we always remove from first level 1 heap */
	level1_heap = (void*)level2_heap->data[0];

	rc = minheap_get_first (level1_heap, elem);
	if (rc)
		return rc;

	/* did we depleat it? */
	if (heap_is_empty (level1_heap))
		minheap2_delete_level1_heap (level2_heap, 0);

	/* done */
	return 0;
}


void 
__heap2_dump (const char *prefix, heap_t *level2_heap)
{
	ofs_t loc;
	char sub_prefix[128];

	printf ("%scnt=%u/%u  min=%u  max=%u\n", prefix,
			level2_heap->cnt, level2_heap->size,
			level2_heap->min, level2_heap->max);

	if (heap_is_empty (level2_heap)) {
		printf ("%ssub = { }\n", prefix);
		return;
	}

	snprintf (sub_prefix, 128, "%s    ", prefix);

	for (loc=0; loc<level2_heap->cnt; loc++) {
		heap_t *level1_heap;

		level1_heap = (void*)level2_heap->data[loc];

		printf ("%ssub[%u] =\n", prefix, loc);
		__heap_dump (sub_prefix, level1_heap);
	}
}


/* ======================================================================== */

static inline heap_t* 
minheap2_create_level1_heap (heap_t *level2_heap, uint index)
{
	heap_t *new_heap;

	if (index >= level2_heap->size) {
		BUG ("index (%u) past heap size (%u)", index, 
				level2_heap->size);
		BP ();
		return NULL;
	}

	if (heap_is_full (level2_heap)) {
		BUG ("level2 heap is full");
		return NULL;
	}

	new_heap = (void*)malloc (sizeof (heap_t));
	if (NULL == new_heap)
		return NULL;

	heap_init (new_heap);

	if (index < level2_heap->cnt) {
		/* shift up */
		uint no_to_move = level2_heap->cnt - index;
		memmove (level2_heap->data + index + 1,
			 level2_heap->data + index,
			 no_to_move * sizeof (elem_t));
	}

	/* store new entry at index */
	level2_heap->data[index] = (elem_t)new_heap;
	level2_heap->cnt++;

	return new_heap;
}

static inline void 
minheap2_delete_level1_heap (heap_t *level2_heap, uint index)
{
	heap_t *dropped_heap;

	if (index >= level2_heap->size) {
		BUG ("index (%u) past heap size (%u)", index, 
				level2_heap->size);
		BP ();
		return;
	}

	if (index >= level2_heap->cnt) {
		BUG ("cannot remove an index (%u) smaller then count (%u)",
				index, level2_heap->cnt);
		return;
	}

	dropped_heap = (void*)level2_heap->data[index];

	if (index < level2_heap->cnt) {
		/* shift down */
		uint no_to_move = level2_heap->cnt - index;
		memmove (level2_heap->data + index,
			level2_heap->data + index + 1,
			no_to_move * sizeof (elem_t));
	}

	/* update count */
	level2_heap->cnt--;

	/* delete */
	free (dropped_heap);
}



/* ======================================================================== */

static inline int minheap2_add_first (heap_t *level2_heap, elem_t elem, 
		key_t elem_key)
{
	int rc;
	heap_t *level1_heap;

	/* allocate a new level 1 heap */
	rc = -ENOMEM;
	level1_heap = minheap2_create_level1_heap (level2_heap, 0);
	if (NULL == level1_heap)
		goto error_create;

	/* add element into level 1 heap */
	rc = minheap_add (level1_heap, elem);
	if (rc)
		goto error_add_level1;

	/* add level 1 heap into level 2 heap */
	level2_heap->min = elem_key;
	level2_heap->max = elem_key;

	return 0;

error_add_level1:
	minheap2_delete_level1_heap (level2_heap, 0);
error_create:
	return rc;
}

/** 
 * \brief attempts to insert elem before existing level 1 heap
 * @param level2_heap 	level 2 heap
 * @param loc 		offset of level 1 heap
 * @param level1_heap 	level 1 heap
 * @param elem 		element 
 * @param elem_key 	element key
 * @return 0 on success, -EAGAIN if not possible, other error on failure
 */
static inline int 
minheap2_add_try_before (heap_t *level2_heap, ofs_t loc, heap_t *level1_heap,
					elem_t elem, key_t elem_key)
{
	int rc, insert_before;
	heap_t *prev_level1_heap = NULL;

	/* if we are the first level1 heap our work is easier */
	if (loc == 0) {
		/* level1_heap is the first heap, we will try to insert */
		insert_before = 1;
pd (2, "    first, will create new one\n", loc);

	} else {
		/* level1_heap is not first, see if elem can be placed in the
		 * heap that preceeds level1_heap */
		prev_level1_heap = (void*)level2_heap->data[loc-1];

pd (2, "    not first tring to use %u level1 {cnt=%u/%u [%u,%u]} for %u\n",
		loc-1,
		prev_level1_heap->cnt, prev_level1_heap->size,
		prev_level1_heap->min, prev_level1_heap->max,
		elem_key);

		/* if it does not fit between the two level1 heaps then bail */
		if (elem_key < prev_level1_heap->max)
			return -EAGAIN;

		/* we can try to insert it */
		if (! heap_is_full (prev_level1_heap)) {
			rc = minheap_add (prev_level1_heap, elem);
			return rc;
		}

		/* if we fit but the previous is full then insert */
pd (2, "    prev was full too, have to create a new one in between\n");
		insert_before = 1;
	}

	/* we try to insert in level1_heap first, is it full? */
	if (heap_is_full (level1_heap)) {
		/* create a replacement */
		level1_heap = minheap2_create_level1_heap (
				level2_heap, loc);
		if (NULL == level1_heap)
			return -ENOMEM;
	}

	/* add the element */
	rc = minheap_add (level1_heap, elem);

	/* failure? did we create this level1 heap? */
	if (heap_is_empty (level1_heap))
		minheap2_delete_level1_heap (level2_heap, 0);

	/* done */
	return rc;
}

static inline int 
minheap2_add_try_after (heap_t *level2_heap, ofs_t loc, heap_t *level1_heap,
					elem_t elem, key_t elem_key)
{
	int rc, insert_after;
	heap_t *next_level1_heap = NULL;

	/* if we are the last level1 heap our work is easier */
	if ((loc+1) == level2_heap->cnt) {
		/* level1_heap is the last heap, we will try to insert */
		insert_after = 1;

	} else {
		/* level1_heap is not last, see if elem can be placed in the
		 * heap that follows level1_heap */
		next_level1_heap = (void*)level2_heap->data[loc+1];

		/* if it does not fit between the two level1 heaps then bail */
		if (elem_key > next_level1_heap->min)
			return -EAGAIN;

		/* we can try to insert it */
		if (! heap_is_full (next_level1_heap)) {
			rc = minheap_add (next_level1_heap, elem);
			return rc;
		}

		/* if we fit but the next is full then insert */
		insert_after = 1;
	}

	/* we try to insert in level1_heap first, is it full? */
	if (heap_is_full (level1_heap)) {
		/* create a replacement */
		level1_heap = minheap2_create_level1_heap (
				level2_heap, loc+1);
		if (NULL == level1_heap)
			return -ENOMEM;
	}

	/* add the element */
	rc = minheap_add (level1_heap, elem);

	/* failure? did we create this level1 heap? */
	if (heap_is_empty (level1_heap))
		minheap2_delete_level1_heap (level2_heap, 0);

	/* done */
	return rc;
}

static inline int 
minheap2_add_try_harder (heap_t *level2_heap, ofs_t *first, ofs_t *last, 
		ofs_t loc, heap_t *level1_heap, elem_t *elem, key_t *elem_key)
{
	int rc, insert_before = 0;

	if (*elem_key < level1_heap->min) {

		if (loc > 0) {
			heap_t *prev_level1_heap;

			prev_level1_heap = (void*)level2_heap->data[loc-1];

			if (*elem_key > prev_level1_heap->max)
				insert_before = 1;
		}
	}

	if (insert_before) {

pd (2, "    will try to add before\n", loc);
		rc = minheap2_add_try_before (level2_heap, 
				loc, level1_heap,
				*elem, *elem_key);

		/* proceed with search */
		*last = loc - 1;

	} else {
		/* elem fits in level1_heap but there was no room */
		elem_t tmp;

pd (2, "     have to shift an element\n", loc);
		/* remove head element temporeraly */
		rc = minheap_get_first (level1_heap, &tmp);
		if (rc) {
			BUG("failure of removing elem from level1 "
					"heap which is full");
			BP();
			return rc;
		}

		/* replace the empty spot with our original element */
		rc = minheap_add (level1_heap, *elem);
		if (rc) {
			BUG("failure of injecting elem into level1 "
					"heap which is not full");
			BP();
			return rc;
		}

		/* continue search with this element */
		*elem = tmp;
		*elem_key = elem_get_key (tmp);

		/* make sure that we have some room before this node */
		if (*first) 
			(*first)--;

		rc = -EAGAIN;
	}

	return rc;
}






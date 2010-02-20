#include <stdint.h>

#ifndef __HEAP_H__
#define __HEAP_H__

#define HEAP_DEPTH 4
#define HEAP_SIZE  ((1<<HEAP_DEPTH) - 1)

typedef unsigned int elem_t;
typedef unsigned int ofs_t;
#define OFS_INVAL ((ofs_t)-1)

static inline key_t
elem_get_key (elem_t elem)
{
	return (key_t)elem;
}

typedef struct heap_s {
	ofs_t	size;		/** size of data[] array; must be (2^x-1) */
	ofs_t 	cnt;		/** number of elements in a heap */
	key_t	min;		/** key of minimum element */
	key_t	max;		/** key of maximum element */
	elem_t	data[HEAP_SIZE];
} heap_t;

void heap_init (heap_t *);
void __heap_dump (const char* prefix, heap_t *);
static inline void heap_dump (heap_t *heap) { __heap_dump ("", heap); }

ofs_t heap_parent_of (heap_t *, ofs_t);

ofs_t heap_left (heap_t *, ofs_t);
ofs_t heap_right (heap_t *, ofs_t);

int heap_is_empty (heap_t *);
int heap_is_full (heap_t *);

int minheap_add (heap_t *, elem_t);
int minheap_get_first (heap_t *, elem_t *);

int minheap2_add (heap_t *, elem_t);
int minheap2_get_first (heap_t *, elem_t *);

void __heap2_dump (const char *, heap_t *);
static inline void heap2_dump (heap_t *heap) { __heap2_dump ("", heap); }

/* internal primitives */
void heap_swap (heap_t *, ofs_t, ofs_t);

extern int debug;

#define BUG(s,a...) fprintf (stderr, "BUG %s:%d  " s "\n", \
		__FUNCTION__ , __LINE__ , ## a)

#define BP() __asm__ __volatile__("ud2\n")
		
#define pd(l,f...) ({ if (l & debug) fprintf (stderr, "..." f); })


#endif /* __HEAP_H__ */

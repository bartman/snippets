#include <stdio.h>

typedef struct foo_s {
	int a;
	int b;
} foo_t;

int 
main (void)
{
	foo_t array[10];

	printf ("sizeof(foo_t) = %u\n", sizeof (foo_t));
	printf ("sizeof(array) = %u\n", sizeof (array));

	return 0;
}

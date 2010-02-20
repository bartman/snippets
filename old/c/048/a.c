#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

// from b.S
extern int foo(int);

int
main (void)
{
	int i, j;
	for (i=0; i<10; i++)
		j = foo(j);
	printf ("%d\n", j);
}

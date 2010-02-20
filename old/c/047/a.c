#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

int
main (void)
{
	int i, j;
	for (i=0; i<1; i++)
		j ++;
	printf ("%d\n", j);
}

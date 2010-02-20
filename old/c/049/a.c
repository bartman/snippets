#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <ctype.h>

int
main (void)
{
	int i;
	printf ("\"");
	for (i=0; i<255; i++) {
		if (isalnum(i))
			printf ("%c", i);
	}
	printf ("\"\n");
}

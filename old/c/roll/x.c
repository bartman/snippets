#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int
main (void)
{
	int a, b, c;
	char buf[200];
	const char *name[] =
	{ "yellow", "blue", "green", "hoard", "hoard", "hoard" };

	srand (time(NULL));

	while (1) {
		int i;

		fgets(buf, 200, stdin);

		a = (6*((float)rand())/((float)RAND_MAX));
		b = (6*((float)rand())/((float)RAND_MAX));
		c = (6*((float)rand())/((float)RAND_MAX));

		for (i=0; i< 100; i++) puts("");
		printf ("[%d] [%d %s]\n", a+1, b+1, name[c]);

	}

	return 0;
}

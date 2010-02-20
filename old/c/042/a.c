#include <stdio.h>
#include <stdint.h>

void *foo (void *ptr)
{
	intptr_t tmp;
	tmp = (intptr_t)ptr;
	return (void*)tmp;
}

int
main (void)
{
	foo (NULL);

        return 0;
}

#include <stdio.h>

int 
main (void)
{
	int a[4] = {0,1,2,3};
	int b[4] = {0,1,2,3};
	int c[4] = {0,1,2,3};

	asm (	
		"movdqa (%0), %%xmm0\n"
		"movdqa (%1), %%xmm1\n"
		"paddq %%xmm0, %%xmm1\n"
		"movdqa %%xmm1, (%2)\n"
		: 
		: "r" (a), "r" (b), "r" (c)
		: "memory");
}

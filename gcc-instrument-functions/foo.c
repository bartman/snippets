#include <stdio.h>
#include <stdlib.h>

__attribute__((no_instrument_function))
void __cyg_profile_func_enter (void *this_fn, void *call_site)
{
	printf(">>\n");
}
__attribute__((no_instrument_function))
void __cyg_profile_func_exit  (void *this_fn, void *call_site)
{
	printf("<<\n");
}

void bar(void)
{
	printf("bar\n");
}

void foo(void)
{
	printf("foo\n");
}

int main(void)
{
	foo();
	bar();
	return 0;
}

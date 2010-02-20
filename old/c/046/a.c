#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <assert.h>


int f1a()
{
	return 1;
}

int f1()
{
	return f1a();
}

int f2a()
{
	return 1;
}

int f2()
{
	return f2a();
}

int main()
{
	int a = f1();
	int b = f2();
	return(a+b);
}


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// if y==1, n=x
// if y==2, n=x*x
// if y==3, n=x*x*x

unsigned int f(unsigned int x, unsigned int y) {
	unsigned int n = 1;
	while (y > 0) {
		// n * (x ^ y) is invariant
		if (y % 2 == 0) {
			y /= 2;
			x = x * x;
		} else {
			y--;
			n = n * x;
		}
	}
	return n;
}

unsigned int g(unsigned int x, unsigned int y)
{
	unsigned int n = 1;
	for (unsigned int m=1; m<=y; m<<=1) {
		if (y&m)
			n *= x;
		x *= x;
	}
	return n;
}

#define NSEC_PER_SEC 1000000000ull
static inline double double_time(void)
{
	struct timespec ts;
	double sec;

	clock_gettime(CLOCK_MONOTONIC, &ts);

	sec = (double)ts.tv_sec
		+ (double)ts.tv_nsec / NSEC_PER_SEC;

	return sec;
}

#define LOOPS 100
#define MAXX 1000000
#define MAXY 32
int main(int main, char *argv[])
{
	double t0, t1, t2;
	double tf=0, tg=0;

	for (int loop=0; loop<LOOPS; loop++) {

		t0 = double_time();

		for (int x = 1; x<MAXX; x++) {
			for (int y=0; y<MAXY; y++) {
				int a = f(x, y);
				(void)a;
			}
		}

		t1 = double_time();

		for (int x = 1; x<MAXX; x++) {
			for (int y=0; y<MAXY; y++) {
				int a = g(x, y);
				(void)a;
			}
		}

		t2 = double_time();

		tf += t1-t0;
		tg += t2-t1;

		printf("loop=%u f=%.6f g=%.6f\r", loop, tf/(loop+1), tg/(loop+1));
		fflush(stdout);
	}

	printf("\n");

	return 0;
}

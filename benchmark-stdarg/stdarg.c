#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

static int sum_v(int *one, ...) __attribute__((noinline));
static void run_vararg(long long loops, int *) __attribute((noinline));

static int sum_v(int *one, ...)
{
	va_list ap;
	int *n, res = *one;
	va_start(ap,one);
	while((n = va_arg(ap, int *)))
		res += *n;
	va_end(ap);
	return res;
}
static void run_vararg(long long loops, int *v)
{
	long long i;
	for (i=0; i<loops; i++)
		*v = sum_v(v, v+1, v+2, v+3, v+4, v+5, v+6, v+7, v+8, v+9, NULL);
}

static int sum_10(int *a, int *b, int *c, int *d, int *e, int *f, int *g,
		int *h, int *i, int *j) __attribute__((noinline));
static void run_fixarg(long long loops, int *) __attribute((noinline));

static int sum_10(int *a, int *b, int *c, int *d, int *e, int *f, int *g,
		int *h, int *i, int *j)
{
	int res = 0;
	if (a) res += *a;
	if (b) res += *b;
	if (c) res += *c;
	if (d) res += *d;
	if (e) res += *e;
	if (f) res += *f;
	if (g) res += *g;
	if (h) res += *h;
	if (i) res += *i;
	if (j) res += *j;
	return res;
}
static void run_fixarg(long long loops, int *v)
{
	long long i;
	for (i=0; i<loops; i++)
		*v = sum_10(v, v+1, v+2, v+3, v+4, v+5, v+6, v+7, v+8, v+9);
}

static void help(int err)
{
	FILE *out = err ? stderr : stdout;
	fprintf(out, "stdarg [ -v | -a ]  [ -l <loops> ]\n");
	exit(err);
}

#define TIME(w,x) ({ \
		time_t s = time(NULL), e; \
		x; \
		e = time(NULL); \
		printf(w ": %lu\n", e-s); \
		})

int main(int argc, char * argv[])
{
	int argi;
	long long loops = 1000000000;
	int do_vararg = 0;
	int do_fixarg = 0;
	int vals[10] = { 0,1,2,3,4,5,6,7,8,9 };

	for (argi = 1; argi < argc; argi++) {
		const char *word = argv[argi];
		if (word[0] != '-' || !word[1] || word[2])
			help(1);
		switch (word[1]) {
		case 'l':
			word = argv[++argi];
			loops = atoll(word);
			break;
		case 'v':
			do_vararg = 1;
			break;
		case 'a':
			do_fixarg = 1;
			break;
		default:
			help(1);
		}
	}

	if (do_vararg)
		TIME("vararg", run_vararg(loops,vals));
	if (do_fixarg)
		TIME("fixed", run_fixarg(loops,vals));

	return 0;
}

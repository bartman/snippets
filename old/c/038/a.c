#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <limits.h>
#include <asm/msr.h>

int
main (void)
{
        volatile int a = 999;
        volatile int max = 1000;
        uint64_t base, before, after, diff;

        rdtscll(base);
        rdtscll(before);

        a = (a+1) % max;

        rdtscll(after);

        diff = (after-before) - (before-base);

        printf ("a = (a+1) %% max;         // %lu ticks\n", diff);

        return 0;
}

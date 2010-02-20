#include <stdio.h>
  
typedef unsigned int u32;
typedef unsigned long long u64;

u32 cpu_khz = 1000; // 1MHz

static void
x( u64 val )
{
    u32 top, v;
    int ex;
    u64 a, b;
    val /= cpu_khz/1000;
    // now we have val == us (10^-6 seconds)
    a = 1000, b = 1;
    ex = -3;
    while (a < val) {
        a *= 10;
        b *= 10;
        ex++;
    }
    top = (u32) (val / b);
    printf("%de%d", (int) top, ex);
}

int 
main(void)
{
  x(1000000LL);
}


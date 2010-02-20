#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <limits.h>

#define myUINT_MAX        (~0U)

static void
foo (int a, int b)
{
        printf (" --- foo a=%d b=%d ---\n", a, b);
        if (a)
                if (b)
                        printf ("a b\n");
        else
                if (b)
                        printf ("!a b\n");
}


static void
bar (int a, int b)
{
        printf (" --- bar a=%d b=%d ---\n", a, b);
        if (a) {
                if (b)
                        printf ("a b\n");
        } else {
                if (b)
                        printf ("!a b\n");
        }
}


int
main (void)
{
        int a,b;

        for (a=0; a<2; a++)
                for (b=0; b<2; b++)
                        foo (a, b);

        printf ("\n");

        for (a=0; a<2; a++)
                for (b=0; b<2; b++)
                        bar (a, b);

        return 0;
}

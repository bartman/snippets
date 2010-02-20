#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

enum foo_e {
        ONE,
        TWO,
        THREE,
};

static inline void
foo(enum foo_e x)
{
        printf ("x=%d\n", x);
}

int
main (void)
{
        enum foo_e x;

        for (x=ONE; x<THREE; x++) {
                foo(x);
        }

        return 0;
}

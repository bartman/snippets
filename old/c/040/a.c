#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <limits.h>

#define myUINT_MAX        (~0U)

int
main (void)
{
        printf ("          - 12345678 -\n");
        printf ("   UINT_MAX %08x\n", UINT_MAX);
        printf ("my UINT_MAX %08x\n", myUINT_MAX);

        return 0;
}

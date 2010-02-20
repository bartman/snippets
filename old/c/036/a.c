#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct foo_s {
        int array[100];
} foo_t;

int
main (void)
{
        foo_t on_stack, *f = &on_stack;

        printf ("f = %p\n", f);
        printf ("  sizeof (f->array) = %u\n", sizeof (f->array));
        printf ("   f->array = %p\n", f->array);
        printf ("  &f->array = %p\n", &f->array);
}

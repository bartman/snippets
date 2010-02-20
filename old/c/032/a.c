#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct {
        int len;
        char *string;
} foo_t;

void int_cleanup (foo_t *p)
{
        printf ("cleanup...\n");
}

int
main (void)
{
        foo_t __attribute__ ((cleanup(int_cleanup))) foo;

        printf ("exiting...\n");

        return 0;
}

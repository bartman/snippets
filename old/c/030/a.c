#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

void * 
fn(void *arg)
{
        int *fd = (int*)arg;
        int max, loops=10;
        fd_set rfds, xfds;

        printf ("thread start\n");

        max = *fd + 1;

        FD_ZERO (&rfds);
        FD_ZERO (&xfds);

        while (loops--) {
                int rc;

                FD_SET (*fd, &rfds);
                FD_SET (*fd, &xfds);

                printf ("select...\n");

                rc = select (max, &rfds, NULL, &xfds, NULL);

                printf ("...select %d\n", rc);

                if (rc < 0) { perror ("select"); exit (-1); }

                if (FD_ISSET (*fd, &xfds)) {
                        printf ("exception event!\n");
                        break;
                }

                if (FD_ISSET (*fd, &rfds)) {
                        printf ("read event!\n");
                        break;
                }

        }

        close (*fd);

        printf ("thread end\n");

        return NULL;
}

int
main (void)
{
        pthread_t th;
        int rc, fd[2];

        rc = pipe (fd);
        if (rc) { perror ("pipe"); exit (-1); }

        rc = pthread_create (&th, NULL, fn, fd+1);
        if (rc) { perror ("pthread_create"); exit (-1); }

        printf ("...waiting\n");
        sleep (1);
        printf ("...waiting\n");
        sleep (1);
        printf ("terminating\n");

        close (fd[0]);

        printf ("joining\n");

        pthread_join (th, NULL);

        printf ("exit\n");

        return 0;
}

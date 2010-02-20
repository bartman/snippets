#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>

#define die(f,a...) ({ \
        fprintf (stderr, "%s\n" f, (errno)?strerror(errno):"", ##a); \
        exit (-1); })

int main() 
{
        int rc, ls=0, as, opt;
        struct sockaddr_in sa;

        ls = socket (AF_INET, SOCK_STREAM, 0);
        if (ls < 0) die ("socket() failed");

        sa.sin_family      = AF_INET;
        sa.sin_port        = htons (1024);
        sa.sin_addr.s_addr = INADDR_ANY;

        opt = 1;
        rc = setsockopt (ls, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof (opt));
        if (rc < 0) die ("setsockopt(SOL_SOCKET, SO_REUSEADDR) failed");

        rc = bind (ls, (struct sockaddr*)&sa, sizeof (sa));
        if (rc < 0) die ("bind() failed");

        rc = listen (ls, 1);
        if (rc < 0) die ("listen() failed");

        printf ("sleeping...\n");
        getchar();

        while (1) {

                as = accept (ls, NULL, NULL);
                if (as < 0) die ("accept() failed");

                printf ("accepted %d\n", as);
        }


        return rc;
}

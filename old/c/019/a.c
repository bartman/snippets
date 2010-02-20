#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>

extern char **environ;

pid_t parent;

static void
msg (const char *fmt, ...)
{
        va_list ap;
        FILE *f;

        f = fopen ("/dev/stdout", "w");
        if (!f) return;

        va_start (ap, fmt);
        if (getpid() != parent)
                fprintf (f, "  ");
        fprintf (f, "%u:", getpid());
        vfprintf (f, fmt, ap);
        va_end (ap);

        fclose(f);
}

static int
run_command (char *cmd)
{
        pid_t pid;
        int fd[2], rc;

        pipe(fd);
        msg ("pipe() = (%d,%d)\n", fd[0], fd[1]);

        pid = fork();
        msg ("fork() = %d\n", pid);
        if (pid < 0) {
                perror ("fork");
                return pid;

        } else if (!pid) {
                int null;
                char *argv[2];

                null = open ("/dev/null", O_RDONLY);
                msg ("open (/dev/null) = %u\n", null);

                /* child */
                msg ("close (%d)\n", fd[0]);
                close (fd[0]);          // close reading end

                msg ("dup2 (%u, 0)\n", null);
                dup2 (null, 0);

                msg ("dup2 (%u, 1)\n", fd[1]);
                dup2 (fd[1], 1);

                msg ("dup2 (%u, 2)\n", fd[1]);
                dup2 (fd[1], 2);


                argv[0] = cmd;
                argv[1] = NULL;

                msg ("execve (%s, 2)\n", argv[0]);
                rc = execve (argv[0], argv, environ);

                exit(-1);
        }

        /* parent */
        close (fd[1]);          // close writing end

        waitpid (pid, &rc, 0);

        msg ("waitpid(%d) = %u (%s,%d)\n", pid, rc,
                        WIFEXITED(rc)?"OK":"FAIL",
                        WEXITSTATUS(rc));
        
        return WEXITSTATUS(rc);
        
}

int
main (void)
{
        parent = getpid();

        run_command ("./a");

        return 0;
}

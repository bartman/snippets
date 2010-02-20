#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <linux/types.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

typedef __attribute__((packed)) struct {
        uint16_t a;
        uint16_t b;
        uint16_t c;
} foo_t;

int
main (void)
{
        int fd, rc;
        const char *str = "12345";
        uint32_t len = strlen (str);
        uint32_t magic = 0x12345678;
        foo_t foo = {
                .a = 1,
                .b = 2,
                .c = 3,
        };

        fd = open ("foo", O_RDWR | O_CREAT | O_TRUNC, 0640);
        if (fd<0) {
                perror ("open");
                exit(1);
        }

        rc = write (fd, &len, sizeof (len));
        if (rc<0) {
                perror ("write(len)");
                exit(1);
        }

        rc = write (fd, str, len);
        if (rc<0) {
                perror ("write(str)");
                exit(1);
        }

        rc = write (fd, &foo, sizeof(foo));
        if (rc<0) {
                perror ("write(foo)");
                exit(1);
        }

        rc = write (fd, &magic, sizeof (magic));
        if (rc<0) {
                perror ("write(magic)");
                exit(1);
        }

        close (fd);

        return 0;
}

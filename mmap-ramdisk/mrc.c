#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>


void die(const char *fmt, ...)
{
	va_list ap;
	fprintf(stderr, "ERROR: ");
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	exit(1);
}

int main(void)
{
	uint32_t *data;
	int fd;
	
	fd = open("/dev/mr", O_RDWR);
	if (fd < 1) die("open(/dev/mr): %s\n", strerror(errno));

	data = mmap(0, 0x00300000, PROT_READ | PROT_WRITE, MAP_SHARED, fd,
			0xc0000000);
	if (fd < 1) die("open(/dev/mr): %s\n", strerror(errno));

	printf("map[0] = 0x%08x", data[0]);

	close(fd);
	munmap(data, 0x00300000);
	return 0;
}

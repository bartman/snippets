#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <assert.h>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

int main(void)
{
	int sock;
	unsigned val, prev;
	socklen_t val_len;
	int rc;

	sock = socket(PF_INET, SOCK_STREAM, 0);
	assert(sock>0);

	val_len = sizeof(val);
	rc = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, &prev, &val_len);
	assert(rc==0);

	printf ("=read %u (%u)\n", prev, val_len);

	do {
		val = prev * 2;

		printf ("write %u\n", val);

		rc = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, &val, sizeof(val));
		assert(rc==0);

		val_len = sizeof(prev);
		rc = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, &prev, &val_len);
		assert(rc==0);

		printf ("`read %u (%u)\n", prev, val_len);

	} while(prev==val);

	return 0;
}

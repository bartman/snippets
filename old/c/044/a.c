#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
 
int dummy = 0;
 
void handler(int sig)
{
	printf("%d\r\n",dummy++);/* for testing*/
	dummy++;
	alarm(1);
}
int main(void)
{
	struct sigaction act, oact;

	printf("main %d\r\n",dummy++);/* for testing*/
	act.sa_handler = handler;
	act.sa_flags = SA_RESTART;
	sigaction(SIGALRM,&act,&oact);
	alarm(1);
	while(1) {
		printf(" waiting...\n");
		sleep(10);
	}
}

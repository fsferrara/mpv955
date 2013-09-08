#include <stdio.h>
#include <sys/file.h>
#include <sys/types.h>
#include <fcntl.h>
#include "mpv955ioctl.h"


#define DEVICE "/dev/mpv955"


int main(int argc, char *argv[]) {

	int fd = 0;
	int chan = 0;
	int count = 0;
	int i,j;
	char scelta = 'n';
	unsigned short buff;
	unsigned short ciclo = 0;

	fd = open(DEVICE, O_WRONLY);
	printf("il file descriptor e' %d\n", fd);

	do {
		printf("insert channel [0-7 ; 8=all_channel]: ");
		scanf("%d", &chan);
		getchar();
	} while ( (chan < 0) || (chan > 8) );

	do {
		printf("number of iteration: ");
		scanf("%d", &count);
		getchar();
	} while (count < 0);

	for (i=0 ; i<count ; i++) {

		if (chan == 8) {

			buff = rand();
			for (j=0 ; j<8 ; j++) {
				if(write(fd, &buff, j) <= -1) {
					printf("Errore nella scrittura\n");
				}
				else {
					printf("chan %d <- %X\n", chan, buff);
				}
			}

		}
		else {

			buff = rand();
			if(write(fd, &buff, chan) <= -1) {
				printf("Errore nella scrittura\n");
			}
			else {
				printf("chan %d <- %X\n", chan, buff);
			}

		}

		sleep(1);

	}


	printf("close() ha restituito %d\n", close(fd));

	exit(0);

}


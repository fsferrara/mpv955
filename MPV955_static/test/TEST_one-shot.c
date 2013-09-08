#include <stdio.h>
#include <sys/file.h>
#include <sys/types.h>
#include <fcntl.h>
#include "mpv955ioctl.h"


#define DEVICE "/dev/mpv955"


int main(int argc, char *argv[]) {

	int fd = 0;
	int chan = 0;
	int scrivi = 1;
	char scelta = 'n';
	unsigned short buff;
	unsigned short ciclo = 0;

	fd = open(DEVICE, O_WRONLY);
	printf("il file descriptor e' %d\n", fd);

	if (ioctl(fd, STOP, NULL) != 0)
		perror("ioctl()");

	if (ioctl(fd, SET_ONESHOT, NULL) != 0)
		perror("ioctl()");

	while (scrivi) {

		do {
			printf("insert channel [0-7]: ");
			scanf("%d", &chan);
			getchar();
		} while ( (chan < 0) || (chan > 7) );

		buff = rand();
		if(write(fd, &buff, chan) <= -1) {
			printf("Errore nella scrittura\n");
		}
		else {
			printf("channel %d <- %X\n", chan, buff);
		}


		printf("write (y/n) ? ");
		scanf("%c", &scelta);
		if (scelta == 'y') {
			/*
			Diamo due volte il comando START perche'
			ogni canale del dispositivo e' doppiamente
			bufferizzato.
			*/
			if (ioctl(fd, START, NULL) != 0)
				perror("ioctl()");

			while (ciclo == 0) {
				if (ioctl(fd, GET_CYCFIN, (void *) &ciclo) != 0)
					perror("ioctl");
			}
			ciclo = 0;

			if (ioctl(fd, START, NULL) != 0)
				perror("ioctl()");
		}
		getchar();

		printf("exit (y/n) ? ");
		scanf("%c", &scelta);
		if (scelta == 'y') {
			scrivi = 0;
		}
		getchar();

	}

	printf("close() ha restituito %d\n", close(fd));

	exit(0);

}


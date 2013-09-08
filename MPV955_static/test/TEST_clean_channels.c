#include <stdio.h>
#include <sys/file.h>
#include <sys/types.h>
#include <fcntl.h>
#include "mpv955ioctl.h"


#define DEVICE "/dev/mpv955"


int main(int argc, char *argv[]) {

	int fd = 0;
	int chan = 0;
	unsigned short buff;
	int ciclo = 0;


	fd = open(DEVICE, O_WRONLY);
	printf("il file descriptor e' %d\n", fd);

	if (ioctl(fd, STOP, NULL) != 0)
		perror("ioctl()");

	if (ioctl(fd, SET_ONESHOT, NULL) != 0)
		perror("ioctl()");

	if (ioctl(fd, GET_ZERO_VOLTS, (void *) &buff) != 0)
		perror("ioctl()");



	do {
		printf("insert channel [0-7 ; 8=all_channel]: ");
		scanf("%d", &chan);
		getchar();
	} while ( (chan < 0) || (chan > 8) );

	if (chan == 8) {

		for (chan=0 ; chan<8 ; chan++) {
			if(write(fd, &buff, chan) <= -1) {
				printf("Errore nella scrittura\n");
			}
			else {
				printf("channel %d <- %X\n", chan, buff);
			}
		}

	}
	else {

		if(write(fd, &buff, chan) <= -1) {
			printf("Errore nella scrittura\n");
		}
		else {
			printf("channel %d <- %X\n", chan, buff);
		}
	}

	/*
	Diamo due volte il comando START perche'
	ogni canale del dispositivo e' doppiamente
	bufferizzato.
	*/
	if (ioctl(fd, START, NULL) != 0)
		perror("ioctl()");

	ciclo = 0;
	while (ciclo == 0) {
		if (ioctl(fd, GET_CYCFIN, (void *) &ciclo) != 0)
			perror("ioctl");
	}
	ciclo = 0;

	if (ioctl(fd, START, NULL) != 0)
		perror("ioctl()");


	printf("close() ha restituito %d\n", close(fd));

	exit(0);

}


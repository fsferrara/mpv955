/**********************************************
* Copyright (C) 2005-2006
* Fioravante De Sapio
*  <desapio82@libero.it>
* Francesco Saverio Ferrara
*  <fs.ferrara@gmail.com>
*
*
* This program is free software; you can
* redistribute it and/or modify it under the
* terms of the GNU General Public License as
* published by the Free Software Foundation;
* either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that
* it will be useful, but WITHOUT ANY WARRANTY;
* without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR
* PURPOSE.  See the GNU General Public
* License for more details.
*
* You should have received a copy of the GNU
* General Public License along with this
* program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place -
* Suite 330, Boston, MA  02111-1307, USA.
*
*
***********************************************
*
*
* File:          mpv955info.c
*
* P.N.:
*
* Product:       Pentland MPV955
*                LynxOS Device Driver
*
* Creation date: 22 october 2005
*
* Description:   It creates and it constructs
*                the device information data
*                structure
*
*
**********************************************/



/*@********************************************
* INCLUDE SECTION
**********************************************/

#include "mpv955info.h"
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>


/*@********************************************
* DEFINE SECTION
**********************************************/

#define FILENAME "mpv955info"


/*@********************************************
* Function:     main
*
* Description:  create a FILENAME
*
* Input:        argc - number of arguments
*               argv - content the arguments
*
* Output:
*
* Return Value: int - exit status
*
**********************************************/

int main(int argc, char *argv[]) {
	
	int fd; // File descriptor
	ssize_t byte; // Numero di byte scritti su FILENAME
	mpv955_info_t info;
	int dr_id, major, nodo;



  	// Inizializzazione della struttura mpv955_info_t 
	info.base_address = 0x00F00000;



	// Apertura del file
	fd = open(FILENAME, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0) {
		perror("open()");
		exit(1);
	}
	
	// Scrittura della struttura info all'interno del file
	byte = write(fd, &info, sizeof(mpv955_info_t));
	if (byte != sizeof(mpv955_info_t)) {
		perror("write()");
		if (close(fd) < 0)
			perror("close()");
		
		exit(1);
	}
	
	// Chiusura del file
	if (close(fd) < 0) {
		perror("close()");
		exit(1);
	}
	
	
	// Uscita dal programma
	exit(0); //Nessun errore
}


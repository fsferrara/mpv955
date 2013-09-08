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
* File:          mpv955drvr.h
*
* P.N.:
*
* Product:       Pentland MPV955 DA Converter
*                LynxOS 4.0.0 Device Driver
*
* Creation date: 22 october 2005
*
* Description:   Header file for the MPV955
*                DAC device driver entry point
*
*
**********************************************/



#ifndef _MPV955DRVR_H_
#define _MPV955DRVR_H_



/*@********************************************
* INCLUDE SECTION
**********************************************/

// Definizione della device information structure mpv955_info
#include "mpv955info.h"
#include "mpv955ioctl.h"

#include <conf.h>
#include <kernel.h>
#include <mem.h>
#include <errno.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <st.h>
#include <cetia.h>
#include <pwb.h>
//#include "/sys/drivers.vmpc/almavme/almavme.h"


/*@********************************************
* DEFINE SECTION
**********************************************/

// Data Memory
#define DATA_WORD 4000

// Control Memory (CM)
#define    MAX_AREA_CM      2
#define    AREA_CM          0
#define    AREA_CM_1        0
#define    AREA_CM_2        1

// I registri occupano 30 byte da REGISTER_OFFSET
#define REGISTERS_OFFSET_A1 0x00008000
#define REGISTERS_OFFSET_A2 0x00008010

// Lo start register �di solo 2 byte
#define START_REGISTER_OFFSET   0x0000C000

// Minima dimensione del canale massima indirizzabile
#define VME_A24_CHANNEL_SIZE 16*1024*1024

// Bit del registro "Control Status"
#define   UNUSED           0xF080
#define   OVER_SAMP        0x0800
#define   CYCFIN           0x0400
#define   TIMEOUT          0x0200
#define   HALT             0x0100
#define   CHAN_S0          0x0010
#define   CHAN_S1          0x0020
#define   CHAN_S2          0x0040
#define   TIMEOUT_ENABLE   0x0008
#define   CONT_ONE         0x0004
#define   NORMAL_EVENT     0x0002
#define   INT_EXT          0x0001

// Configurazione di default
#define   RATE_TIMER       0xFFFB
#define   TIMEOUT_VALUE    0xFFFF

// Define che devono essere decommentate oppure passare
// con l'opzione -D del compilatore gcc
// #define   ZERO_VOLTS       0x7FFF
// #define   DEBUG_MPV955



/*@********************************************
* MACRO SECTION
**********************************************/

#define SET_REG_BIT(board, reg, bit)   { if ((board) != NULL)\
(((board)->reg) |= (bit)); }

#define RESET_REG_BIT(board, reg, bit) {if((board) != NULL)\
(((board)->reg) &= (~(bit))); }

#define WRITE_REG(board, reg, value) {if((board) != NULL)\
(((board)->reg) = (value)); }

#define READ_REG(board, reg) ((board)->reg)

#define READ_REG_BIT(board, reg, bit) (((board)->reg) & (bit))

/*@********************************************
* STATICS DATA STRUCTURE
**********************************************/

typedef struct {

	/*
	* Registri contenuti nell'area
	* chiamata "Control Memory"
	*/
	unsigned short control_status;
	unsigned short start_address_register;
	unsigned short stop_address_register;
	unsigned short interrupt_control_register;
	unsigned short rate_timer_control;
	unsigned short timeout_control;
	unsigned short dac_disable;

} mpv955_registers_t;


typedef struct {

	/*
	* Lo START REGISTER deve contenere il valore
	* di timeout
	*/
	unsigned short timeout;

} mpv955_start_register_t;


typedef struct {

	mpv955_info_t *info;
	int area_cm;
	mpv955_registers_t *registers[MAX_AREA_CM];
	mpv955_start_register_t *start_register;
	unsigned short *data_memory; 
	char *p_a24_phys_addr;
	unsigned short zero_volts;
	unsigned short rate_timer;
	unsigned short timeout_value;
	int primo; // Indice se si e' il primo processo ad aprire il dispositivo

} mpv955_data_t;

/*@********************************************
* GLOBAL DECLARATION SECTION
**********************************************/

/*@********************************************
* EXTERN DECLARATION SECTION
**********************************************/

extern int ionull();

/*@********************************************
* FUNCTION PROTOTYPE
**********************************************/

int mpv955_open(mpv955_data_t *, int, struct file *);
int mpv955_close(mpv955_data_t *, struct file *);
int mpv955_write(mpv955_data_t *, struct file *, char *, int);
int mpv955_ioctl(mpv955_data_t *, struct file *, int, char *);
int mpv955_select(mpv955_data_t *, struct file *, int, struct sel *);
char *mpv955_install(mpv955_info_t *);
int mpv955_uninstall(mpv955_data_t *);



#endif

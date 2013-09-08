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
* File:          mpv955drvr.c
*
* P.N.:
*
* Product:       Pentland MPV955 DA Converter
*                LynxOS 4.0.0 Device Driver
*
* Creation date: 22 october 2005
*
* Description:   Entry point implementation for
*                MPV955 DAC device driver
*
*
**********************************************/



/*@********************************************
* INCLUDE SECTION
**********************************************/

#include "mpv955drvr.h"
#include <dldd.h> // Per l'installazione dinamica del driver



/*@********************************************
* Function:     mpv955_debug_registers
*
* Description:  print board registers state
*
* Input:        *p_regs - pointer to the board
*                   registers
*               *punto - string that contain
*                   debug information
*
* Output:       none
*
* Return Value: none
*
**********************************************/

void mpv955_debug_registers(mpv955_registers_t *pregs, char *punto) {


	ckprintf("######## PUNTO ESECUZIONE %s\n", punto);


	ckprintf("[%X]-> Control status register\n", READ_REG(pregs, control_status));
	ckprintf("[%X]-> Start address register\n", READ_REG(pregs, start_address_register));
	ckprintf("[%X]-> Stop address register\n", READ_REG(pregs, stop_address_register));
	ckprintf("[%X]-> Interrupt control register\n", READ_REG(pregs, interrupt_control_register));
	ckprintf("[%X]-> Rate timer control\n", READ_REG(pregs, rate_timer_control));
	ckprintf("[%X]-> Timeout control\n", READ_REG(pregs, timeout_control));
	ckprintf("[%X]-> DAC disable\n", READ_REG(pregs, dac_disable));
	ckprintf("############################################\n");
}



/*@********************************************
* Function:     mpv955_debug_memory
*
* Description:  print board buffer
*
* Input:        *p_mpv955_data - pointer to
*                   the static structure
*               *punto - string that contain
*                   debug information
*
* Output:       none
*
* Return Value: none
*
**********************************************/

void mpv955_debug_memory(mpv955_data_t *p_mpv955_data, char *punto) {

	int i;
	int count = 8;

	kkprintf("========= STATO MEMORIA %s\n", punto);

	for (i=0 ; i<count ; i++) {
		kkprintf("[%d]->%X\t", i, p_mpv955_data->data_memory[i]);
	}
	kkprintf("\n==========================\n");

}



/*@********************************************
* Function:     mpv955_open
*
* Description:  entry point function
*
* Input:        *p_mpv955_data - pointer to
*                   the static structure
*               devno - contains the major and
*                   the minor number
*               *f - is a pointer to a file
*                   structure
*
* Output:       none
*
* Return Value: int - exit status
*
**********************************************/

int mpv955_open(mpv955_data_t *p_mpv955_data, int devno, struct file *f) {

	mpv955_registers_t *pregs = NULL; // Usato per accorciare i nomi
	int ps;


	// Puntatore usato per accorciare i nomi
	pregs = p_mpv955_data->registers[p_mpv955_data->area_cm];

#ifdef DEBUG_MPV955
	mpv955_debug_registers(pregs, "inizio open");
#endif

	// Configurazione di default
	disable(ps);

	// Questa parte di codice deve essere eseguita solo dal primo
	// thread che apre il dispositivo

	if (p_mpv955_data->primo == 1) {
		p_mpv955_data->primo = 0;
		RESET_REG_BIT(pregs, control_status, INT_EXT);
		RESET_REG_BIT(pregs, control_status, NORMAL_EVENT);
		RESET_REG_BIT(pregs, control_status, CONT_ONE);
		SET_REG_BIT(pregs, control_status, TIMEOUT_ENABLE);
		SET_REG_BIT(pregs, control_status, CHAN_S0);
		SET_REG_BIT(pregs, control_status, CHAN_S1);
		SET_REG_BIT(pregs, control_status, CHAN_S2);
		RESET_REG_BIT(pregs, control_status, UNUSED);
		WRITE_REG(pregs, rate_timer_control, p_mpv955_data->rate_timer);
		WRITE_REG(pregs, interrupt_control_register, 0x0000);
		WRITE_REG(pregs, start_address_register, 0x0000);
		WRITE_REG(pregs, stop_address_register, 0x0007);
		WRITE_REG(pregs, dac_disable, 0x0000);
		WRITE_REG(p_mpv955_data->start_register, timeout, p_mpv955_data->timeout_value);
	}

	enable(ps);

#ifdef DEBUG_MPV955
	mpv955_debug_registers(pregs, "inizio open");
#endif

	/*
	* FINE PROCEDURA OPEN
	*/

#ifdef DEBUG_MPV955
	ckprintf("mpv955 correctly opened\n");
#endif

	return (OK);
}



/*@********************************************
* Function:     mpv955_close
*
* Description:  entry point function
*
* Input:        *p_mpv955_data - pointer to
*                   the static structure
*               *f - is a pointer to a file
*                   structure
*
* Output:       none
*
* Return Value: int - exit status
*
**********************************************/

int mpv955_close(mpv955_data_t *p_mpv955_data, struct file *f) {

	mpv955_registers_t *pregs = NULL; // Usato per accorciare i nomi

	// Puntatore usato per accorciare i nomi
	pregs = p_mpv955_data->registers[p_mpv955_data->area_cm];

#ifdef DEBUG_MPV955
	mpv955_debug_registers(pregs, "inizio close");
#endif

	p_mpv955_data->primo=1; // La funzione close() non e' rientrante
	WRITE_REG(pregs, dac_disable, 0x0001);


	// QUI DEVI RIPRISTINARE I VALORI DI DEFAULT

	/*
	* FINE PROCEDURA CLOSE
	*/

#ifdef DEBUG_MPV955
	ckprintf("mpv955 correctly closed\n");
#endif

	return (OK);
}



/*@********************************************
* Function:     mpv955_write
*
* Description:  entry point function
*
* Input:        *p_mpv955_data - pointer to
*                   the static structure
*               *f - is a pointer to a file
*                   structure
*               *buf - is a pointer to a
*                   character buffer
*               count - specifies the number
*                   byte to copy
*
* Output:       none
*
* Return Value: int - number of written bytes
*
**********************************************/

int mpv955_write(mpv955_data_t *p_mpv955_data, struct file *f, char *buf, int count) {

	int i;
	mpv955_registers_t *pregs = NULL; // Usato per accorciare i nomi


	pregs = p_mpv955_data->registers[p_mpv955_data->area_cm];

#ifdef DEBUG_MPV955
	mpv955_debug_registers(pregs, "inizio write");
	mpv955_debug_memory(p_mpv955_data, "prima di scrivere nella memoria");
#endif
	
	if ((count < 0) || (count > 7)) {
		// Non si sta scrivendo in nessun canale
		pseterr(EINVAL);
		return (SYSERR);
	}
	else {	
		p_mpv955_data->data_memory[count] = *((unsigned short *) buf);
	}

#ifdef DEBUG_MPV955
	mpv955_debug_memory(p_mpv955_data, "dopo aver scritto nella memoria");
#endif

        return (sizeof(unsigned short));
}



/*@********************************************
* Function:     mpv955_ioctl
*
* Description:  entry point function
*
* Input:        *p_mpv955_data - pointer to
*                   the static structure
*               *f - is a pointer to a file
*                   structure
*               command - specifies the
*                   command to execute
*               *arg - is a pointer to a
*                   command argument
*
* Output:       none
*
* Return Value: int - exit status
*
**********************************************/

int mpv955_ioctl(mpv955_data_t *p_mpv955_data, struct file *f, int command, char *arg) {

	mpv955_registers_t *pregs = NULL; // Usato per accorciare i nomi
	unsigned short *punt = NULL; // Puntatore all'argomento
	// Puntatore usato per accorciare i nomi
	pregs = p_mpv955_data->registers[p_mpv955_data->area_cm];

#ifdef DEBUG_MPV955
	mpv955_debug_registers(pregs, "inizio ioctl");
#endif

	punt = (unsigned short *) arg;

	switch (command) {

		case START:
			WRITE_REG(p_mpv955_data->start_register, timeout, p_mpv955_data->timeout_value);
			break;

		case STOP:
			WRITE_REG(p_mpv955_data->registers[AREA_CM_1], start_address_register, 0x0000);
			break;

		case ENABLE_DAC:
			WRITE_REG(pregs, dac_disable, 0x0000);
			break;

		case DISABLE_DAC:
			WRITE_REG(pregs, dac_disable, 0x0001);
			break;

		case ENABLE_TIMEOUT:
			if (READ_REG_BIT(pregs, control_status, CONT_ONE) == CONT_ONE) {
				 // La scheda e' in one-shot mode
				p_mpv955_data->timeout_value = *punt;
				RESET_REG_BIT(pregs, control_status, TIMEOUT_ENABLE);
                        }
                        else {
                                // La scheda e' in continuous mode
				pseterr(EINVAL);
                                return (SYSERR);
                        }
			break;

		case DISABLE_TIMEOUT:
			p_mpv955_data->timeout_value = TIMEOUT_VALUE;
			SET_REG_BIT(pregs, control_status, TIMEOUT_ENABLE);
			break;

		case SET_CONTINUOUS:
			RESET_REG_BIT(pregs, control_status, CONT_ONE);
			break;

		case SET_ONESHOT:
			SET_REG_BIT(pregs, control_status, CONT_ONE);
			break;

		case ENABLE_INT_TRIGGER:
			RESET_REG_BIT(pregs, control_status, INT_EXT);
			RESET_REG_BIT(pregs, control_status, NORMAL_EVENT);
			break;

		case ENABLE_EXT_TRIGGER:
			SET_REG_BIT(pregs, control_status, INT_EXT);
			RESET_REG_BIT(pregs, control_status, NORMAL_EVENT);
			break;

		case ENABLE_EVENT_TRIGGER:
			RESET_REG_BIT(pregs, control_status, INT_EXT);
			SET_REG_BIT(pregs, control_status, NORMAL_EVENT);
			break;

		case GET_TIMEOUT_STATUS:
			if (READ_REG_BIT(pregs, control_status, TIMEOUT) == TIMEOUT) {
				// La scheda e' andata in timeout
				*punt = 1;
			}
			else {
				// La scheda non e' ancora in timeout
				*punt = 0;
			}
			break;

		case GET_OUTPUT_MODE:
			if (READ_REG_BIT(pregs, control_status, CONT_ONE) == CONT_ONE) {
                                // La scheda e' in one-shot mode 
                                *punt = 1;
                        }
                        else {
                                // La scheda e' in continuous mode
                                *punt = 0;
                        }
			break;

		case GET_TRIGGERING_MODE:
			if (READ_REG_BIT(pregs, control_status, INT_EXT) == INT_EXT) {
                                // EXT oppure IMPOSSIBILE 
   				if (READ_REG_BIT(pregs, control_status, NORMAL_EVENT) == NORMAL_EVENT) {
                                	// La scheda e' in uno stato incoerente
                                	*punt = 3;
                        	}       
                        	else {
                                	// La scheda e' in EXT Trigger
                                	*punt = 0;
                        	}
                        }
                        else {
                                // INT oppure EVENT
				if (READ_REG_BIT(pregs, control_status, NORMAL_EVENT) == NORMAL_EVENT) {
                                	// La scheda e' in EVENT Trigger
                                	*punt = 2;
                        	}       
                        	else {  
                                	// La scheda e' in INT Trigger
                                	*punt = 1;
                        	}        
                        }
			break;

		case SET_RATE_TIMER:
			p_mpv955_data->rate_timer = *punt;
			WRITE_REG(pregs, rate_timer_control, p_mpv955_data->rate_timer);
			break;

		case GET_CYCFIN:
			if (READ_REG_BIT(pregs, control_status, CYCFIN) == CYCFIN) {
                                // La scheda ha terminato il ciclo di output 
                                *punt = 1;
                        }
                        else {  
                                // La scheda non ha terminato il ciclo di output
                                *punt = 0;
                        }
			break;

		case GET_OVER_SAMP:
			if (READ_REG_BIT(pregs, control_status, OVER_SAMP) == OVER_SAMP) {
                                // La scheda e' sotto sforzo 
                                *punt = 1;
                        }
                        else {
                                // La scheda non e' sotto sforzo
                                *punt = 0;
                        }
			break;

		case GET_NCHAN:
			*punt = 0x0000;
			(*punt) |= READ_REG_BIT(pregs, control_status, CHAN_S0);
			(*punt) |= READ_REG_BIT(pregs, control_status, CHAN_S1);
			(*punt) |= READ_REG_BIT(pregs, control_status, CHAN_S2);
			(*punt) = (*punt>>4);
			break;

		case GET_ZERO_VOLTS:
			*punt = p_mpv955_data->zero_volts;
			break;

		case SET_NCHAN:
			if (*punt > 7) {
				pseterr(EINVAL);
                        	return (SYSERR);
			}

			if ( ((*punt<<4) & CHAN_S0) == CHAN_S0) {
				SET_REG_BIT(pregs, control_status, CHAN_S0);
			}
			else {
				RESET_REG_BIT(pregs, control_status, CHAN_S0);
			}	
	
			if ( ((*punt<<4) & CHAN_S1) == CHAN_S1) {
                                SET_REG_BIT(pregs, control_status, CHAN_S1);
                        }
                        else {
                                RESET_REG_BIT(pregs, control_status, CHAN_S1);
                        }

			if ( ((*punt<<4) & CHAN_S2) == CHAN_S2) {
                                SET_REG_BIT(pregs, control_status, CHAN_S2);
                        }
                        else {
                                RESET_REG_BIT(pregs, control_status, CHAN_S2);
                        }
			break;

		default:
			pseterr(EINVAL);
			return (SYSERR);
	}



#ifdef DEBUG_MPV955
	mpv955_debug_registers(pregs, "fine ioctl");
	ckprintf("mpv955_ioctl() executed\n");
#endif



	/*
	* FINE PROCEDURA IOCTL
	*/

	return (OK);
}



/*@********************************************
* Function:     mpv955_select
*
* Description:  entry point function
*
* Input:        *p_mpv955_data - pointer to
*                   the static structure
*               *f - is a pointer to a file
*                   structure
*               which - specifies the
*                   condition to monitor
*               *ffs - is a pointer to a sel
*                   data structure
*
* Output:       none
*
* Return Value: int - exit status
*
**********************************************/

int mpv955_select(mpv955_data_t *p_mpv955_data, struct file *f, int which, struct sel *ffs) {


	mpv955_registers_t *pregs = NULL; // Usato per accorciare i nomi

	// Puntatore usato per accorciare i nomi
	pregs = p_mpv955_data->registers[p_mpv955_data->area_cm];

#ifdef DEBUG_MPV955
	mpv955_debug_registers(pregs, "funzione ioctl");
#endif

	/*
	* FINE PROCEDURA SELECT
	*/

#ifdef DEBUG_MPV955
	ckprintf("mpv955_select() executed\n");
#endif

	return (SYSERR); // Operazione non supportata dal device NULL
}



/*@********************************************
* Function:     mpv955_install
*
* Description:  entry point function
*
* Input:        *p_mpv955_info - pointer to
*                   the information structure
*
* Output:       none
*
* Return Value: char * - pointer to the
*                   static structure
*
**********************************************/

char *mpv955_install(mpv955_info_t *p_mpv955_info) {

	mpv955_data_t *p_mpv955_data = NULL;
	char *p_a24_phys_addr = NULL;
	mpv955_registers_t *pregs = NULL; // Usato per accorciare i nomi
	int vaddr = 0;
	int i;


	/*
	* PREPARAZIONE STRUTTURE DATI
	*/
	
	// Alloco memoria per la struttura statics
	p_mpv955_data = (mpv955_data_t *) sysbrk((long) sizeof(mpv955_data_t));
	if (!(p_mpv955_data))
		return (char *) SYSERR;

	// Azzeramento memoria per la struttura statics
	bzero((char *) p_mpv955_data, sizeof(mpv955_data_t));

	
	
	/*
	* INIZIALIZZAZIONE BUS
	*/
	
	// Allocazione canale VME A24
	p_a24_phys_addr = (char *) almavme_channel_alloc("A24_MPV955", 0x0, VME_A24_CHANNEL_SIZE, VMEFG_AM_A24SDATA);

	if (p_a24_phys_addr == (char *) -1) { // Controllo errore
		ckprintf("mpv955_install(): error opening A24 VME channel\n");
		sysfree((char *) p_mpv955_data, sizeof(mpv955_data_t));
		pseterr(ENOMEM);
		return ((char *) SYSERR);
	}

	ckprintf("%s VME-Bus A24 PhysAddr = 0x%X\n", __FUNCTION__, p_a24_phys_addr);
	p_mpv955_data->p_a24_phys_addr = p_a24_phys_addr; // Conservo l´indirizzo fisico

	// Ricavo l´indirizzo virtuale dall´indirizzo fisico
	vaddr = (int) p_a24_phys_addr - 0x10000000 + p_mpv955_info->base_address; 
        // Il valore 0x10000000 e' lo scostamento tra la gli indirizzi fisici
        // e quelli virtuali. Si veda 'man uvmedrvr' e 'man almavmedrvr'

	
	/*
	* INIZIALIZZAZIONE STRUTTURA MPV955_DATA_T
	*/
        p_mpv955_data->info = p_mpv955_info;
	p_mpv955_data->area_cm = AREA_CM;
        p_mpv955_data->registers[0] = (mpv955_registers_t *) (vaddr + REGISTERS_OFFSET_A1);
	p_mpv955_data->registers[1] = (mpv955_registers_t *) (vaddr + REGISTERS_OFFSET_A2);
        p_mpv955_data->start_register = (mpv955_start_register_t *) (vaddr + START_REGISTER_OFFSET);
	p_mpv955_data->data_memory = (unsigned short *) (vaddr);
	p_mpv955_data->zero_volts = ZERO_VOLTS;
	p_mpv955_data->rate_timer = RATE_TIMER;
	p_mpv955_data->timeout_value = TIMEOUT_VALUE;
	p_mpv955_data->primo = 1; // Nessun task ha aperto ancora il dispositivo


#ifdef DEBUG_MPV955
	kkprintf("\n***************STRUTTURA DATA*************\n");
	kkprintf("info = %X\n", p_mpv955_data->info);
	kkprintf("info->base_address = %X\n", p_mpv955_data->info->base_address);
	kkprintf("area_cm = %X\n", p_mpv955_data->area_cm);
	kkprintf("registers[0] = %X\n", p_mpv955_data->registers[0]);
	kkprintf("registers[1] = %X\n", p_mpv955_data->registers[1]);
	kkprintf("start_register = %X\n", p_mpv955_data->start_register);
	kkprintf("data_memory = %X\n", p_mpv955_data->data_memory);
	kkprintf("p_a24_phys_addr = %X\n", p_a24_phys_addr);
	kkprintf("******************************************\n");
#endif


	/*
	* TEST DI FUNZIONAMENTO
	*/
	
        if ( bprobe(vaddr, -1) ) { // Scheda non presente
                ckprintf("%s bprobe board mpv955 not detected\n", __FUNCTION__);
                almavme_channel_free(p_a24_phys_addr);
                sysfree((char *) p_mpv955_data, (long) sizeof(mpv955_data_t));
                pseterr(ENODEV);
                return ((char *) SYSERR);
        }
        

	/*
	* PULITURA DEI CANALI
	*/
	
	// Puntatore usato per accorciare i nomi
	pregs = p_mpv955_data->registers[p_mpv955_data->area_cm];

#ifdef DEBUG_MPV955
	mpv955_debug_registers(pregs, "prima della pulitura");
#endif

	// Inizializzo la memoria a 0V
	for (i=0 ; i < DATA_WORD ; i++) {
		p_mpv955_data->data_memory[i] = p_mpv955_data->zero_volts;
	}

	WRITE_REG(pregs, dac_disable, 0x0001);
	RESET_REG_BIT(pregs, control_status, INT_EXT);
	RESET_REG_BIT(pregs, control_status, NORMAL_EVENT);
	SET_REG_BIT(pregs, control_status, CONT_ONE);
	SET_REG_BIT(pregs, control_status, TIMEOUT_ENABLE);
	SET_REG_BIT(pregs, control_status, CHAN_S0);
	SET_REG_BIT(pregs, control_status, CHAN_S1);
	SET_REG_BIT(pregs, control_status, CHAN_S2);
	RESET_REG_BIT(pregs, control_status, UNUSED);
	WRITE_REG(pregs, rate_timer_control, p_mpv955_data->rate_timer);
	WRITE_REG(pregs, interrupt_control_register, 0x0000);
	WRITE_REG(pregs, start_address_register, 0x0000);
	WRITE_REG(pregs, stop_address_register, 0x000F);

#ifdef DEBUG_MPV955
	mpv955_debug_registers(pregs, "dopo della pulitura");
#endif

	WRITE_REG(p_mpv955_data->start_register, timeout, p_mpv955_data->timeout_value);

	// Attende la pulitura dei registri
        while (READ_REG_BIT(pregs, control_status, CYCFIN) != CYCFIN);
	
#ifdef DEBUG_MPV955
	mpv955_debug_registers(pregs, "dopo la install");
	ckprintf("mpv955 correctly installed\n");
#endif


	/*
	* FINE PROCEDURA INSTALL
	*/


	return (char *) p_mpv955_data;
}



/*@********************************************
* Function:     mpv955_uninstall
*
* Description:  entry point function
*
* Input:        *p_mpv955_data - pointer to
*                   the static structure
*
* Output:       none
*
* Return Value: int - exit status
*
**********************************************/

int mpv955_uninstall(mpv955_data_t *p_mpv955_data) {


	mpv955_registers_t *pregs = NULL; // Usato per accorciare i nomi

	// Puntatore usato per accorciare i nomi
	pregs = p_mpv955_data->registers[p_mpv955_data->area_cm];

#ifdef DEBUG_MPV955
	mpv955_debug_registers(pregs, "funzione ioctl");
	ckprintf("mpv955_uninstall(): start uninstall routine\n");
#endif


	/*
	* LIBERO IL CANALE VME
	*/

	almavme_channel_free(p_mpv955_data->p_a24_phys_addr);

	/*
	* LIBERO LA MEMORIA
	*/
	
	sysfree((char *) p_mpv955_data, (long) sizeof(mpv955_data_t));

	
	/*
	* FINE PROCEDURA UNINSTALL
	*/

#ifdef DEBUG_MPV955
        ckprintf("uninstall mpv955 done\n");
#endif

	return (OK);
}



/*@********************************************
* DLDD DECLARATION
*
* DLDD e' utile solo se si tratta di un driver
* linkato dinamicamente al kenrnel. Se si
* decide di eliminare questa dichiarazione,
* ricordarsi di cancellare le righe:
*
*   include <dldd.h>
*   extern int ionull();
*
**********************************************/

struct dldd entry_points = {
	mpv955_open,      // open
	mpv955_close,     // close
	ionull,           // read
	mpv955_write,     // write
	mpv955_select,    // select
	mpv955_ioctl,     // ioctl
	mpv955_install,   // install
	mpv955_uninstall, // uninstall
	(kaddr_t) 0       // non utilizzato
};

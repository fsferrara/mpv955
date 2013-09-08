###############################################################################
#                                                                             #
#         MPV955 Digital-Analogic Converter Card Device Driver                #
#                                                                             #
#                       Copyright (C) 2005-2006                               #
#               De Sapio Fioravante - desapio82@libero.it                     #
#           Ferrara Francesco Saverio - fs.ferrara@gmail.com                  #
#                                                                             #
#                                                                             #
# This program is free software; you can redistribute it and/or modify it     #
# under the terms of the GNU General Public License as published by the Free  #
# Software Foundation; either version 2 of the License, or (at your option)   #
# any later version.                                                          #
#                                                                             #
# This program is distributed in the hope that it will be useful, but         #
# WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY  #
# or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License    #
# for more details.                                                           #
#                                                                             #
# You should have received a copy of the GNU General Public License along     #
# with this program; if not, write to the Free Software Foundation, Inc., 59  #
# Temple Place - Suite 330, Boston, MA  02111-1307, USA.                      #
#                                                                             #
#                                                                             #
###############################################################################
#
# I target definiti in questo Makefile sono:
#
#         all - Il target principale che serve a compilare il progetto. Esso
#               e' il target di default di quando si chiama il comando make
#               senza argomenti. Se si vuole compilare il progetto basta
#               chiamare make, se invece si preferisce specificare delle
#               opzioni, sono previste:
#                make DEBUG=yes - compila informazioni per il debug
#                make CDK=yes - compila il driver su un Cross Development Kit
#
#       clean - Pulisce la directory da tutti i file creati.
#
#       clear - Cancella i fastidiosi file temporanei che terminano con il
#               carattere '~` creati da alcuni editor di testo.
#
###############################################################################



# Path delle librerie
# PATH_LIB = -L/usr/local/lib/
# LIB = -lblas -lg2c

# Compilare con 'make CDK=yes` se si vogliono aggiungere informazioni per il debug all'eseguibile
ifeq ($(CDK), yes) # Ci troviamo in un Cross Development Kit
	# Directory aggiuntive su un sistema LynxOS
	MORE_DIRECTORY = -I/usr/lynx/4.0.0/ppc/sys/include/kernel -I/usr/lynx/4.0.0/ppc/sys/include/family/ppc -I/usr/lynx/4.0.0/ppc/sys/bsp.vmpc

	# Modificare per scegliere un altro compilatore
	CC = /usr/lynx/4.0.0/ppc/cdk/linux-xcoff-ppc/usr/bin/gcc

	# Modificare per scegliere un altro linker
	LD = /usr/lynx/4.0.0/ppc/cdk/linux-xcoff-ppc/usr/bin/ld
else
	# Directory aggiuntive su un sistema LynxOS
	MORE_DIRECTORY = -I/sys/include/kernel -I/sys/include/family/ppc -I/sys/bsp.vmpc

	# Modificare per scegliere un altro compilatore
	CC = /usr/ppc-xcoff-lynxos/usr/bin/gcc

	# Modificare per scegliere un altro linker
	LD = /usr/ppc-xcoff-lynxos/usr/bin/ld
endif

# Compilatore per gli eseguibili che non fanno parte del driver
GCC = gcc

# Opzioni del linker
LD_OPTION = -bM:SRE -bimport:mpv955.import

# Opzioni passate al compilatore
CFLAG = -DZERO_VOLTS=0x7FFF


# Nome del file eseguibile da creare
DRIVER = MPV955.DRIVER

# Compilare con 'make DEBUG=yes` se si vogliono aggiungere informazioni per il debug all'eseguibile
ifeq ($(DEBUG), yes)
	CFLAG := $(CFLAG) -g -DDEBUG_MPV955
endif


# File sorgenti (.c) e i rispettivi file oggetto (.o)
SOURCES = mpv955drvr.c
OBJECTS = $(SOURCES:.c=.o)
DEVICE_INFO_SRC = mpv955info.c
DEVICE_INFO = $(DEVICE_INFO_SRC:.c=.run)


# Comando per cancellare i file usato durante il target "clean"
DEL = rm -rf


# Comando usato per fare il debug del programma
GDB = gdb


# Target "all"  il target principale.
all : $(DRIVER) $(DEVICE_INFO)
	

# Eseguibili che non fanno parte del driver
$(DEVICE_INFO) : 
	$(GCC) -o $(DEVICE_INFO) $(DEVICE_INFO_SRC) 

  
# Linkaggio per avere il file oggetto (driver)
$(DRIVER) : $(OBJECTS) $(OBJECTS)
	$(LD) $(LD_OPTION) -o $(DRIVER) $(OBJECTS) *.o

$(OBJECTS) : $(SOURCES)
	$(CC) -c $(SOURCES) $(MORE_DIRECTORY) $(PATH_LIB) $(CFLAG) $(LIB) -D__LYNXOS

# Definisco i target PHONY (target che non corrispondono a file)
.PHONY: clean clean_exe clean_obj clear
	

# Pulisce i file dopo la compilazione
clean: clean_exe clean_obj
	

clean_exe:
	$(DEL) $(DRIVER) $(DEVICE_INFO)

clean_obj:
	$(DEL) $(OBJECTS)


# Cancella i fastidiosi file temporanei che terminano con il carattere '~ creati da alcuni editor di testo.
clear:
	$(DEL) *~

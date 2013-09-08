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
##############################################################################


include	$(ENV_PREFIX)/ENVIRONMENT

# Nome del file eseguibile da creare
DRIVER = MPV955.DRIVER

# Path delle librerie
# PATH_LIB = -L/usr/local/lib/
# LIB = -lblas -lg2c

# Directory aggiuntive su un sistema LynxOS
MORE_DIRECTORY = -I. -DLynx -D__Lynx__ -D__NO_INCLUDE_WARN__ -fwritable-strings -fcommon -D__powerpc__ -DLYNXOS -D__LYNXOS -DNDEBUG -fwritable-strings -fcommon -fvolatile-global -fvolatile -finhibit-size-directive -msoft-float -DNO_CSP_MODEL -I/sys/include/io  -I/sys/bsp.vmpc -I. -I/sys/include/kernel -I/sys/include/family/ppc -I/usr/include -I/sys/dheaders -I/sys/bsp.vmpc

# Modificare per scegliere un altro compilatore
CC = /usr/ppc-xcoff-lynxos/usr/bin/gcc

AR = /usr/ppc-xcoff-lynxos/usr/bin/ar
RANLIB = /usr/ppc-xcoff-lynxos/usr/bin/ranlib

# Modificare per scegliere un altro linker
LD = /usr/ppc-xcoff-lynxos/usr/bin/ld

# Compilatore per gli eseguibili che non fanno parte del driver
GCC = gcc

# Opzioni del linker
LD_OPTION = -bM:SRE -bimport:mpv955.import

# Opzioni passate al compilatore
CFLAG = -DZERO_VOLTS=0x7FFF

# Compilare con 'make DEBUG=yes` se si vogliono aggiungere informazioni per il debug all'eseguibile
ifeq ($(DEBUG), yes)
	CFLAG := $(CFLAG) -g -DDEBUG_MPV955
endif


# File sorgenti (.c) e i rispettivi file oggetto (.o)
SOURCES = mpv955drvr.c
OBJECTS = $(SOURCES:.c=.o)
HFILES = $(SOURCES:.c=.h)
DEVICE_INFO_SRC = mpv955info.c
DEVICE_INFO = $(DEVICE_INFO_SRC:.c=.run)
ALL_FILES = Makefile $(SOURCES)
LIBRARY = drivers_vmpc

# Comando per cancellare i file usato durante il target "clean"
DEL = rm -rf


# Comando usato per fare il debug del programma
GDB = gdb



install: all
	

customer_kern_install : all
	

all : ranlib
	

ranlib : cancella
	$(RANLIB) /sys/lib/libdrivers_vmpc.a

cancella : ar_run
	$(DEL) $(OBJECTS)

ar_run : $(DRIVER)
	$(AR) -ruv /sys/lib/libdrivers_vmpc.a $(OBJECTS) 

$(DRIVER) : $(SOURCES)
	$(CC) -c $(SOURCES) $(MORE_DIRECTORY) -o $(OBJECTS) $(CFLAG) $(LIB) -D__LYNXOS -O2 

# Definisco i target PHONY (target che non corrispondono a file)
.PHONY: clean clobber clear
	

clean:
	$(DEL) $(OBJECTS) $(DRIVER)

clobber:
	clean

# Cancella i fastidiosi file temporanei che terminano con il carattere '~ creati da alcuni editor di testo.
clear:
	$(DEL) *~

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
* File:          mpv955info.h
*
* P.N.:
*
* Product:       Pentland MPV955 DAC
*                LynxOS Device Driver
*
* Authors:       De Sapio Fioravante
*                Francesco Saverio Ferrara
*
* Creation date: 22 october 2005
*
* Description:   Definition of the device
*                information data structure
*
*
**********************************************/


#ifndef _MPV955INFO_H_
#define _MPV955INFO_H_



typedef struct mpv955_info {
	unsigned int base_address;      // A32 VME base address
} mpv955_info_t;



#endif /* _MPV955INFO_H_ */

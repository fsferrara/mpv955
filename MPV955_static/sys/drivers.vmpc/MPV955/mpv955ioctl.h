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
* File:          mpv955ioctl.h
*
* P.N.:
*
* Product:       Pentland MPV955 DA Converter
*                LynxOS 4.0.0 Device Driver
*
* Creation date: 03 december 2005
*
* Description:   IOCTL Commands definition for
*                MPV955 device driver
*
*
**********************************************/



#ifndef _MPV955IOCTL_H_
#define _MPV955IOCTL_H_



/*@********************************************
* DEFINE IOCTL COMMANDS
**********************************************/

#define  IOCTL          (('i'<<24) | ('o'<<16))
#define  START                 (IOCTL | 0x0001)
#define  STOP                  (IOCTL | 0x0002)
#define  ENABLE_DAC            (IOCTL | 0x0003)
#define  DISABLE_DAC           (IOCTL | 0x0004)
#define  ENABLE_TIMEOUT        (IOCTL | 0x0005)
#define  DISABLE_TIMEOUT       (IOCTL | 0x0006)
#define  SET_CONTINUOUS        (IOCTL | 0x0007)
#define  SET_ONESHOT           (IOCTL | 0x0008)
#define  ENABLE_INT_TRIGGER    (IOCTL | 0x0009)
#define  ENABLE_EXT_TRIGGER    (IOCTL | 0x000A)
#define  ENABLE_EVENT_TRIGGER  (IOCTL | 0x000B)
#define  GET_TIMEOUT_STATUS    (IOCTL | 0x000C)
#define  GET_OUTPUT_MODE       (IOCTL | 0x000D)
#define  GET_TRIGGERING_MODE   (IOCTL | 0x000E)
#define  SET_RATE_TIMER        (IOCTL | 0x000F)
#define  GET_CYCFIN            (IOCTL | 0x0010)
#define  GET_OVER_SAMP         (IOCTL | 0x0011)
#define  GET_NCHAN             (IOCTL | 0x0012)
#define  SET_NCHAN             (IOCTL | 0x0013)
#define  GET_ZERO_VOLTS        (IOCTL | 0x0014)



#endif

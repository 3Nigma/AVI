/*
***************************************************************************
*
* Author: Victor ADASCALITEI
*
* Copyright (C) 2012 Victor ADASCALITEI
*
* adascalitei.victor@gmail.com
*
***************************************************************************
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation version 2 of the License.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
***************************************************************************
*
* This version of GPL is at http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*
***************************************************************************
*/

#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include <stdlib.h>
#include <gtk/gtk.h>
#include <unistd.h>
#include "rs232.h"
#include "datatypes.h"

#define ttyUSB0 16

extern int initLink(int port, int baud);
extern void closeLink(int port);

extern gboolean isPCAviOnline();
extern gboolean softResetCam(uint8_t);
extern gboolean powerDownCam(uint8_t);
extern gboolean getPixelData(uint8_t, uint8_t **, uint8_t);

#endif

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

#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include<gtk/gtk.h>

extern GtkWidget *avi_new_console();
extern GtkWidget *avi_new_console_labeled();

extern void avi_console_append_text(GtkWidget *textview, gchar *text);

#endif

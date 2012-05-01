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

#ifndef _CAMWIDGET_H_
#define _CAMWIDGET_H_

#include <gtk/gtk.h>

#define AVI_CAM_WIDTH 18
#define AVI_CAM_HEIGHT 18
#define AVI_CAM_BUFLENGTH ((AVI_CAM_HEIGHT * AVI_CAM_WIDTH + 4)/8)

extern void DrawCamWidgetBorder(cairo_t *cr, int widWidth, int widHeight, double re, double gr, double bl);
extern void RenderCamWidget(cairo_t *cr, int widWidth, int widHeight, int camWidth, int camHeight, guint8 *pixData);
extern GtkWidget *avi_new_cam(gpointer *pixDataHolder);

#endif

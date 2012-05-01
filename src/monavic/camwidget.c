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

#include "camwidget.h"

void DrawCamWidgetBorder(cairo_t *cr, int widWidth, int widHeight, double re, double gr, double bl) {
  cairo_set_line_width(cr, 1.0);
  cairo_set_source_rgb(cr, re, gr, bl);
  cairo_rectangle(cr, 0.5, 0.5, (double)widWidth - 0.5, (double)widHeight - 0.5);
  cairo_stroke(cr);
}

void RenderCamWidget(cairo_t *cr, int widWidth, int widHeight, int camWidth, int camHeight, guint8 *pixData) {
  g_assert(NULL != cr);
  g_assert(widWidth >= camWidth);
  g_assert(widHeight >= camHeight);
  g_assert(NULL != pixData);

  double pixelWidth = (double)widWidth/camWidth;
  double pixelHeight = (double)widHeight/camHeight;
  double xCoord = 0;
  double yCoord = 0;
  int pixDataIndex = 0;
  int drawnPixelCount = 0;

  cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);

  for(yCoord = 0.0; yCoord < (double)widHeight - pixelHeight; yCoord += pixelHeight) {
    for(xCoord = 0.0; xCoord < (double)widWidth - pixelWidth + 1e-7; xCoord += pixelWidth) {
      if((pixData[pixDataIndex] & 0x80) != 0) {
        cairo_rectangle(cr, xCoord, yCoord, pixelWidth, pixelHeight);
      }
      pixData[pixDataIndex] <<= 1;
      drawnPixelCount ++;
      if((drawnPixelCount % (sizeof(pixData[0]) * 8)) == 0) {
        pixDataIndex ++;
      }
    }
  }

  cairo_fill (cr);
}

static gboolean ExposeCamWidgetCallback(GtkWidget *widget, GdkEventExpose *event, gpointer data) {
  cairo_t *cr = gdk_cairo_create (widget->window);
  gpointer pixDupData = g_memdup(data, (AVI_CAM_WIDTH * AVI_CAM_HEIGHT)/8 + 1);

  DrawCamWidgetBorder(cr, widget->allocation.width, widget->allocation.height, 0.0, 0.5, 0.25);
  RenderCamWidget(cr, widget->allocation.width, widget->allocation.height, AVI_CAM_WIDTH, AVI_CAM_HEIGHT, pixDupData);

  g_free(pixDupData);
  cairo_destroy(cr);
  return TRUE;
}

GtkWidget *avi_new_cam(gpointer *pixDataHolder) {
  GtkWidget *cam = gtk_drawing_area_new();

  gtk_widget_set_size_request(cam, 250, 250);
  g_signal_connect(G_OBJECT(cam), "expose_event",  
                   G_CALLBACK(ExposeCamWidgetCallback), pixDataHolder);

  return cam;
}

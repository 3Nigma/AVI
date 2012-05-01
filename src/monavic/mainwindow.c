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

#include "mainwindow.h"

guint8 topCamPixData[AVI_CAM_BUFLENGTH];
guint8 bottomCamPixData[AVI_CAM_BUFLENGTH];

static gboolean delete_event(GtkWidget *widget, GdkEvent *ev, gpointer data) {
  gtk_main_quit();
  return FALSE;
}

GtkWidget *avi_new_button_box() {
  GtkWidget *comButtons = NULL;
  GtkWidget *btStop = NULL;
  GtkWidget *btLoadScript = NULL;
  GtkWidget *btManualCtrl = NULL;

  btLoadScript = gtk_button_new_with_mnemonic("_Run script");
  btStop = gtk_button_new_with_mnemonic("_STOP!"); 
  btManualCtrl = gtk_toggle_button_new_with_mnemonic("_Auto Run"); 

  comButtons = gtk_hbutton_box_new();
  gtk_container_add(GTK_CONTAINER(comButtons), btStop);
  gtk_container_add(GTK_CONTAINER(comButtons), btLoadScript);
  gtk_container_add(GTK_CONTAINER(comButtons), btManualCtrl);

  return comButtons;
}

GtkWidget *avi_new_appwindow() {
  GtkWidget *window = NULL;
  GtkWidget *windowLayout = NULL;
  GtkWidget *imgCamTop = NULL;
  GtkWidget *imgCamBottom = NULL;

  /* initialize main window */
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "AVI Monitor");
  gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
  g_signal_connect(window, "delete-event", G_CALLBACK(delete_event), NULL);
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);

  /* initialize the camera widgets */
  imgCamTop = avi_new_cam((gpointer *)&topCamPixData[0]);
  imgCamBottom = avi_new_cam((gpointer *)&bottomCamPixData[0]);

  /* add window container and organize widgets */
  windowLayout = gtk_table_new(3, 3, FALSE);
  gtk_table_attach_defaults(GTK_TABLE(windowLayout), imgCamTop, 0, 1, 0, 1);
  gtk_table_attach_defaults(GTK_TABLE(windowLayout), gtk_image_new_from_file("../../doc/avi_cam_layout_small.png"), 1, 2, 0, 1);
  gtk_table_attach_defaults(GTK_TABLE(windowLayout), imgCamBottom, 2, 3, 0, 1);
  gtk_table_attach_defaults(GTK_TABLE(windowLayout), avi_new_console_labeled(), 0, 3, 1, 2);
  gtk_table_attach_defaults(GTK_TABLE(windowLayout), avi_new_button_box(), 0, 3, 2, 3);
  gtk_container_add(GTK_CONTAINER(window), windowLayout);

  /* check if pcavi is present */
  if(isPCAviOnline())
    avi_console_append_text(NULL, "PC Avi ONLINE!\n");
  else
    avi_console_append_text(NULL, "PC Avi OFFLINE!\n");

  //softResetCam(CAR_CAMS_ID_MID);
  //getPixelData(CAR_CAMS_ID_MID, (uint8_t **)&bottomCamPixData, AVI_CAM_BUFLENGTH);
  //sleep(1);
  //powerDownCam(CAR_CAMS_ID_MID);

  return window;
}

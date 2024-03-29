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

#include <gtk/gtk.h>

#include "commands.h"
#include "mainwindow.h"

int main(int argc, char *argv[]) {
  GtkWidget *window = NULL;

  gtk_init(&argc, &argv);

  /* initialize serial link */
  if(initLink(ttyUSB0, 19200) == FALSE) {
    fprintf(stderr, "Error on opening serial port!\n");
    return -1;
  }

  /* initialize main window */
  window = avi_new_appwindow();
  gtk_widget_show_all(window);

  gtk_main();

  closeLink(ttyUSB0);
  return 0;
}

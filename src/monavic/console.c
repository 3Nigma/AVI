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

#include "console.h"

GtkWidget *consoleView;

GtkWidget *avi_new_console() {
  if(NULL == consoleView) {
    consoleView = gtk_text_view_new();
    gtk_widget_set_size_request(consoleView, 1, 150);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(consoleView), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(consoleView), FALSE);

    avi_console_append_text(consoleView, "MonAviC process started...\n");
  }

  return consoleView;
}

GtkWidget *avi_new_console_labeled() {
  GtkWidget *container = NULL;
  GtkWidget *label = NULL;

  label = gtk_label_new("AVI Console :");
  gtk_misc_set_alignment(GTK_MISC(label),0.0,0.5);
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);

  container = gtk_table_new(2, 1, FALSE);
  gtk_table_attach(GTK_TABLE(container), label, 0, 1, 0, 1, GTK_FILL, GTK_FILL, 0, 0);
  gtk_table_attach(GTK_TABLE(container), avi_new_console(), 0, 1, 1, 2, GTK_FILL|GTK_EXPAND, GTK_FILL, 0, 0);

  return container;
}

void avi_console_append_text(GtkWidget *textview, gchar *text) {
  GtkTextBuffer *tbuffer;
  GtkTextIter ei;

  if(NULL == textview) {
    g_assert(NULL != consoleView);
    textview = consoleView;
  }

  tbuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
  gtk_text_buffer_get_end_iter(tbuffer, &ei);
  gtk_text_buffer_insert(tbuffer, &ei, text, -1);
}

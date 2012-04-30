#include <gtk/gtk.h>

#define AVI_CAM_WIDTH 18
#define AVI_CAM_HEIGHT 18

guint8 topCamPixData[] = {0b10101010, 0b10101010, 0b10100000, 0b00000000, 0b00000000, 0b00000000,
                            0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
                            0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
                            0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
                            0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
                            0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
                            0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00010000};

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

static gboolean ExposeWidgetCallback(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
  cairo_t *cr = gdk_cairo_create (widget->window);
  gpointer pixDupData = g_memdup(data, (AVI_CAM_WIDTH * AVI_CAM_HEIGHT)/8 + 1);

  RenderCamWidget(cr, widget->allocation.width, widget->allocation.height, AVI_CAM_WIDTH, AVI_CAM_HEIGHT, pixDupData);

  g_free(pixDupData);
  cairo_destroy(cr);
  return TRUE;
}

static gboolean delete_event(GtkWidget *widget, GdkEvent *ev, gpointer data) {
  gtk_main_quit();
  return FALSE;
}

int main(int argc, char *argv[]) {
  GtkWidget *window;
  GtkWidget *imgCamTop;

  gtk_init(&argc, &argv);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "AVI Monitor");
  g_signal_connect(window, "delete-event", G_CALLBACK(delete_event), NULL);
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);

  imgCamTop = gtk_drawing_area_new();
  gtk_widget_set_size_request (imgCamTop, 250, 250);
  g_signal_connect (G_OBJECT (imgCamTop), "expose_event",  
                    G_CALLBACK (ExposeWidgetCallback), (gpointer *)&topCamPixData[0]);
  gtk_container_add(GTK_CONTAINER(window), imgCamTop);

  /*box1 = gtk_hbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(window), box1);

  button = gtk_button_new_with_label("Buton 1!");
  g_signal_connect(button, "clicked", G_CALLBACK(hello), (gpointer)"Buton 1");
  gtk_box_pack_start(GTK_BOX(box1), button, TRUE, TRUE, 0);
  gtk_widget_show(button);

  button = gtk_button_new_with_label("Buton 2!");
  g_signal_connect(button, "clicked", G_CALLBACK(hello), (gpointer)"Buton 2");
  gtk_box_pack_start(GTK_BOX(box1), button, TRUE, TRUE, 0);
  gtk_widget_show(button);

  button = gtk_button_new_with_label("Quit");
  g_signal_connect(button, "clicked", G_CALLBACK(delete_event), NULL);
  gtk_box_pack_start(GTK_BOX(box1), button, TRUE, TRUE, 0);
  gtk_widget_show(button);

  gtk_widget_show(box1);*/
  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}

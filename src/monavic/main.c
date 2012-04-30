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

guint8 bottomCamPixData[] = {0b01010101, 0b01010101, 0b01000000, 0b00000000, 0b00000000, 0b00000000,
                            0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
                            0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
                            0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
                            0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
                            0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
                            0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b10000000};

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

static gboolean delete_event(GtkWidget *widget, GdkEvent *ev, gpointer data) {
  gtk_main_quit();
  return FALSE;
}

int main(int argc, char *argv[]) {
  GtkWidget *window = NULL;
  GtkWidget *windowLayout = NULL;
  GtkWidget *imgCamTop = NULL;
  GtkWidget *imgCamBottom = NULL;

  gtk_init(&argc, &argv);

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
  gtk_container_add(GTK_CONTAINER(window), windowLayout);

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

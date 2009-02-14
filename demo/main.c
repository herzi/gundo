#include <glib/gtypes.h>
#include <gtk/gtkmain.h>

#include "sketch.h"
#include "sketch-window.h"

gint
main(gint argc, gchar** argv) {
	gtk_init(&argc, &argv);
	gtk_widget_show_all(sketch_window_new());
	gtk_main();
	quit_sketch();
	return 0;
}


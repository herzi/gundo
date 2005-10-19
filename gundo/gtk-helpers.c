#include "gtk-helpers.h"

void
gtk_widget_get_extends(GtkWidget* widget, gint* x, gint* y, gint* w, gint* h) {
	GdkWindow* window = widget->window;
	gdk_window_get_origin(window, x, y);

	if(x) {
		*x += widget->allocation.x;
	}

	if(y) {
		*y += widget->allocation.y;
	}

	if(w) {
		*w =  widget->allocation.width;
	}

	if(h) {
		*h =  widget->allocation.height;
	}
}


#warning "FIXME: add GPL intro"

#ifndef GTK_HELPERS_H
#define GTK_HELPERS_H

#include <gtk/gtkwidget.h>

G_BEGIN_DECLS

void gtk_widget_get_extends(GtkWidget* widget, gint* x, gint* y, gint* w, gint* h);

G_END_DECLS

#endif /* !GTK_HELPERS_H */

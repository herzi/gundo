#ifndef SKETCH_WINDOW_H
#define SKETCH_WINDOW_H

#include <gtk/gtkwidget.h>
#include "sketch.h"

typedef struct _SketchWindow SketchWindow;
typedef GtkWindowClass SketchWindowClass;

#define SKETCH_TYPE_WINDOW         (sketch_window_get_type())
#define SKETCH_WINDOW(o)           (G_TYPE_CHECK_INSTANCE_CAST((o), SKETCH_TYPE_WINDOW, SketchWindow))
#define SKETCH_WINDOW_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), SKETCH_TYPE_WINDOW, SketchWindowClass))
#define SKETCH_IS_WINDOW(o)        (G_TYPE_CHECK_INSTANCE_TYPE((o), SKETCH_TYPE_WINDOW))
#define SKETCH_IS_WINDOW_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE((k), SKETCH_TYPE_WINDOW))
#define SKETCH_WINDOW_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS((o), SKETCH_TYPE_WINDOW, SketchWindowClass))

GType      sketch_window_get_type(void);

GtkWidget* sketch_window_new(void);
void       sketch_window_set_sketch(SketchWindow* win, Sketch* sketch);

#endif /* !SKETCH_WINDOW_H */

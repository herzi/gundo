#ifndef SKETCH_H
#define SKETCH_H

#include <glib/garray.h>
#include <gtk/gtkobject.h>
#include <gundo/gundo.h>

#include "stroke.h"

typedef struct _Sketch Sketch;
typedef struct _SketchClass SketchClass;

#define SKETCH_TYPE         (sketch_get_type())
#define SKETCH(o)           (G_TYPE_CHECK_INSTANCE_CAST((o), SKETCH_TYPE, Sketch))
#define SKETCH_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), SKETCH_TYPE, SketchClass))
#define IS_SKETCH(o)        (G_TYPE_CHECK_INSTANCE_TYPE((o), SKETCH_TYPE))
#define IS_SKETCH_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE((k), SKETCH_TYPE))
#define SKETCH_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS((o), SKETCH_TYPE, SketchClass))

GType          sketch_get_type   (void);

Sketch*        sketch_new        (void);
void           sketch_free       (Sketch* sketch);

void           sketch_clear      (Sketch* sketch);
GundoSequence* sketch_get_actions(Sketch* sketch);
Stroke*        sketch_get_last_stroke(Sketch* self);

void           sketch_start_stroke(Sketch* sketch);
void           sketch_add_point   (Sketch* sketch,
				   guint   x,
				   guint   y);
void	       sketch_finish_stroke(Sketch* sketch);

void quit_sketch(void);

struct _Sketch {
	GtkObject      object;
	GPtrArray    * strokes;
	Stroke       * current;
	GundoSequence* actions;
};

struct _SketchClass {
	GtkObjectClass object_class;

	/* signals */
	void (*stroke_added)  (Sketch* sk,
			       Stroke* st);
	void (*stroke_removed)(Sketch* sk,
			       Stroke* st);
};

GundoActionType action_type;

#endif /* !SKETCH_H */

#include "sketch-window.h"

#include <gundo/gundo.h>
#include <gundo/gundo-ui.h>
#include <gundo/gundo-tool-undo.h>

#include "stroke.h"

struct _SketchWindow {
	GtkWindow  window;
	Sketch   * s;
	GtkWidget* da;
	GtkWidget* statusbar;
	guint context_id;
	GtkToolItem* redo,
		   * undo,
		   * clear;
};

G_DEFINE_TYPE(SketchWindow, sketch_window, GTK_TYPE_WINDOW)

Sketch*
sketch_window_get_sketch(SketchWindow* win) {
	return win->s;
}

static void
draw_stroke(Stroke* s, GdkDrawable* d, GdkGC* gc) {
	g_return_if_fail(s);
	g_return_if_fail(s->points);

	gdk_draw_lines(d, gc,
		       (GdkPoint*)s->points->data,
		       s->points->len);
}

static void
sw_stroke_added(SketchWindow* self, Stroke* st, Sketch* sk) {
	draw_stroke(st, self->da->window, self->da->style->black_gc);
}

static void
sw_stroke_removed(SketchWindow* self, Stroke* st, Sketch* sk) {
	gtk_widget_queue_draw(self->da);
}

void
sketch_window_set_sketch(SketchWindow* win, Sketch* sketch) {
	Sketch   * cur = sketch_window_get_sketch(win);
	
	if(cur) {
		g_signal_handlers_disconnect_by_func(sketch, G_CALLBACK(sw_stroke_added), win);
		g_object_unref(cur);
	}

	win->s = g_object_ref(sketch);
	gtk_object_sink(GTK_OBJECT(sketch));

	gundo_tool_undo_connect(GUNDO_TOOL_UNDO(win->undo), sketch_get_actions(sketch));
	gundo_make_redo_sensitive(GTK_WIDGET(win->redo),  sketch_get_actions(sketch));
	gundo_make_undo_sensitive(GTK_WIDGET(win->clear), sketch_get_actions(sketch));

	g_signal_connect_swapped(sketch, "stroke-added",
			         G_CALLBACK(sw_stroke_added), win);
	g_signal_connect_swapped(sketch, "stroke-removed",
			         G_CALLBACK(sw_stroke_removed), win);
}

static gboolean
clear_clicked(SketchWindow* self) {
	sketch_clear(sketch_window_get_sketch(self));
	return TRUE;
}

static gboolean
redo_clicked(SketchWindow* win) {
	Sketch* s = sketch_window_get_sketch(win);
	gundo_sequence_redo(s->actions);
	return TRUE;
}

static gboolean
da_expose_event(SketchWindow* win, GdkEventExpose* ev, GtkWidget* da) {
	Sketch* s = sketch_window_get_sketch(win);
	guint i;
	for(i = 0; i < s->strokes->len; i++) {
		draw_stroke((Stroke*)g_ptr_array_index(s->strokes, i), da->window, da->style->black_gc);
	}
	return TRUE;
}

static gboolean
da_button_press_event(SketchWindow* win, GdkEventButton* ev, GtkWidget* da) {
	Sketch* s = sketch_window_get_sketch(win);
	sketch_start_stroke(s);
	sketch_add_point(s, ev->x, ev->y);
	return TRUE;
}

static gboolean
da_button_release_event(SketchWindow* win, GdkEventButton* ev, GtkWidget* da) {
	sketch_finish_stroke(sketch_window_get_sketch(win));
	return TRUE;
}

static gboolean
da_motion_notify_event(SketchWindow* win, GdkEventMotion* ev, GtkWidget* da) {
	sketch_add_point(sketch_window_get_sketch(win), ev->x, ev->y);
	draw_stroke(sketch_window_get_sketch(win)->current, da->window, da->style->black_gc);
	return TRUE;
}

static void
sketch_window_class_init(SketchWindowClass* k) {
}

static void
sketch_window_init(SketchWindow* self) {
	Sketch   * s   = sketch_new();
	gtk_window_set_title(GTK_WINDOW(self), "Undo Demo - Sketch");
	gtk_window_set_policy(GTK_WINDOW(self), TRUE, TRUE, TRUE);
	g_signal_connect(self, "delete-event", 
			 G_CALLBACK(gtk_main_quit), NULL);

	/* vertical box */
	{
		GtkWidget* vbox = gtk_vbox_new(FALSE, 0);

		/* toolbar */
		{
			GtkWidget* toolbar = gtk_toolbar_new();
			gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_ICONS);
			/* quit */
			{
				GtkToolItem* quit = gtk_tool_button_new_from_stock(GTK_STOCK_QUIT);
				g_signal_connect(quit, "clicked",
						 G_CALLBACK(gtk_main_quit), NULL);
				gtk_toolbar_insert(GTK_TOOLBAR(toolbar), GTK_TOOL_ITEM(quit), -1);
			}

			gtk_toolbar_insert(GTK_TOOLBAR(toolbar), gtk_separator_tool_item_new(), -1);

			/* undo */
			{
				self->undo = gundo_tool_undo_new();
				gtk_toolbar_insert(GTK_TOOLBAR(toolbar), GTK_TOOL_ITEM(self->undo), -1);
			}
			
			/* redo */
			{
				self->redo = gtk_tool_button_new_from_stock(GTK_STOCK_REDO);
				g_signal_connect_swapped(self->redo, "clicked",
							 G_CALLBACK(redo_clicked), self);
				gtk_toolbar_insert(GTK_TOOLBAR(toolbar), GTK_TOOL_ITEM(self->redo), -1);
			}
			
			/* clear */
			{
				self->clear = gtk_tool_button_new_from_stock(GTK_STOCK_CLEAR);
				g_signal_connect_swapped(self->clear, "clicked",
							 G_CALLBACK(clear_clicked), self);
				gtk_toolbar_insert(GTK_TOOLBAR(toolbar), self->clear, -1);
			}
			gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, FALSE, 0);
		}

		/* drawing area */
		{
			self->da = gtk_drawing_area_new();
			gtk_drawing_area_size(GTK_DRAWING_AREA(self->da), 256, 256 );
			gtk_widget_set_events(self->da, gtk_widget_get_events(self->da)
						  |GDK_EXPOSURE_MASK|GDK_BUTTON_MOTION_MASK
						  |GDK_BUTTON_PRESS_MASK
						  |GDK_BUTTON_RELEASE_MASK);
			    
			g_signal_connect_swapped(self->da, "expose-event",
						 G_CALLBACK(da_expose_event), self);
			g_signal_connect_swapped(self->da, "button-press-event",
						 G_CALLBACK(da_button_press_event), self);
			g_signal_connect_swapped(self->da, "button-release-event",
						 G_CALLBACK(da_button_release_event), self);
			g_signal_connect_swapped(self->da, "motion-notify-event",
						 G_CALLBACK(da_motion_notify_event), self);
			
			gtk_box_pack_start(GTK_BOX(vbox), self->da, TRUE, TRUE, 0);
		}
		
		/* statusbar */
		{
		
			self->statusbar = gtk_statusbar_new();
			
			self->context_id = gtk_statusbar_get_context_id(GTK_STATUSBAR(self->statusbar),
									"sketch");
			gtk_statusbar_push(GTK_STATUSBAR(self->statusbar), self->context_id, 
					   "click and drag to create lines");
			
			gtk_box_pack_end(GTK_BOX(vbox), self->statusbar, FALSE, FALSE, 0);
		}	
		
		gtk_container_add(GTK_CONTAINER(self), vbox);
	}
    
	sketch_window_set_sketch(self, s);
}

GtkWidget*
sketch_window_new(void) {
	return GTK_WIDGET(g_object_new(SKETCH_TYPE_WINDOW, "type", GTK_WINDOW_TOPLEVEL, NULL));
}


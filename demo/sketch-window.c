/* This file is part of gundo, a multilevel undo/redo facility for GTK+
 *
 * Copyright (C) 1999  Nat Pryce
 * Copyright (C) 2005  Sven Herzberg
 * Copyright (C) 2009  Stefan Kost
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of the
 * License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 */

#include "sketch-window.h"

#include <gundo.h>
#include <gundo-ui.h>

#include <gtk/gtkdrawingarea.h>
#include <gtk/gtkmain.h>
#include <gtk/gtkseparatortoolitem.h>
#include <gtk/gtkstatusbar.h>
#include <gtk/gtkstock.h>
#include <gtk/gtktoolbar.h>
#include <gtk/gtktoolbutton.h>
#include <gtk/gtkvbox.h>

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
		g_signal_handlers_disconnect_by_func(sketch, G_CALLBACK(sw_stroke_removed), win);
		g_object_unref(cur);
	}

	win->s = g_object_ref(sketch);
	gtk_object_sink(GTK_OBJECT(sketch));

        gundo_tool_set_history (GUNDO_TOOL (win->redo), GUNDO_HISTORY (sketch_get_actions (sketch)));
        gundo_tool_set_history (GUNDO_TOOL (win->undo), GUNDO_HISTORY (sketch_get_actions (sketch)));

	g_signal_connect_swapped(sketch, "stroke-added",
			         G_CALLBACK(sw_stroke_added), win);
	g_signal_connect_swapped(sketch, "stroke-removed",
			         G_CALLBACK(sw_stroke_removed), win);

  gtk_widget_queue_draw (win->da);
}

static gboolean
clear_clicked (SketchWindow* self)
{
  sketch_window_set_sketch (self, sketch_new ());

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
sketch_window_dispose(GObject *object) {
  SketchWindow* self = SKETCH_WINDOW(object);
  
  if(self->s) {
    g_object_unref(self->s);
    self->s = NULL;
  }
}

static void
sketch_window_class_init(SketchWindowClass* k) {
  GObjectClass *gobject_class = G_OBJECT_CLASS(k);
    
  gobject_class->dispose     = sketch_window_dispose;
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
                        /* FIXME: properly implement toolbar styles and drop that */
                        gtk_toolbar_set_show_arrow (GTK_TOOLBAR (toolbar), FALSE);
			gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_ICONS);
			/* quit */
			{
				GtkToolItem* quit = gtk_tool_button_new_from_stock(GTK_STOCK_QUIT);
				g_signal_connect(quit, "clicked",
						 G_CALLBACK(gtk_main_quit), NULL);
				gtk_toolbar_insert(GTK_TOOLBAR(toolbar), GTK_TOOL_ITEM(quit), -1);
			}

			gtk_toolbar_insert(GTK_TOOLBAR(toolbar), gtk_separator_tool_item_new(), -1);

                        /* new */
                        {
                          self->clear = gtk_tool_button_new_from_stock (GTK_STOCK_NEW);
                          g_signal_connect_swapped (self->clear, "clicked",
                                                    G_CALLBACK (clear_clicked), self);
                          gtk_toolbar_insert (GTK_TOOLBAR (toolbar), self->clear, -1);
                        }

                        gtk_toolbar_insert(GTK_TOOLBAR(toolbar), gtk_separator_tool_item_new(), -1);

			/* undo */
			{
				self->undo = gundo_tool_undo_new();
				gtk_toolbar_insert(GTK_TOOLBAR(toolbar), GTK_TOOL_ITEM(self->undo), -1);
			}
			
			/* redo */
			{
				self->redo = gundo_redo_tool_new ();
				gtk_toolbar_insert(GTK_TOOLBAR(toolbar), GTK_TOOL_ITEM(self->redo), -1);
			}
			
			gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, FALSE, 0);
		}

		/* drawing area */
		{
			self->da = gtk_drawing_area_new();
			gtk_drawing_area_size(GTK_DRAWING_AREA(self->da), 256, 256 );
			gtk_widget_set_events(self->da, gtk_widget_get_events(self->da)
						  |GDK_EXPOSURE_MASK | GDK_BUTTON_MOTION_MASK
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


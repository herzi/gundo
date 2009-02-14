/* This file is part of gundo, a multilevel undo/redo facility for GTK+
 * 
 * AUTHORS
 *	Nat Pryce
 *	Sven Herzberg		<herzi@gnome-de.org>
 *
 * Copyright (C) 1999		Nat Pryce
 * Copyright (C) 2005		Sven Herzberg
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

#include "sketch.h"

#include <gtk/gtk.h>
#include <gundo-ui/gundo-ui.h>

#include "stroke.h"

Sketch*
sketch_new(void) {
	return g_object_new(SKETCH_TYPE, NULL);;
}

void
sketch_clear(Sketch* s) {
	guint i;

	gundo_sequence_clear(s->actions);

	for(i = 0; i < s->strokes->len; i++) {
		Stroke* st = (Stroke*)g_ptr_array_index(s->strokes, i);
		g_signal_emit_by_name(s, "stroke-removed", st);
		stroke_destroy(st);
	}

	g_ptr_array_set_size(s->strokes, 0);
}

GundoSequence*
sketch_get_actions(Sketch* s) {
	g_return_val_if_fail(s, NULL);

	return s->actions;
}

Stroke*
sketch_get_last_stroke(Sketch* s) {
	return (Stroke*)g_ptr_array_index(s->strokes, s->strokes->len - 1);
}

void
sketch_start_stroke(Sketch* s) {
	s->current = stroke_new();
}

void
sketch_add_point(Sketch* s, guint x, guint y) {
	stroke_add_point(s->current, x, y);
}

struct stroke_change {
	Sketch* sk;
	Stroke* st;
};

static void
s_add_stroke(Sketch* sk, Stroke* st) {
	g_ptr_array_add(sk->strokes, st);
	g_signal_emit_by_name(sk, "stroke-added", st);
}

void
sketch_finish_stroke(Sketch* sk) {
	struct stroke_change* s = g_new0(struct stroke_change, 1);
	s->sk = g_object_ref(sk);
	s->st = stroke_copy(sk->current);
	
	s_add_stroke(sk, sk->current);
	
	gundo_sequence_add_action(sk->actions, &action_type, s);
	sk->current = NULL;
}

static void
undo_stroke(struct stroke_change* c) {
	/* shrink the stroke list by one */
        g_ptr_array_set_size(c->sk->strokes, c->sk->strokes->len - 1);

	g_signal_emit_by_name(c->sk, "stroke-removed", c->st);
}

static void
redo_stroke(struct stroke_change* c) {
	s_add_stroke(c->sk, stroke_copy(c->st));
}

static void
free_stroke(struct stroke_change* c) {
	g_object_unref(c->sk);
	stroke_destroy(c->st);
	g_free(c);
}

GundoActionType action_type = {
	(GundoActionCallback)undo_stroke,
	(GundoActionCallback)redo_stroke,
	(GundoActionCallback)free_stroke
};

// CHECKED // CHECKED // CHECKED // CHECKED // CHECKED // CHECKED // CHECKED //

/*  Globals */
void quit_sketch() {
/*    int i;
    
    gtk_widget_destroy( window );
    if( current_stroke ) stroke_destroy( current_stroke );
    
    for( i = 0; i < visible_strokes->len; i++ ) {
        stroke_destroy( (Stroke*)g_ptr_array_index( visible_strokes, i ) );
    }
    g_ptr_array_free( visible_strokes, TRUE );
    
    gtk_object_destroy( GTK_OBJECT(actions) );*/
}

static void
s_stroke_added(Sketch* sk, Stroke* st) {}

static void
s_stroke_removed(Sketch* sk, Stroke* st) {}

static void
sketch_class_init(SketchClass* klass) {
	g_signal_new("stroke-added",
		     SKETCH_TYPE,
		     G_SIGNAL_RUN_LAST,
		     G_STRUCT_OFFSET(SketchClass, stroke_added),
		     NULL, NULL,
		     g_cclosure_marshal_VOID__BOXED,
		     G_TYPE_NONE,
		     1,
		     SKETCH_TYPE_STROKE);
	g_signal_new("stroke-removed",
		     SKETCH_TYPE,
		     G_SIGNAL_RUN_LAST,
		     G_STRUCT_OFFSET(SketchClass, stroke_removed),
		     NULL, NULL,
		     g_cclosure_marshal_VOID__BOXED,
		     G_TYPE_NONE,
		     1,
		     SKETCH_TYPE_STROKE);

	klass->stroke_added   = s_stroke_added;
	klass->stroke_removed = s_stroke_removed;
}

static void
sketch_init(Sketch* self) {
	self->strokes = g_ptr_array_new();
	self->actions = gundo_sequence_new();
}

G_DEFINE_TYPE(Sketch, sketch, GTK_TYPE_OBJECT)


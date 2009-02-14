/* This file is part of gundo, a multilevel undo/redo facility for GTK+
 * 
 * AUTHORS
 *	Sven Herzberg		<herzi@gnome-de.org>
 *
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

#include "stroke.h"

#include <glib/gmem.h>
#include <gdk/gdktypes.h>

/*  Management of Stroke objects */

Stroke*
stroke_new(void) {
	Stroke *stroke = g_new0(Stroke, 1);
	
	stroke->points = g_array_new(FALSE, FALSE, sizeof(GdkPoint));
	stroke->is_visible = TRUE;

	return stroke;
}

Stroke*
stroke_copy(Stroke* src) {
	Stroke* copy = stroke_new();
	
	g_array_append_vals(copy->points, src->points->data, src->points->len);
	copy->is_visible = src->is_visible;

	return copy;
}

void
stroke_destroy(Stroke *stroke) {
	g_array_free(stroke->points, TRUE);
	g_free(stroke);
}

void
stroke_add_point(Stroke *stroke, gint16 x, gint16 y) {
    	GdkPoint pt;

	g_return_if_fail(stroke);
	g_return_if_fail(stroke->points);
	
    	pt.x = x;
    	pt.y = y;
    	g_array_append_val(stroke->points, pt);
}

GType
stroke_get_type(void) {
	static GType type = 0;

	if(!type) {
		type = g_boxed_type_register_static("Stroke",
						    (GBoxedCopyFunc)stroke_copy,
						    (GBoxedFreeFunc)stroke_destroy);
	}

	return type;
}


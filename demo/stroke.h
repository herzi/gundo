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

#ifndef STROKE_H
#define STROKE_H

#include <glib/garray.h>
#include <glib-object.h>

/*  A Stroke holds information about a single stroke of the user's "pen"
 *  on the drawing area.
 */
typedef struct _Stroke Stroke;

#define SKETCH_TYPE_STROKE (stroke_get_type())

struct _Stroke {
    GArray  * points;
    gboolean  is_visible;
};

GType   stroke_get_type (void);

Stroke* stroke_new      (void);
void    stroke_destroy  (Stroke *stroke);
Stroke* stroke_copy     (Stroke *src);

void    stroke_add_point(Stroke *stroke, gint16 x, gint16 y);

#endif /* !STROKE_H */


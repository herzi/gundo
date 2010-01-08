/* This file is part of gundo, a multilevel undo/redo facility for GTK+
 *
 * Copyright (C) 1999  Nat Price
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

#include <glib/gtypes.h>
#include <gtk/gtkmain.h>

#include "sketch.h"
#include "sketch-window.h"

int
main(int argc, char** argv) {
	gtk_init(&argc, &argv);
	gtk_widget_show_all(sketch_window_new());
	gtk_main();
	return 0;
}


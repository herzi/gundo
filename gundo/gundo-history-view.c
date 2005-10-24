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

#include <gundo/gundo-history-view.h>

#include <gundo/gundo.h>

#include "gobject-helpers.h"

G_DEFINE_IFACE_FULL(GundoHistoryView, gundo_history_view, G_TYPE_INTERFACE);

void
_gundo_history_view_install_properties(GObjectClass *go_class, gint prop_id_history) {
	g_object_class_override_property(go_class, prop_id_history, "history");
}

static void
gundo_history_view_class_init(gpointer iface) {
	g_object_interface_install_property(iface,
					    g_param_spec_object("history",
						    		"History",
								"The GundoHistory that's connected to this view",
								GUNDO_TYPE_HISTORY,
								G_PARAM_READWRITE));
}


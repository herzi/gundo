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

#include <gundo/gundo-history.h>

#include "gobject-helpers.h"

G_DEFINE_IFACE_FULL(GundoHistory, gundo_history, G_TYPE_INTERFACE);

/**
 * gundo_history_can_redo:
 * @self: a #GundoHistory
 * 
 * Queries whether the undo history contains any actions that can be redone.
 * 
 * Returns TRUE if there are actions that have already been undone, and
 * can therefore be redone, FALSE otherwise.
 */
gboolean
gundo_history_can_redo(GundoHistory* self) {
	g_return_val_if_fail(GUNDO_HISTORY_GET_CLASS(self)->can_redo, FALSE);

	return GUNDO_HISTORY_GET_CLASS(self)->can_redo(self);
}

/**
 * gundo_history_can_undo:
 * @self: a #GundoHistory
 * 
 * Queries whether the undo history contains any actions that can be undone.
 *
 * Returns TRUE if there are actions that can be undone, FALSE otherwise.
 */
gboolean
gundo_history_can_undo(GundoHistory* self) {
	g_return_val_if_fail(GUNDO_HISTORY_GET_CLASS(self)->can_undo, FALSE);

	return GUNDO_HISTORY_GET_CLASS(self)->can_undo(self);
}

/**
 * gundo_history_undo:
 * @self: a #GundoHistory
 *
 * Undoes the action at the end of the history.
 * 
 * <em>Prerequisites</em>: no group is being constructed && undo_sequence_can_undo(seq).
 */
void
gundo_history_undo(GundoHistory* self) {
	g_return_if_fail(GUNDO_HISTORY_GET_CLASS(self)->undo);

	GUNDO_HISTORY_GET_CLASS(self)->undo(self);
}

/* GInterface stuff */
void
gundo_history_install_properties(GObjectClass* go_class, guint id_undo, guint id_redo) {
	g_object_class_override_property(go_class, id_undo, "can-undo");
	g_object_class_override_property(go_class, id_redo, "can-redo");
}

static void
gundo_history_class_init(gpointer iface) {
	g_object_interface_install_property(iface,
					    g_param_spec_boolean("can-undo",
						    		 "can undo",
								 "Is it possible to undo any action",
								 FALSE,
								 G_PARAM_READABLE));
	g_object_interface_install_property(iface,
					    g_param_spec_boolean("can-redo",
						    		 "can redo",
								 "Is it possible to redo any action",
								 FALSE,
								 G_PARAM_READABLE));
}


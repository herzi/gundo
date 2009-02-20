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

/* Signal handling stuff */
static void
ghv_emit_notify_can_undo(GundoHistoryView* self, GParamSpec* ignored, GundoHistory* history) {
	g_object_ref(self);
	GUNDO_HISTORY_VIEW_GET_CLASS(self)->notify_can_undo(self, gundo_history_can_undo(history));
	g_object_unref(self);
}

static void
view_emit_notify_can_redo (GundoHistoryView* self,
                           GParamSpec      * ignored,
                           GundoHistory    * history)
{
  g_object_ref (self);
  GUNDO_HISTORY_VIEW_GET_CLASS (self)->notify_can_redo (self, gundo_history_can_redo (history));
  g_object_unref (self);
}

/**
 * gundo_history_view_register:
 * @self: a @GundoHistoryView
 * @history: a @GundoHistory
 *
 * Connects a history view to a history. This is part of the MVC design
 * pattern. Every notification handler that's specified in the class of the
 * view gets connected to the appropriate signals.
 *
 * Side effects:
 *
 * To ensure that we always have a valid connection the history gets referenced
 * here. To remove that reference, call @gundo_history_view_unregister on the
 * same history again.
 *
 * To make sure the view components are set to the correct states, the
 * notification handlers are executed once. There's no need to care for this
 * manually.
 */
void
gundo_history_view_register (GundoHistoryView* self,
                             GundoHistory    * history)
{
	g_object_ref(history);

	if(GUNDO_HISTORY_VIEW_GET_CLASS(self)->notify_can_undo) {
		g_signal_connect_swapped(history, "notify::can-undo",
					 G_CALLBACK(ghv_emit_notify_can_undo), self);
		ghv_emit_notify_can_undo(self, NULL, history);
	}

  if (GUNDO_HISTORY_VIEW_GET_CLASS (self)->notify_can_redo)
    {
      g_signal_connect_swapped (history, "notify::can-redo",
                                G_CALLBACK (view_emit_notify_can_redo), self);
      view_emit_notify_can_redo (self, NULL, history);
    }
}

void
gundo_history_view_unregister (GundoHistoryView* self,
                               GundoHistory    * history)
{
	if(GUNDO_HISTORY_VIEW_GET_CLASS(self)->notify_can_undo) {
		g_signal_handlers_disconnect_by_func(history, ghv_emit_notify_can_undo, self);
	}

  if (GUNDO_HISTORY_VIEW_GET_CLASS (self)->notify_can_redo)
    {
      g_signal_handlers_disconnect_by_func (history, view_emit_notify_can_redo, self);
    }

  g_object_unref(history);
}

/* GInterface stuff */

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


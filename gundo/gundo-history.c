/* This file is part of gundo, a multilevel undo/redo facility for GTK+
 * 
 * AUTHORS
 *	Sven Herzberg		<herzi@gnome-de.org>
 *
 * Copyright (C) 2005, 2009  Sven Herzberg
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

/**
 * GundoHistory:
 *
 * A #GObject instance that implements the #GundoHistoryIface interface.
 */

#include <gundo/gundo-history.h>

#include "gobject-helpers.h"

/**
 * GundoHistoryIface:
 * @changed: the signal slot for the <link linkend="GundoHistory-changed">changed</link> signal
 * @can_redo: the function slot for gundo_history_can_redo()
 * @can_undo: the function slot for gundo_history_can_undo()
 * @get_n_redos: the function slot for gundo_history_get_n_redos()
 * @get_n_undos: the function slot for gundo_history_get_n_undos()
 * @redo: the function slot for gundo_history_redo()
 * @undo: the function slot for gundo_history_undo()
 *
 * The %GTypeInterface for an undo/redo history.
 */
/* FIXME: be more verbose */

enum {
  SIGNAL_CHANGED,
  SIGNAL_REDO,
  SIGNAL_UNDO,
  N_SIGNALS
};

static guint signals[N_SIGNALS] = {0};

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
gundo_history_can_redo (GundoHistory* self)
{
  g_return_val_if_fail (GUNDO_IS_HISTORY (self), FALSE);
  g_return_val_if_fail (GUNDO_HISTORY_GET_IFACE (self)->can_redo, FALSE);

  return GUNDO_HISTORY_GET_IFACE (self)->can_redo (self);
}

/**
 * gundo_history_can_undo:
 * @self: a #GundoHistory
 *
 * Queries whether the undo history contains any actions that can be undone.
 *
 * Returns: %TRUE if there are actions that can be undone, %FALSE otherwise.
 */
gboolean
gundo_history_can_undo (GundoHistory* self)
{
  g_return_val_if_fail (GUNDO_IS_HISTORY (self), FALSE);
  g_return_val_if_fail (GUNDO_HISTORY_GET_IFACE (self)->can_undo, FALSE);

  return GUNDO_HISTORY_GET_IFACE (self)->can_undo (self);
}

/**
 * gundo_history_changed:
 * @self: a #GundoHistory
 *
 * Emit the #GundoHistory::changed signal to notify viewers about a change.
 */
void
gundo_history_changed (GundoHistory* self)
{
  g_return_if_fail (GUNDO_IS_HISTORY (self));

  g_signal_emit (self, signals[SIGNAL_CHANGED], 0);
}

/**
 * gundo_history_get_n_redos:
 * @self: a #GundoHistory
 *
 * Find out how many redoable actions are available.
 *
 * Returns: the number of redoable actions in @self.
 */
guint
gundo_history_get_n_redos (GundoHistory* self)
{
  g_return_val_if_fail (GUNDO_IS_HISTORY (self), 0);
  g_return_val_if_fail (GUNDO_HISTORY_GET_IFACE (self)->get_n_redos, 0);

  return GUNDO_HISTORY_GET_IFACE (self)->get_n_redos (self);
}

/**
 * gundo_history_get_n_undos:
 * @self: a #GundoHistory
 *
 * Query the number of undoable changes.
 *
 * Returns: the number of undoable changes.
 */
guint
gundo_history_get_n_undos (GundoHistory* self)
{
  g_return_val_if_fail (GUNDO_IS_HISTORY (self), 0);
  g_return_val_if_fail (GUNDO_HISTORY_GET_IFACE (self)->get_n_undos, 0);

  return GUNDO_HISTORY_GET_IFACE (self)->get_n_undos (self);
}

/**
 * gundo_history_redo:
 * @self: a #GundoHistory
 *
 * Redoes the last action that was undone.
 *
 * <emphasis>Prerequisitions</emphasis>: no group is being constructed && gundo_history_can_redo().
 */
void
gundo_history_redo (GundoHistory* self)
{
  g_return_if_fail (GUNDO_IS_HISTORY (self));
  g_return_if_fail (gundo_history_can_redo (self));

  g_signal_emit (self, signals[SIGNAL_REDO], 0);
}

/**
 * gundo_history_undo:
 * @self: a #GundoHistory
 *
 * Undoes the action at the end of the history.
 *
 * <emphasis>Prerequisites</emphasis>: no group is being constructed && gundo_history_can_undo().
 */
void
gundo_history_undo (GundoHistory* self)
{
  g_return_if_fail (GUNDO_IS_HISTORY (self));
  g_return_if_fail (gundo_history_can_undo (self));

  g_signal_emit (self, signals[SIGNAL_UNDO], 0);
}

/* GInterface stuff */
/**
 * gundo_history_install_properties:
 * @go_class: the #GObjectClass implementing #GundoHistory
 * @id_undo: the property id of the #GundoHistory:can-undo property
 * @id_redo: the property id of the #GundoHistory:can-redo property
 *
 * Install the properties of the #GundoHistory interface to @go_class.
 */
/* FIXME: rename go_class to object_class */
void
gundo_history_install_properties(GObjectClass* go_class, guint id_undo, guint id_redo) {
	g_object_class_override_property(go_class, id_undo, "can-undo");
	g_object_class_override_property(go_class, id_redo, "can-redo");
}

static void
gundo_history_class_init (gpointer iface)
{
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

  /**
   * GundoHistory::changed:
   *
   * This signal gets emitted when the history changes. Usually this signal is
   * emitted when users perform undoable tasks, undo tasks or redo tasks.
   * Classes implementing this interface can use gundo_history_changed() to emit
   * the signal.
   */
        signals[SIGNAL_CHANGED] = g_signal_new ("changed", G_TYPE_FROM_INTERFACE (iface),
                                                G_SIGNAL_ACTION | G_SIGNAL_RUN_FIRST,
                                                G_STRUCT_OFFSET (GundoHistoryIface, changed),
                                                NULL, NULL,
                                                g_cclosure_marshal_VOID__VOID,
                                                G_TYPE_NONE, 0);

  /**
   * GundoHistory::redo:
   *
   * This action signal can be emitted (usually via gundo_history_redo()) to
   * redo the latest undone action.
   */
        signals[SIGNAL_REDO] = g_signal_new ("redo", G_TYPE_FROM_INTERFACE (iface),
                                             G_SIGNAL_ACTION | G_SIGNAL_RUN_FIRST,
                                             G_STRUCT_OFFSET (GundoHistoryIface, redo),
                                             NULL, NULL,
                                             g_cclosure_marshal_VOID__VOID,
                                             G_TYPE_NONE, 0);

  /**
   * GundoHistory::undo:
   *
   * This action signal can be emitted (usually via gundo_history_undo()) to
   * undo the latest performed action.
   */
        signals[SIGNAL_UNDO] = g_signal_new ("undo", G_TYPE_FROM_INTERFACE (iface),
                                             G_SIGNAL_ACTION | G_SIGNAL_RUN_FIRST,
                                             G_STRUCT_OFFSET (GundoHistoryIface, undo),
                                             NULL, NULL,
                                             g_cclosure_marshal_VOID__VOID,
                                             G_TYPE_NONE, 0);
}

/* vim:set et: */

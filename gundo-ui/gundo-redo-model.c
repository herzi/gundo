/* This file is part of gundo
 *
 * AUTHORS
 *     Sven Herzberg  <herzi@lanedo.com>
 *
 * Copyright (C) 2009  Sven Herzberg
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

/* FIXME: rename to GUndoUIRedoStore */
/* FIXME: make sure we properly support groups */

/**
 * GUndoRedoModel:
 *
 * An implementation of a #GtkTreeModel that displays redoable tasks from a
 * #GundoHistory.
 */

#include "gundo-redo-model.h"

#include <string.h>

struct _GUndoRedoModelPrivate {
  gint emit_n_removes;
};

#define PRIV(i) (((GUndoRedoModel*)(i))->_private)

static void implement_gtk_tree_model (GtkTreeModelIface* iface);

G_DEFINE_TYPE_WITH_CODE (GUndoRedoModel, gundo_redo_model, GUNDO_TYPE_POPUP_MODEL,
                         G_IMPLEMENT_INTERFACE (GTK_TYPE_TREE_MODEL, implement_gtk_tree_model));

static void
gundo_redo_model_init (GUndoRedoModel* self)
{
  PRIV (self) = G_TYPE_INSTANCE_GET_PRIVATE (self, GUNDO_TYPE_REDO_MODEL, GUndoRedoModelPrivate);

  PRIV (self)->emit_n_removes = 0;
}

static void
history_changed_before (GundoHistory  * history,
                        GUndoRedoModel* self)
{
  GtkTreePath* path = gtk_tree_path_new_from_string ("0");
  for (PRIV (self)->emit_n_removes = gundo_history_get_n_redos (gundo_popup_model_get_history (GUNDO_POPUP_MODEL (self)));
       PRIV (self)->emit_n_removes;
       PRIV (self)->emit_n_removes--)
    {
      gtk_tree_model_row_deleted (GTK_TREE_MODEL (self), path);
    }
  gtk_tree_path_free (path);
}

static void
history_redo (GundoHistory  * history,
              GUndoRedoModel* self)
{
  GtkTreePath* path = gtk_tree_path_new_from_string ("0");
  gtk_tree_model_row_deleted (GTK_TREE_MODEL (self), path);
  gtk_tree_path_free (path);
}

static void
history_undo (GundoHistory  * history,
              GUndoRedoModel* self)
{
  GtkTreePath* path = gtk_tree_path_new_from_string ("0");
  GtkTreeIter  iter;

  if (gtk_tree_model_get_iter (GTK_TREE_MODEL (self), &iter, path))
    {
      gtk_tree_model_row_inserted (GTK_TREE_MODEL (self),
                                   path, &iter);
    }
  else
    {
      g_warning ("eeek!");
    }

  gtk_tree_path_free (path);
}

static void
model_finalize (GObject* object)
{
  g_signal_handlers_disconnect_by_func (gundo_popup_model_get_history (GUNDO_POPUP_MODEL (object)), history_changed_before, object);
  g_signal_handlers_disconnect_by_func (gundo_popup_model_get_history (GUNDO_POPUP_MODEL (object)), history_redo, object);
  g_signal_handlers_disconnect_by_func (gundo_popup_model_get_history (GUNDO_POPUP_MODEL (object)), history_undo, object);

  G_OBJECT_CLASS (gundo_redo_model_parent_class)->finalize (object);
}

static void
model_notify (GObject   * object,
              GParamSpec* pspec)
{
  if (!strcmp ("history", g_param_spec_get_name (pspec)))
    {
      g_signal_connect (gundo_popup_model_get_history (GUNDO_POPUP_MODEL (object)), "changed",
                        G_CALLBACK (history_changed_before), object);
      g_signal_connect_after (gundo_popup_model_get_history (GUNDO_POPUP_MODEL (object)), "redo",
                              G_CALLBACK (history_redo), object);
      g_signal_connect_after (gundo_popup_model_get_history (GUNDO_POPUP_MODEL (object)), "undo",
                              G_CALLBACK (history_undo), object);
    }

  if (G_OBJECT_CLASS (gundo_redo_model_parent_class)->notify)
    {
      G_OBJECT_CLASS (gundo_redo_model_parent_class)->notify (object, pspec);
    }
}

static void
gundo_redo_model_class_init (GUndoRedoModelClass* self_class)
{
  GObjectClass* object_class = G_OBJECT_CLASS (self_class);

  object_class->finalize = model_finalize;
  object_class->notify   = model_notify;

  g_type_class_add_private (self_class, sizeof (GUndoRedoModelPrivate));
}

/**
 * gundo_redo_model_new:
 * @history: a #GundoHistory
 *
 * Create a redo model for @history. This model will be able to display the
 * redoable actions of @history.
 *
 * Returns: a new instance of #GUndoRedoModel casted as a #GtkTreeModel.
 */
GtkTreeModel*
gundo_redo_model_new (GundoHistory* history)
{
  g_return_val_if_fail (GUNDO_IS_HISTORY (history), NULL);

  return g_object_new (GUNDO_TYPE_REDO_MODEL,
                       "history", history,
                       NULL);
}

static GtkTreeModelFlags
model_get_flags (GtkTreeModel* model)
{
  return GTK_TREE_MODEL_LIST_ONLY;
}

static gint
model_get_n_columns (GtkTreeModel* model)
{
  return POPUP_N_COLUMNS;
}

static GType
model_get_column_type (GtkTreeModel* model,
                       gint          column)
{
  static GType types[POPUP_N_COLUMNS] = {
    G_TYPE_STRING
  };

  g_return_val_if_fail (column >= 0, G_TYPE_INVALID);
  g_return_val_if_fail (column < model_get_n_columns (model), G_TYPE_INVALID);

  return types[column];
}

static void
model_get_value (GtkTreeModel* model,
                 GtkTreeIter * iter,
                 gint          column,
                 GValue      * value)
{
  g_value_init (value, model_get_column_type (model, column));

  switch (column)
    {
      case POPUP_COLUMN_TEXT:
        g_value_take_string (value, g_strdup_printf ("%d. Action", GPOINTER_TO_INT (iter->user_data) + 1));
        break;
      default:
        g_assert_not_reached ();
        break;
    }
}

static gboolean
model_iter_from_index (GtkTreeModel* model,
                       GtkTreeIter * iter,
                       gint          index)
{
  if (index < 0)
    return FALSE;

  if (index >= gundo_history_get_n_redos (gundo_popup_model_get_history (GUNDO_POPUP_MODEL (model))) - PRIV (model)->emit_n_removes)
    return FALSE;

  iter->user_data = GINT_TO_POINTER (index);

  return TRUE;
}

static gboolean
model_get_iter (GtkTreeModel* model,
                GtkTreeIter * iter,
                GtkTreePath * path)
{
  g_return_val_if_fail (gtk_tree_path_get_depth (path) == 1, FALSE);

  return model_iter_from_index (model, iter, gtk_tree_path_get_indices (path)[0]);
}

static gboolean
model_iter_next (GtkTreeModel* model,
                 GtkTreeIter * iter)
{
  return model_iter_from_index (model, iter, GPOINTER_TO_INT (iter->user_data) + 1);
}

static gboolean
model_iter_nth_child (GtkTreeModel* model,
                      GtkTreeIter * iter,
                      GtkTreeIter * parent,
                      gint          index)
{
  if (parent)
    return FALSE;

  return model_iter_from_index (model, iter, index);
}

static void
implement_gtk_tree_model (GtkTreeModelIface* iface)
{
  iface->get_flags       = model_get_flags;
  iface->get_n_columns   = model_get_n_columns;
  iface->get_column_type = model_get_column_type;

  iface->get_value       = model_get_value;

  iface->get_iter        = model_get_iter;
  iface->iter_next       = model_iter_next;
  iface->iter_nth_child  = model_iter_nth_child;
}


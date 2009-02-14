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

#include "gundo-popup-model.h"

/* GtkTreeIter format:
 * ===================
 * stamp:      unused
 * user_data:  GINT_TO_POINTER (<position>)
 * user_data2: unused
 * user_data3: unused
 *
 * "position" is the index counted from the current state:
 *   the next change gets 0, the one before 1, the last one n_changes() - 1
 */

struct _GUndoPopupModelPrivate {
  GundoHistory* history;
};

#define PRIV(i) (((GUndoPopupModel*)(i))->_private)

enum {
  PROP_0,
  PROP_HISTORY
};

static void implement_gtk_tree_model (GtkTreeModelIface* iface);

G_DEFINE_TYPE_WITH_CODE (GUndoPopupModel, gundo_popup_model, G_TYPE_OBJECT,
                         G_IMPLEMENT_INTERFACE (GTK_TYPE_TREE_MODEL, implement_gtk_tree_model));

static void
gundo_popup_model_init (GUndoPopupModel* self)
{
  PRIV (self) = G_TYPE_INSTANCE_GET_PRIVATE (self, GUNDO_TYPE_POPUP_MODEL, GUndoPopupModelPrivate);
}

static void
redo_callback (GundoHistory   * history,
               GUndoPopupModel* self)
{
  GtkTreePath* path = gtk_tree_path_new_from_string ("0");
  GtkTreeIter  iter;
  gtk_tree_model_get_iter     (GTK_TREE_MODEL (self),
                               &iter, path);
  gtk_tree_model_row_inserted (GTK_TREE_MODEL (self),
                               path, &iter);
  gtk_tree_path_free (path);
}

static void
undo_callback (GundoHistory   * history,
               GUndoPopupModel* self)
{
  GtkTreePath* path = gtk_tree_path_new_from_string ("0");
  gtk_tree_model_row_deleted (GTK_TREE_MODEL (self),
                              path);
  gtk_tree_path_free (path);
}

static void
model_finalize (GObject* object)
{
  g_signal_handlers_disconnect_by_func (PRIV (object)->history, redo_callback, object);
  g_signal_handlers_disconnect_by_func (PRIV (object)->history, undo_callback, object);
  g_object_unref (PRIV (object)->history);

  G_OBJECT_CLASS (gundo_popup_model_parent_class)->finalize (object);
}

static void
model_get_property (GObject   * object,
                    guint       prop_id,
                    GValue    * value,
                    GParamSpec* pspec)
{
  switch (prop_id)
    {
      case PROP_HISTORY:
        g_value_set_object (value, PRIV (object)->history);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}

static void
model_set_property (GObject     * object,
                    guint         prop_id,
                    GValue const* value,
                    GParamSpec  * pspec)
{
  switch (prop_id)
    {
      case PROP_HISTORY:
        g_return_if_fail (!PRIV (object)->history);

        PRIV (object)->history = g_value_dup_object (value);

        g_signal_connect_after (PRIV (object)->history, "changed",
                                G_CALLBACK (redo_callback), object);
        g_signal_connect_after (PRIV (object)->history, "redo",
                                G_CALLBACK (redo_callback), object);
        g_signal_connect_after (PRIV (object)->history, "undo",
                                G_CALLBACK (undo_callback), object);

        g_object_notify (object, "history");
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}

static void
gundo_popup_model_class_init (GUndoPopupModelClass* self_class)
{
  GObjectClass* object_class = G_OBJECT_CLASS (self_class);

  object_class->finalize     = model_finalize;
  object_class->get_property = model_get_property;
  object_class->set_property = model_set_property;

  g_object_class_install_property (object_class,
                                   PROP_HISTORY,
                                   g_param_spec_object ("history", "history", "history",
                                                        GUNDO_TYPE_HISTORY,
                                                        G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_type_class_add_private (self_class, sizeof (GUndoPopupModelPrivate));
}

GtkTreeModel*
gundo_popup_model_new (GundoHistory* history)
{
  g_return_val_if_fail (history, NULL);

  return g_object_new (GUNDO_TYPE_POPUP_MODEL,
                       "history", history,
                       NULL);
}

static GtkTreeModelFlags
model_get_flags (GtkTreeModel* model)
{
  return GTK_TREE_MODEL_ITERS_PERSIST | GTK_TREE_MODEL_LIST_ONLY;
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

static gboolean
model_iter_from_index (GtkTreeModel* model,
                       GtkTreeIter * iter,
                       guint         index)
{
  iter->user_data = GINT_TO_POINTER (index);

  return GPOINTER_TO_INT (iter->user_data) < gundo_history_get_n_changes (PRIV (model)->history);
}

static gboolean
model_get_iter (GtkTreeModel* model,
                GtkTreeIter * iter,
                GtkTreePath * path)
{
  gint* indices;

  g_return_val_if_fail (gtk_tree_path_get_depth (path) == 1, FALSE);

  indices = gtk_tree_path_get_indices (path);

  return model_iter_from_index (model, iter, indices[0]);
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
model_iter_next (GtkTreeModel* model,
                 GtkTreeIter * iter)
{
  return model_iter_from_index (model, iter, GPOINTER_TO_INT (iter->user_data) + 1);
}

static gboolean
model_iter_has_child (GtkTreeModel* model,
                      GtkTreeIter * iter)
{
  if (iter)
    return FALSE;

  return gundo_history_can_undo (PRIV (model)->history);
}

static gboolean
model_iter_nth_child (GtkTreeModel* model,
                      GtkTreeIter * iter,
                      GtkTreeIter * parent,
                      gint          n)
{
  if (parent)
    return FALSE;

  return model_iter_from_index (model, iter, n);
}

static void
implement_gtk_tree_model (GtkTreeModelIface* iface)
{
  iface->get_flags       = model_get_flags;
  iface->get_n_columns   = model_get_n_columns;
  iface->get_column_type = model_get_column_type;

  iface->get_iter        = model_get_iter;
  iface->get_value       = model_get_value;
  iface->iter_next       = model_iter_next;
  iface->iter_has_child  = model_iter_has_child;
  iface->iter_nth_child  = model_iter_nth_child;
}


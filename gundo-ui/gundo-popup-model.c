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
implement_gtk_tree_model (GtkTreeModelIface* iface)
{
  iface->get_n_columns   = model_get_n_columns;
  iface->get_column_type = model_get_column_type;
}


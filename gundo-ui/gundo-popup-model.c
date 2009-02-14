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

static void implement_gtk_tree_model (GtkTreeModelIface* iface);

G_DEFINE_TYPE_WITH_CODE (GUndoPopupModel, gundo_popup_model, G_TYPE_OBJECT,
                         G_IMPLEMENT_INTERFACE (GTK_TYPE_TREE_MODEL, implement_gtk_tree_model));

static void
gundo_popup_model_init (GUndoPopupModel* self)
{}

static void
gundo_popup_model_class_init (GUndoPopupModelClass* self_class)
{}

GtkTreeModel*
gundo_popup_model_new (GundoHistory* history)
{
  return g_object_new (GUNDO_TYPE_POPUP_MODEL,
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


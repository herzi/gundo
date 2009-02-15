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

#include "gundo-undo-model.h"

G_DEFINE_TYPE (GUndoUndoModel, gundo_undo_model, GUNDO_TYPE_POPUP_MODEL);

static void
gundo_undo_model_init (GUndoUndoModel* self)
{}

static void
gundo_undo_model_class_init (GUndoUndoModelClass* self_class)
{}

GtkTreeModel*
gundo_undo_model_new (GundoHistory* history)
{
  g_return_val_if_fail (GUNDO_IS_HISTORY (history), NULL);

  return g_object_new (GUNDO_TYPE_UNDO_MODEL,
                       "history", history,
                       NULL);
}


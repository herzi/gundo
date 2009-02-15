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

#ifndef GUNDO_UNDO_MODEL_H
#define GUNDO_UNDO_MODEL_H

#include <gtk/gtk.h>
#include <gundo-popup-model.h>

G_BEGIN_DECLS

typedef struct _GUndoUndoModel        GUndoUndoModel;
typedef struct _GUndoUndoModelPrivate GUndoUndoModelPrivate;
typedef struct _GUndoUndoModelClass   GUndoUndoModelClass;

#define GUNDO_TYPE_UNDO_MODEL         (gundo_undo_model_get_type ())
#define GUNDO_UNDO_MODEL(i)           (G_TYPE_CHECK_INSTANCE_CAST ((i), GUNDO_TYPE_UNDO_MODEL, GUndoUndoModel))
#define GUNDO_UNDO_MODEL_CLASS(c)     (G_TYPE_CHECK_CLASS_CAST ((c), GUNDO_TYPE_UNDO_MODEL, GUndoUndoModelClass))
#define GUNDO_IS_UNDO_MODEL(i)        (G_TYPE_CHECK_INSTANCE_TYPE ((i), GUNDO_TYPE_UNDO_MODEL))
#define GUNDO_IS_UNDO_MODEL_CLASS(c)  (G_TYPE_CHECK_CLASS_TYPE ((c), GUNDO_TYPE_UNDO_MODEL))
#define GUNDO_UNDO_MODEL_GET_CLASS(i) (G_TYPE_INSTANCE_GET_CLASS ((i), GUNDO_TYPE_UNDO_MODEL, GUndoUndoModelClass))

GType         gundo_undo_model_get_type (void);
GtkTreeModel* gundo_undo_model_new      (GundoHistory* history);

struct _GUndoUndoModel {
  GUndoPopupModel        base_instance;
  GUndoUndoModelPrivate* _private;
};

struct _GUndoUndoModelClass {
  GUndoPopupModelClass   base_class;
};

G_END_DECLS

#endif /* !GUNDO_UNDO_MODEL_H */

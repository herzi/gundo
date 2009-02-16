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

#ifndef GUNDO_REDO_MODEL_H
#define GUNDO_REDO_MODEL_H

#include <gtk/gtk.h>
#include <gundo-popup-model.h>

G_BEGIN_DECLS

typedef struct _GUndoRedoModel        GUndoRedoModel;
typedef struct _GUndoRedoModelPrivate GUndoRedoModelPrivate;
typedef struct _GUndoRedoModelClass   GUndoRedoModelClass;

#define GUNDO_TYPE_REDO_MODEL         (gundo_redo_model_get_type ())
#define GUNDO_REDO_MODEL(i)           (G_TYPE_CHECK_INSTANCE_CAST ((i), GUNDO_TYPE_REDO_MODEL, GUndoRedoModel))
#define GUNDO_REDO_MODEL_CLASS(c)     (G_TYPE_CHECK_CLASS_CAST ((c), GUNDO_TYPE_REDO_MODEL, GUndoRedoModelClass))
#define GUNDO_IS_REDO_MODEL(i)        (G_TYPE_CHECK_INSTANCE_TYPE ((i), GUNDO_TYPE_REDO_MODEL))
#define GUNDO_IS_REDO_MODEL_CLASS(c)  (G_TYPE_CHECK_CLASS_TYPE ((c), GUNDO_TYPE_REDO_MODEL))
#define GUNDO_REDO_MODEL_GET_CLASS(i) (G_TYPE_INSTANCE_GET_CLASS ((i), GUNDO_TYPE_REDO_MODEL, GUndoRedoModelClass))

GType         gundo_redo_model_get_type (void);
GtkTreeModel* gundo_redo_model_new      (GundoHistory* history);

struct _GUndoRedoModel {
  GUndoPopupModel        base_instance;
  GUndoRedoModelPrivate* _private;
};

struct _GUndoRedoModelClass {
  GUndoPopupModelClass   base_class;
};

G_END_DECLS

#endif /* !GUNDO_REDO_MODEL_H */

/* This file is part of gundo, a multilevel undo/redo facility for GTK+
 * 
 * AUTHORS
 *	Sven Herzberg		<herzi@gnome-de.org>
 *
 * Copyright (C) 2005,2009  Sven Herzberg
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

#ifndef GUNDO_POPUP_MODEL_H
#define GUNDO_POPUP_MODEL_H

#include <gtk/gtk.h>
#include <gundo.h>

G_BEGIN_DECLS

typedef struct _GUndoPopupModel        GUndoPopupModel;
typedef struct _GUndoPopupModelPrivate GUndoPopupModelPrivate;
typedef struct _GUndoPopupModelClass   GUndoPopupModelClass;

#define GUNDO_TYPE_POPUP_MODEL         (gundo_popup_model_get_type ())
#define GUNDO_POPUP_MODEL(i)           (G_TYPE_CHECK_INSTANCE_CAST ((i), GUNDO_TYPE_POPUP_MODEL, GUndoPopupModel))
#define GUNDO_POPUP_MODEL_CLASS(c)     (G_TYPE_CHECK_CLASS_CAST ((c), GUNDO_TYPE_POPUP_MODEL, GUndoPopupModelClass))
#define GUNDO_IS_POPUP_MODEL(i)        (G_TYPE_CHECK_INSTANCE_TYPE ((i), GUNDO_TYPE_POPUP_MODEL))
#define GUNDO_IS_POPUP_MODEL_CLASS(c)  (G_TYPE_CHECK_CLASS_TYPE ((c), GUNDO_TYPE_POPUP_MODEL))
#define GUNDO_POPUP_MODEL_GET_CLASS(i) (G_TYPE_INSTANCE_GET_CLASS ((i), GUNDO_TYPE_POPUP_MODEL, GUndoPopupModelClass))

enum {
	POPUP_COLUMN_TEXT,
	POPUP_N_COLUMNS
};

GType         gundo_popup_model_get_type (void);

struct _GUndoPopupModel {
  GObject                 base_instance;
  GUndoPopupModelPrivate* _private;
};

struct _GUndoPopupModelClass {
  GObjectClass            base_class;
};

G_END_DECLS

#endif /* !GUNDO_POPUP_MODEL_H */

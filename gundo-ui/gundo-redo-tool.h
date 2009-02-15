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

#ifndef GUNDO_REDO_TOOL_H
#define GUNDO_REDO_TOOL_H

#include <gundo-tool.h>

G_BEGIN_DECLS

typedef struct _GUndoRedoTool        GUndoRedoTool;
typedef struct _GUndoRedoToolPrivate GUndoRedoToolPrivate;
typedef struct _GUndoRedoToolClass   GUndoRedoToolClass;

#define GUNDO_TYPE_REDO_TOOL         (gundo_redo_tool_get_type ())
#define GUNDO_REDO_TOOL(i)           (G_TYPE_CHECK_INSTANCE_CAST ((i), GUNDO_TYPE_REDO_TOOL, GUndoRedoTool))
#define GUNDO_REDO_TOOL_CLASS(c)     (G_TYPE_CHECK_CLASS_CAST ((c), GUNDO_TYPE_REDO_TOOL, GUndoRedoToolClass))
#define GUNDO_IS_REDO_TOOL(i)        (G_TYPE_CHECK_INSTANCE_TYPE ((i), GUNDO_TYPE_REDO_TOOL))
#define GUNDO_IS_REDO_TOOL_CLASS(c)  (G_TYPE_CHECK_CLASS_TYPE ((c), GUNDO_TYPE_REDO_TOOL))
#define GUNDO_REDO_TOOL_GET_CLASS(i) (G_TYPE_INSTANCE_GET_CLASS ((i), GUNDO_TYPE_REDO_TOOL, GUndoRedoToolClass))

GType        gundo_redo_tool_get_type (void);
GtkToolItem* gundo_redo_tool_new      (void);

struct _GUndoRedoTool {
  GUndoTool             base_instance;
  GUndoRedoToolPrivate* _private;
};

struct _GUndoRedoToolClass {
  GUndoToolClass        base_class;
};

G_END_DECLS

#endif /* !GUNDO_REDO_TOOL_H */

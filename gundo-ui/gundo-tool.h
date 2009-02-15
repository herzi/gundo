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

#ifndef GUNDO_TOOL_H
#define GUNDO_TOOL_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

typedef struct _GUndoTool        GUndoTool;
typedef struct _GUndoToolPrivate GUndoToolPrivate;
typedef struct _GUndoToolClass   GUndoToolClass;

#define GUNDO_TYPE_TOOL         (gundo_tool_get_type ())
#define GUNDO_TOOL(i)           (G_TYPE_CHECK_INSTANCE_CAST ((i), GUNDO_TYPE_TOOL, GUndoTool))
#define GUNDO_TOOL_CLASS(c)     (G_TYPE_CHECK_CLASS_CAST ((c), GUNDO_TYPE_TOOL, GUndoToolClass))
#define GUNDO_IS_TOOL(i)        (G_TYPE_CHECK_INSTANCE_TYPE ((i), GUNDO_TYPE_TOOL))
#define GUNDO_IS_TOOL_CLASS(c)  (G_TYPE_CHECK_CLASS_TYPE ((c), GUNDO_TYPE_TOOL))
#define GUNDO_TOOL_GET_CLASS(i) (G_TYPE_INSTANCE_GET_CLASS ((i), GUNDO_TYPE_TOOL, GUndoToolClass))

GType gundo_tool_get_type (void);

struct _GUndoTool {
  GtkToolItem       base_instance;
  GUndoToolPrivate* _private;
};

struct _GUndoToolClass {
  GtkToolItemClass  base_class;
};

G_END_DECLS

#endif /* !GUNDO_TOOL_H */

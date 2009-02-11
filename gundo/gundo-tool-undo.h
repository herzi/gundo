/* This file is part of gundo, a multilevel undo/redo facility for GTK+
 * 
 * AUTHORS
 *	Sven Herzberg		<herzi@gnome-de.org>
 *
 * Copyright (C) 2005		Sven Herzberg
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

#ifndef GUNDO_TOOL_UNDO_H
#define GUNDO_TOOL_UNDO_H

#include <gtk/gtktoolitem.h>
#include <gundo-sequence.h>

typedef struct _GundoToolUndo GundoToolUndo;
typedef GtkToolItemClass GundoToolUndoClass;

#define GUNDO_TYPE_TOOL_UNDO         (gundo_tool_undo_get_type())
#define GUNDO_TOOL_UNDO(o)           (G_TYPE_CHECK_INSTANCE_CAST((o), GUNDO_TYPE_TOOL_UNDO, GundoToolUndo))
#define GUNDO_TOOL_UNDO_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), GUNDO_TYPE_TOOL_UNDO, GundoToolUndoClass))
#define GUNDO_IS_TOOL_UNDO(o)        (G_TYPE_CHECK_INSTANCE_TYPE((o), GUNDO_TYPE_TOOL_UNDO))
#define GUNDO_IS_TOOL_UNDO_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE((k), GUNDO_TYPE_TOOL_UNDO))
#define GUNDO_TOOL_UNDO_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS((o), GUNDO_TYPE_TOOL_UNDO, GundoToolUndoClass))

GType        gundo_tool_undo_get_type(void);
GtkToolItem* gundo_tool_undo_new     (void);
void         gundo_tool_undo_connect (GundoToolUndo* self,
				      GundoSequence* sequence);

struct _GundoToolUndo {
	GtkToolItem    tool_item;
	GtkWidget    * icon_button;
	GtkWidget    * arrow_button;
	GundoSequence* sequence;
};

#endif /* !GUNDO_TOOL_UNDO_H */

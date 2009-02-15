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

#include "gundo-redo-tool.h"

G_DEFINE_TYPE (GUndoRedoTool, gundo_redo_tool, GUNDO_TYPE_TOOL);

static void
gundo_redo_tool_init (GUndoRedoTool* self)
{}

static void
gundo_redo_tool_class_init (GUndoRedoToolClass* self_class)
{}

GtkToolItem*
gundo_redo_tool_new (void)
{
  return g_object_new (GUNDO_TYPE_REDO_TOOL,
                       NULL);
}

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

#include <string.h>

#include "gundo-redo-model.h"

static void implement_gundo_history_view (GundoHistoryViewIface* iface);

G_DEFINE_TYPE_WITH_CODE (GUndoRedoTool, gundo_redo_tool, GUNDO_TYPE_TOOL,
                         G_IMPLEMENT_INTERFACE (GUNDO_TYPE_HISTORY_VIEW, implement_gundo_history_view));

static void
gundo_redo_tool_init (GUndoRedoTool* self)
{}

static void
redo_notify (GObject   * object,
             GParamSpec* pspec)
{
  if (!strcmp ("history", g_param_spec_get_name (pspec)))
    {
      GtkTreeModel* model = gundo_redo_model_new (gundo_tool_get_history (GUNDO_TOOL (object)));
      gundo_tool_set_model (GUNDO_TOOL (object), model);
      g_object_unref (model);
    }

  if (G_OBJECT_CLASS (gundo_redo_tool_parent_class)->notify)
    {
      G_OBJECT_CLASS (gundo_redo_tool_parent_class)->notify (object, pspec);
    }
}

static void
redo_clicked (GUndoTool* tool)
{
  gundo_history_redo (gundo_tool_get_history (tool));
}

static void
gundo_redo_tool_class_init (GUndoRedoToolClass* self_class)
{
  GObjectClass  * object_class = G_OBJECT_CLASS (self_class);
  GUndoToolClass* tool_class = GUNDO_TOOL_CLASS (self_class);

  object_class->notify = redo_notify;

  tool_class->clicked  = redo_clicked;
}

GtkToolItem*
gundo_redo_tool_new (void)
{
  return g_object_new (GUNDO_TYPE_REDO_TOOL,
                       "stock-id", GTK_STOCK_REDO,
                       NULL);
}

static gpointer parent_view = NULL;

static void
tool_notify_can_redo (GundoHistoryView* view,
                      gboolean          can_redo)
{
  gtk_widget_set_sensitive(GTK_WIDGET(view), can_redo);

  if (parent_view && ((GundoHistoryViewIface*)parent_view)->notify_can_redo)
    {
      ((GundoHistoryViewIface*)parent_view)->notify_can_undo (view, can_redo);
    }
}

static void
implement_gundo_history_view (GundoHistoryViewIface* iface)
{
  parent_view = g_type_interface_peek_parent (iface);

  iface->notify_can_redo = tool_notify_can_redo;
}


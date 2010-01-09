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

/**
 * GundoToolUndo:
 *
 * A #GtkToolItem to add undoable actions to a #GtkToolbar.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "gundo-undo-tool.h"

#include <string.h>
#include <gtk/gtkarrow.h>
#include <gtk/gtkbutton.h>
#include <gtk/gtkcellrenderertext.h>
#include <gtk/gtkframe.h>
#include <gtk/gtkhbox.h>
#include <gtk/gtklabel.h>
#include <gtk/gtkscrolledwindow.h>
#include <gtk/gtkstock.h>
#include <gtk/gtktogglebutton.h>
#include <gtk/gtktreeview.h>

#include "gundo-ui.h"

static void gtu_history_view_init (GundoHistoryViewIface* iface);

G_DEFINE_TYPE_WITH_CODE (GundoToolUndo, gundo_tool_undo, GUNDO_TYPE_TOOL,
                         G_IMPLEMENT_INTERFACE (GUNDO_TYPE_HISTORY_VIEW, gtu_history_view_init));

/**
 * gundo_tool_undo_new:
 *
 * Get a new #GundoToolUndo.
 *
 * Returns: a new #GundoToolUndo casted as a #GtkToolItem (for easy packing
 * into a #GtkToolbar).
 */
GtkToolItem*
gundo_tool_undo_new (void)
{
  return g_object_new(GUNDO_TYPE_TOOL_UNDO,
                      "stock-id", GTK_STOCK_UNDO,
                      NULL);
}

static void
undo_notify (GObject   * object,
             GParamSpec* pspec)
{
  if (!strcmp ("history", g_param_spec_get_name (pspec)))
    {
      GundoToolUndo* self = GUNDO_TOOL_UNDO (object);
      GtkTreeModel * model = gundo_undo_model_new (gundo_tool_get_history (GUNDO_TOOL (self)));
      gundo_tool_set_model (GUNDO_TOOL (object), model);
      g_object_unref (model);
    }

  G_OBJECT_CLASS (gundo_tool_undo_parent_class)->notify (object, pspec);
}

static void
undo_clicked (GUndoTool* tool)
{
  gundo_history_undo (gundo_tool_get_history (tool));
}

static void
gundo_tool_undo_class_init (GundoToolUndoClass* self_class)
{
  GObjectClass  * object_class = G_OBJECT_CLASS (self_class);
  GUndoToolClass* tool_class   = GUNDO_TOOL_CLASS (self_class);

  object_class->notify = undo_notify;

  // FIXME: listen to the toolbar_reconfigured signal

  tool_class->clicked  = undo_clicked;
}

static void
gundo_tool_undo_init (GundoToolUndo* self)
{
}

/* GundoHistoryView interface */

static gpointer parent_view = NULL;

static void
gtu_notify_can_undo (GundoHistoryView* view,
                     gboolean          can_undo)
{
  gtk_widget_set_sensitive(GTK_WIDGET(view), can_undo);

  if (parent_view && ((GundoHistoryViewIface*)parent_view)->notify_can_undo)
    {
      ((GundoHistoryViewIface*)parent_view)->notify_can_undo (view, can_undo);
    }
}

static void
gtu_history_view_init (GundoHistoryViewIface* iface)
{
  parent_view = g_type_interface_peek_parent (iface);

  iface->notify_can_undo = gtu_notify_can_undo;
}


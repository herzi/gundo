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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "gundo-undo-tool.h"

#include <glib/gi18n-lib.h>
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

#include "gtk-helpers.h"

static void gtu_history_view_init (GundoHistoryViewIface* iface);

G_DEFINE_TYPE_WITH_CODE (GundoToolUndo, gundo_tool_undo, GUNDO_TYPE_TOOL,
                         G_IMPLEMENT_INTERFACE (GUNDO_TYPE_HISTORY_VIEW, gtu_history_view_init));

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
      GtkTreeModel * model;

      g_return_if_fail (self->popup_tree);

      model = gundo_undo_model_new (gundo_tool_get_history (GUNDO_TOOL (self)));
      gtk_tree_view_set_model (GTK_TREE_VIEW (self->popup_tree),
                               model);
      g_object_unref (model);
    }

  if (G_OBJECT_CLASS (gundo_tool_undo_parent_class)->notify)
    {
      G_OBJECT_CLASS (gundo_tool_undo_parent_class)->notify (object, pspec);
    }
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
gtu_toggle_list (GundoToolUndo* self,
                 gboolean       show_menu)
{
  gtk_window_set_screen (GTK_WINDOW (self->popup_window),
                         gtk_widget_get_screen (GTK_WIDGET (self)));

        if (show_menu)
          {
		gint       x, y, w, h;
		gint       max_x, max_y;
		gint       pop_w, pop_h;
		GdkScreen* screen;

            gtk_widget_get_extends (GTK_WIDGET (self), &x, &y, &w, &h);
		screen = gtk_widget_get_screen(self->popup_window);
		max_x = gdk_screen_get_width(screen);
		max_y = gdk_screen_get_height(screen);
                // FIXME: get the size of the window"

		//gtk_widget_get_extends(self->popup_window, NULL, NULL, &pop_w, &pop_h);
		pop_w = w;
		pop_h = h;

		if(x+pop_w <= max_x) {
			// leave x as it is
		} else {
			//x = max_x - pop_w;
		}
		if(y+h <= max_y) {
			y += h;
		} else {
                        // FIXME: put the popup over the tool item"
			y -= pop_h;
		}

		gtk_widget_show(self->popup_window);
		gtk_window_move(GTK_WINDOW(self->popup_window), x, y);
          }
        else
          {
		gtk_widget_hide(self->popup_window);
          }
}

static void
gundo_tool_undo_init (GundoToolUndo* self)
{
  GtkWidget* frame;
  GtkWidget* scrolled;
  GtkTreeViewColumn* column;

  self->popup_window = gtk_window_new(GTK_WINDOW_POPUP);
  frame = gtk_frame_new(NULL);
  gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_OUT);
  gtk_container_add(GTK_CONTAINER(self->popup_window), frame);
  self->popup_tree = gtk_tree_view_new();
  column = gtk_tree_view_column_new_with_attributes(_("Undo Actions"),
                  gtk_cell_renderer_text_new(),
                  "text", POPUP_COLUMN_TEXT,
                  NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (self->popup_tree),
                               column);
  scrolled = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled),
                                  GTK_POLICY_NEVER,
                                  GTK_POLICY_ALWAYS);
  gtk_container_add(GTK_CONTAINER(scrolled), self->popup_tree);
  gtk_container_add(GTK_CONTAINER(frame), scrolled);
  gtk_widget_show_all(frame);

  g_signal_connect (self, "show-menu",
                    G_CALLBACK (gtu_toggle_list), NULL);
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


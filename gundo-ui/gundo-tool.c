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

/**
 * GUndoTool:
 *
 * A shared base class for #GUndoRedoTool and #GundoToolUndo.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "gtk-helpers.h"
#include "gundo-popup-model.h"
#include "gundo-tool.h"

#include <glib/gi18n-lib.h>

struct _GUndoToolPrivate {
  GundoHistory * history;

  /* FIXME: drop this... */
  gchar        * stock_id;

  GtkWidget    * hbox;
  GtkWidget    * icon_button;
  GtkWidget    * arrow_button;

  GtkWidget    * popup_window;
  GtkWidget    * popup_tree;
};

#define PRIV(i) (((GUndoTool*)(i))->_private)

enum {
  PROP_0,
  PROP_HISTORY,
  PROP_MODEL,
  PROP_STOCK_ID
};

enum {
  SIGNAL_CLICKED,
  SIGNAL_SHOW_MENU,
  N_SIGNALS
};

static guint signals[N_SIGNALS] = {0};

G_DEFINE_ABSTRACT_TYPE_WITH_CODE (GUndoTool, gundo_tool, GTK_TYPE_TOOL_ITEM,
                                  G_IMPLEMENT_INTERFACE (GUNDO_TYPE_HISTORY_VIEW, NULL));

static void
icon_clicked (GtkButton* icon,
              gpointer   user_data)
{
  g_signal_emit (GUNDO_TOOL (user_data), signals[SIGNAL_CLICKED], 0);
}

static void
icon_raise (GtkWidget* arrow,
            GtkButton* icon)
{
  gtk_button_set_relief(icon, GTK_RELIEF_NORMAL);
}

static void
icon_sink (GtkWidget* arrow,
           GtkButton* icon)
{
  gtk_button_set_relief(icon, GTK_RELIEF_NONE);
}

static void
arrow_toggled (GtkToggleButton* arrow_button,
               gpointer         user_data)
{
  GUndoTool* self = user_data;

  gtk_window_set_screen (GTK_WINDOW (PRIV (self)->popup_window),
                         gtk_widget_get_screen (GTK_WIDGET (self)));

  if (gtk_toggle_button_get_active (arrow_button))
    {
		gint       x, y, w, h;
		gint       max_x, max_y;
		gint       pop_w, pop_h;
		GdkScreen* screen;

      gtk_widget_get_extends (GTK_WIDGET (self), &x, &y, &w, &h);
      screen = gtk_widget_get_screen (PRIV (self)->popup_window);
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

      gtk_widget_show (PRIV (self)->popup_window);
      gtk_window_move (GTK_WINDOW (PRIV (self)->popup_window), x, y);

      /* FIXME: add some proper grabs to the popup, so it behaves like it should */
    }
  else
    {
      gtk_widget_hide (PRIV (self)->popup_window);
    }
}

static void
scrolled_window_size_request (GtkWidget     * widget,
                              GtkRequisition* requisition)
{
  GdkRectangle  rect;
  GtkTreeView * treeview = GTK_TREE_VIEW (gtk_bin_get_child (GTK_BIN (widget)));
  GtkTreePath * path = gtk_tree_path_new_from_indices (0, -1);

  gtk_tree_view_get_background_area (treeview, path, NULL, &rect);

  gtk_tree_view_convert_bin_window_to_widget_coords (treeview, 0, 0, &rect.x, NULL);
  requisition->height = MAX (requisition->height, rect.x + 6 * rect.height); /* default to six rows */

  gtk_tree_path_free (path);
}

static void
gundo_tool_init (GUndoTool* self)
{
  GtkWidget* frame;
  GtkWidget* scrolled;
  GtkTreeViewColumn* column;

  PRIV (self) = G_TYPE_INSTANCE_GET_PRIVATE (self, GUNDO_TYPE_TOOL, GUndoToolPrivate);

  PRIV (self)->hbox = gtk_hbox_new (FALSE, 0);

  PRIV (self)->icon_button = gtk_button_new_from_stock (NULL);
  icon_sink (NULL, GTK_BUTTON (PRIV (self)->icon_button));
  g_signal_connect (PRIV (self)->icon_button, "clicked",
                    G_CALLBACK (icon_clicked), self);
  gtk_box_pack_start (GTK_BOX (PRIV (self)->hbox), PRIV (self)->icon_button,
                      FALSE, FALSE, 0);

  PRIV (self)->arrow_button = gtk_toggle_button_new ();
  gtk_button_set_relief (GTK_BUTTON (PRIV (self)->arrow_button), GTK_RELIEF_NONE);
  g_signal_connect_after   (PRIV (self)->arrow_button, "enter",
                            G_CALLBACK (icon_raise), PRIV (self)->icon_button);
  g_signal_connect_after   (PRIV (self)->arrow_button, "leave",
                            G_CALLBACK (icon_sink), PRIV (self)->icon_button);
  g_signal_connect (PRIV (self)->arrow_button, "toggled",
                    G_CALLBACK (arrow_toggled), self);
  gtk_container_add (GTK_CONTAINER (PRIV (self)->arrow_button),
                     gtk_arrow_new (GTK_ARROW_DOWN, GTK_SHADOW_IN));
  gtk_box_pack_start (GTK_BOX (PRIV (self)->hbox), PRIV (self)->arrow_button,
                      FALSE, FALSE, 0);

  gtk_widget_show_all (PRIV (self)->hbox);
  gtk_container_add (GTK_CONTAINER (self), PRIV (self)->hbox);

  PRIV (self)->popup_window = gtk_window_new (GTK_WINDOW_POPUP);
  frame = gtk_frame_new(NULL);
  gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_OUT);
  gtk_container_add (GTK_CONTAINER (PRIV (self)->popup_window), frame);
  PRIV (self)->popup_tree = gtk_tree_view_new();
  column = gtk_tree_view_column_new_with_attributes(_("Undo Actions"),
                  gtk_cell_renderer_text_new(),
                  "text", POPUP_COLUMN_TEXT,
                  NULL);
  /* FIXME: set some rubberbanding selection, close to the font selection in AbiWord */
  gtk_tree_view_append_column (GTK_TREE_VIEW (PRIV (self)->popup_tree),
                               column);
  scrolled = gtk_scrolled_window_new(NULL, NULL);
  g_signal_connect (scrolled, "size-request",
                    G_CALLBACK (scrolled_window_size_request), NULL);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled),
                                  GTK_POLICY_NEVER,
                                  GTK_POLICY_ALWAYS);
  gtk_container_add (GTK_CONTAINER (scrolled), PRIV (self)->popup_tree);
  gtk_container_add(GTK_CONTAINER(frame), scrolled);
  gtk_widget_show_all(frame);
}

static void
tool_finalize (GObject* object)
{
  if (PRIV (object)->history)
    {
      gundo_history_view_unregister (GUNDO_HISTORY_VIEW (object), PRIV (object)->history);
      g_object_unref (PRIV (object)->history);
      PRIV (object)->history = NULL;
    }

  g_free (PRIV (object)->stock_id);

  if (G_OBJECT_CLASS (gundo_tool_parent_class)->finalize)
    {
      G_OBJECT_CLASS (gundo_tool_parent_class)->finalize (object);
    }
}

static void
tool_get_property (GObject   * object,
                   guint       prop_id,
                   GValue    * value,
                   GParamSpec* pspec)
{
  switch (prop_id)
    {
      case PROP_HISTORY:
        g_value_set_object (value, gundo_tool_get_history (GUNDO_TOOL (object)));
        break;
      case PROP_STOCK_ID:
        g_value_set_string (value, gundo_tool_get_stock_id (GUNDO_TOOL (object)));
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}

static void
tool_set_property (GObject     * object,
                   guint         prop_id,
                   GValue const* value,
                   GParamSpec  * pspec)
{
  switch (prop_id)
    {
      case PROP_HISTORY:
        gundo_tool_set_history (GUNDO_TOOL (object), g_value_get_object (value));
        break;
      case PROP_MODEL:
        gundo_tool_set_model (GUNDO_TOOL (object), g_value_get_object (value));
        break;
      case PROP_STOCK_ID:
        g_free (PRIV (object)->stock_id);
        PRIV (object)->stock_id = g_value_dup_string (value);
        gtk_button_set_label (GTK_BUTTON (PRIV (object)->icon_button),
                              PRIV (object)->stock_id);
        g_object_notify (object, "stock-id");
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}

static void
gundo_tool_class_init (GUndoToolClass* self_class)
{
  GObjectClass* object_class = G_OBJECT_CLASS (self_class);

  object_class->finalize     = tool_finalize;
  object_class->get_property = tool_get_property;
  object_class->set_property = tool_set_property;

  g_object_class_install_property (object_class, PROP_MODEL,
                                   g_param_spec_object ("model", "model", "model",
                                                        GTK_TYPE_TREE_MODEL, G_PARAM_WRITABLE));
  g_object_class_install_property (object_class, PROP_STOCK_ID,
                                   g_param_spec_string ("stock-id", "stock-id", "stock-id",
                                                        NULL, G_PARAM_READWRITE));

  /**
   * GUndoTool::clicked:
   *
   * This signal gets emitted when the #GtkButton of the #GUndoTool gets
   * clicked. This happens when a user wants to undo/redo a task.
   */
  signals[SIGNAL_CLICKED]   = g_signal_new ("clicked", G_OBJECT_CLASS_TYPE (self_class),
                                            G_SIGNAL_RUN_FIRST, G_STRUCT_OFFSET (GUndoToolClass, clicked),
                                            NULL, NULL,
                                            g_cclosure_marshal_VOID__VOID,
                                            G_TYPE_NONE, 0);

  /* FIXME: tool_item_class->create_menu_proxy */
  /* FIXME: tool_item_class->toolbar_reconfigured */

  _gundo_history_view_install_properties(object_class, PROP_HISTORY);

  g_type_class_add_private (self_class, sizeof (GUndoToolPrivate));
}

/**
 * gundo_tool_get_history:
 * @self: a #GUndoTool
 *
 * Get the #GundoHistory of @self.
 *
 * Returns: the #GundoHistory currently assigned to @self.
 */
GundoHistory*
gundo_tool_get_history (GUndoTool* self)
{
  g_return_val_if_fail (GUNDO_IS_TOOL (self), NULL);

  return PRIV (self)->history;
}

/**
 * gundo_tool_get_stock_id:
 * @self: a #GUndoTool
 *
 * Get the stock id of the tool. It is used to choose the #GtkButton's label
 * and icon.
 *
 * Returns: The stock id, usually %GTK_STOCK_REDO or %GTK_STOCK_UNDO.
 */
gchar const*
gundo_tool_get_stock_id (GUndoTool* self)
{
  g_return_val_if_fail (GUNDO_IS_TOOL (self), NULL);

  return PRIV (self)->stock_id;
}

/**
 * gundo_tool_set_history:
 * @self: a #GUndoTool
 * @history: a #GundoHistory
 *
 * Assign @history to @self. The #GUndoTool will now display it's state
 * according to the #GundoHistory.
 */
void
gundo_tool_set_history (GUndoTool   * self,
                        GundoHistory* history)
{
  g_return_if_fail (GUNDO_IS_TOOL (self));
  g_return_if_fail (!history || GUNDO_IS_HISTORY (history));

  if(PRIV (self)->history) {
          gundo_history_view_unregister(GUNDO_HISTORY_VIEW(self), PRIV (self)->history);
          g_object_unref(PRIV (self)->history);
          PRIV (self)->history = NULL;
  }

  if(history) {
          PRIV (self)->history = g_object_ref(history);
          gundo_history_view_register(GUNDO_HISTORY_VIEW(self), PRIV (self)->history);
  } else {
          gtk_widget_set_sensitive(GTK_WIDGET(self), FALSE);
  }

  g_object_notify(G_OBJECT(self), "history");
}

/**
 * gundo_tool_set_model:
 * @self: a #GUndoTool
 * @model: a #GtkTreeModel
 *
 * Sets the model that's used for display in the popup window.
 */
void
gundo_tool_set_model (GUndoTool   * self,
                      GtkTreeModel* model)
{
  g_return_if_fail (GUNDO_IS_TOOL (self));
  g_return_if_fail (!model || GTK_IS_TREE_MODEL (model));

  gtk_tree_view_set_model (GTK_TREE_VIEW (PRIV (self)->popup_tree),
                           model);

  g_object_notify (G_OBJECT (self), "model");
}


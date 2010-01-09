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

#ifndef GUNDO_HISTORY_VIEW_H
#define GUNDO_HISTORY_VIEW_H

#include <gundo-history.h>

G_BEGIN_DECLS

typedef struct _GundoHistoryView      GundoHistoryView;
typedef struct _GundoHistoryViewIface GundoHistoryViewIface;

#define GUNDO_TYPE_HISTORY_VIEW         (gundo_history_view_get_type())
#define GUNDO_HISTORY_VIEW(i)           (G_TYPE_CHECK_INSTANCE_CAST((i), GUNDO_TYPE_HISTORY_VIEW, GundoHistoryView))
#define GUNDO_IS_HISTORY_VIEW(i)        (G_TYPE_CHECK_INSTANCE_TYPE((i), GUNDO_TYPE_HISTORY_VIEW))
#define GUNDO_HISTORY_VIEW_GET_CLASS(i) (G_TYPE_INSTANCE_GET_INTERFACE((i), GUNDO_TYPE_HISTORY_VIEW, GundoHistoryViewIface))

GType gundo_history_view_get_type(void);

void gundo_history_view_register  (GundoHistoryView* self,
				   GundoHistory    * history);
void gundo_history_view_unregister(GundoHistoryView* self,
				   GundoHistory    * history);

void _gundo_history_view_install_properties(GObjectClass *go_class,
					    gint prop_id_history);

struct _GundoHistoryViewIface
  {
    /*< private >*/
	GTypeInterface base_interface;

    /*< public >*/
	/* vtable */
	void (*notify_can_redo) (GundoHistoryView* self,
				 gboolean          can_redo);
	void (*notify_can_undo) (GundoHistoryView* self,
				 gboolean          can_undo);
};

G_END_DECLS

#endif /* !GUNDO_HISTORY_VIEW_H */


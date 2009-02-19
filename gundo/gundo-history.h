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

#ifndef GUNDO_HISTORY_H
#define GUNDO_HISTORY_H

#include <glib-object.h>

G_BEGIN_DECLS

typedef struct _GundoHistory GundoHistory;
typedef struct _GundoHistoryIface GundoHistoryIface;

#define GUNDO_TYPE_HISTORY         (gundo_history_get_type())
#define GUNDO_HISTORY(i)           (G_TYPE_CHECK_INSTANCE_CAST((i), GUNDO_TYPE_HISTORY, GundoHistory))
#define GUNDO_IS_HISTORY(i)        (G_TYPE_CHECK_INSTANCE_TYPE((i), GUNDO_TYPE_HISTORY))
#define GUNDO_HISTORY_GET_IFACE(i) (G_TYPE_INSTANCE_GET_INTERFACE((i), GUNDO_TYPE_HISTORY, GundoHistoryIface))

GType    gundo_history_get_type(void);

gboolean gundo_history_can_redo      (GundoHistory* self);
gboolean gundo_history_can_undo      (GundoHistory* self);
void     gundo_history_changed       (GundoHistory* self);
guint    gundo_history_get_n_redos   (GundoHistory* self);
guint    gundo_history_get_n_undos   (GundoHistory* self);
void     gundo_history_redo          (GundoHistory* self);
void     gundo_history_undo          (GundoHistory* self);

void     gundo_history_install_properties(GObjectClass* go_class,
					  guint id_undo,
					  guint id_redo);

struct _GundoHistoryIface {
	GTypeInterface base_interface;

        /* signals */
        void     (*changed)       (GundoHistory* self);

        /* vtable */
        gboolean (*can_redo)      (GundoHistory* self);
        gboolean (*can_undo)      (GundoHistory* self);

        guint    (*get_n_redos)   (GundoHistory* self);
        guint    (*get_n_undos)   (GundoHistory* self);

        void     (*redo)          (GundoHistory* self);
        void     (*undo)          (GundoHistory* self);
};

G_END_DECLS

#endif /* !GUNDO_HISTORY_H */



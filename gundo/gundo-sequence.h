/* This file is part of gundo
 *
 * AUTHORS
 *     Sven Herzberg  <herzi@gnome-de.org>
 *
 * Copyright (C) 2005,2009  Sven Herzberg
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

#ifndef GUNDO_SEQUENCE_H
#define GUNDO_SEQUENCE_H

#include <glib-object.h>

G_BEGIN_DECLS

typedef struct _GundoSequence GundoSequence;
typedef struct _GObjectClass  GundoSequenceClass;

#define GUNDO_TYPE_SEQUENCE         (gundo_sequence_get_type())
#define GUNDO_SEQUENCE(i)           (G_TYPE_CHECK_INSTANCE_CAST((i), GUNDO_TYPE_SEQUENCE, GundoSequence))
#define GUNDO_SEQUENCE_CLASS(c)     (G_TYPE_CHECK_CLASS_CAST( (c), GUNDO_TYPE_SEQUENCE, GundoSequenceClass))
#define GUNDO_IS_SEQUENCE(i)        (G_TYPE_CHECK_INSTANCE_TYPE((i), GUNDO_TYPE_SEQUENCE))
#define GUNDO_IS_SEQUENCE_CLASS(c)  (G_TYPE_CHECK_CLASS_TYPE((c), GUNDO_TYPE_SEQUENCE))
#define GUNDO_SEQUENCE_GET_CLASS(i) (G_TYPE_INSTANCE_GET_CLASS((i), GUNDO_TYPE_SEQUENCE, GundoSequenceClass))

typedef void (*GundoActionCallback)( gpointer action_data );
typedef struct _GundoActionType GundoActionType;

GType          gundo_sequence_get_type   (void);
GundoSequence *gundo_sequence_new        (void);
void           gundo_sequence_clear      (GundoSequence *seq );
void           gundo_sequence_add_action (GundoSequence *seq, 
                                          const GundoActionType *type, 
                                          gpointer data );
void           gundo_sequence_start_group(GundoSequence *seq );
void           gundo_sequence_end_group  (GundoSequence *seq );
void           gundo_sequence_abort_group(GundoSequence *seq );

struct _GundoSequence
{
	GObject        base_object;
	GArray       * actions;
	guint          next_redo;
	GundoSequence* group;
};

struct _GundoActionType {
    GundoActionCallback undo;
    GundoActionCallback redo;
    GundoActionCallback free;
};

G_END_DECLS

#endif /* !GUNDO_SEQUENCE_H */

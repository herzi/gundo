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

#include <gtk/gtk.h>

#include <gundo.h>

G_BEGIN_DECLS

struct _GundoSequence
{
    GtkObject base;
    GArray *actions;
    int next_redo;
    GundoSequence *group;
};

struct _GundoSequenceClass
{
    GtkObjectClass base;

    void (*can_undo)( GundoSequence*, gboolean );
    void (*can_redo)( GundoSequence*, gboolean );
};

G_END_DECLS

#endif /* !GUNDO_SEQUENCE_H */
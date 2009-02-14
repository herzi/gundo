/*
    Gtk Undo: Multilevel undo/redo for Gtk
    Copyright (C) 1999  Nat Pryce
    Copyright (C) 2005  Sven Herzberg

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef GUNDO_UI_H
#define GUNDO_UI_H

#include <gundo.h>

#include <gundo-tool-undo.h>
#include <gundo-undo-model.h>

G_BEGIN_DECLS

void gundo_make_undo_sensitive(GtkWidget *widget, GundoHistory *history);

void gundo_make_redo_sensitive(GtkWidget *widget, GundoHistory *history);

G_END_DECLS

#endif /* !GUNDO_UI_H */

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


#include <gundo-ui/gundo-ui.h>


typedef struct Connection Connection;
struct Connection {
    GundoSequence *seq;
    guint undo_sensitive_signal;
    guint undo_destroy_signal;
    GtkWidget *widget;
    guint widget_destroy_signal;
};


static void cb_set_sensitivity( GundoSequence *seq, 
                                gboolean sensitive, 
                                GtkWidget *widget ) 
{
    gtk_widget_set_sensitive( widget, sensitive );
}

static void cb_widget_destroyed(GtkWidget *widget, Connection *cx) {
#ifdef RE_INTRODUCE_DESTROY_ON_SEQ
    g_signal_handler_disconnect(G_OBJECT(cx->seq), cx->undo_destroy_signal );
    g_signal_handler_disconnect(G_OBJECT(cx->seq), cx->undo_sensitive_signal );
#endif
    
    g_free(cx);
}

static void cb_undo_sequence_destroyed( GundoSequence *seq, Connection *cx ) {
    g_signal_handler_disconnect( GTK_OBJECT(cx->widget), cx->widget_destroy_signal );
    
    g_free(cx);
}

static void
make_sensitive(GtkWidget* widget, GundoSequence* seq, char* signal_name) {
    Connection *cx = g_new( Connection, 1 );
    
    cx->seq = seq;
    cx->widget = widget;
    
    cx->undo_sensitive_signal = 
        g_signal_connect( G_OBJECT(seq),
                            signal_name,
                            G_CALLBACK(cb_set_sensitivity),
                            widget );
#warning "FIXME: re-introduce this as a weak reference"
#ifdef RE_INTRODUCE_DESTROY_ON_SEQ
    cx->undo_destroy_signal =
        g_signal_connect( G_OBJECT(seq),
                            "destroy",
                            G_CALLBACK(cb_undo_sequence_destroyed),
                            cx );
#endif
    cx->widget_destroy_signal =
        g_signal_connect( G_OBJECT(widget),
                            "destroy",
                            G_CALLBACK(cb_widget_destroyed),
                            cx );
}

/**
 * gundo_make_undo_sensitive:
 * @widget: The widget to make undo-sensitive
 * @seq: The undo sequence that the widget should be sensitive to
 *
 * Makes a widget sensitive to the current undo state of a GundoSequence.
 * A widget that is undo-sensitive will only be sensitive
 * when it is possible to call \Ref{gundo_sequence_undo} on its associated
 * GundoSequence (that is, when \Ref{gundo_sequence_can_undo} returns TRUE).
 */
void gundo_make_undo_sensitive( GtkWidget *widget, GundoSequence *seq ) {
    make_sensitive( widget, seq, "can_undo" );
    gtk_widget_set_sensitive( widget, gundo_sequence_can_undo(seq) );
}

/**
 * gundo_make_redo_sensitive:
 * @widget: The #GtkWidget to make redo-sensitive.
 * @seq: The undo sequence that the widget should be sensitive to.
 *
 * Makes a widget sensitive to the current redo state of a GundoSequence.  
 * A widget that is redo-sensitive will only be sensitive when it is 
 * possible to call \Ref{gundo_sequence_redo} on its associated GundoSequence 
 * (that is, when \Ref{gundo_sequence_can_redo} returns TRUE).
 */
void gundo_make_redo_sensitive( GtkWidget *widget, GundoSequence *seq ) {
    make_sensitive( widget, seq, "can_redo" );
    gtk_widget_set_sensitive( widget, gundo_sequence_can_redo(seq) );
}



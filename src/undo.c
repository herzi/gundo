/*
    Gtk Undo: Multilevel undo/redo for Gtk
    Copyright (C) 1999  Nat Pryce

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

#include <stdio.h>
#include <glib.h>
#include "undo.h"


typedef struct _UndoAction UndoAction;
struct _UndoAction {
    const UndoActionType *type;
    gpointer data;
};

enum UndoSignalType {
    UNDO_SEQUENCE_SIGNAL_CAN_UNDO,
    UNDO_SEQUENCE_SIGNAL_CAN_REDO,
    UNDO_SEQUENCE_SIGNAL_LAST = UNDO_SEQUENCE_SIGNAL_CAN_REDO
};


static void undo_sequence_class_init( UndoSequenceClass* );
static void undo_sequence_init( UndoSequence* );
static void undo_sequence_destroy( GtkObject *object );
static void group_undo( UndoSequence *seq );
static void group_redo( UndoSequence *seq );
static void free_actions( int actc, UndoAction *actv );


static gint undo_sequence_signals[UNDO_SEQUENCE_SIGNAL_LAST+1];
static GtkObjectClass *base_class = 0;
static UndoActionType undo_action_group = {
    (UndoActionCallback)group_undo,
    (UndoActionCallback)group_redo,
    (UndoActionCallback)gtk_object_destroy
};



guint undo_sequence_get_type () {
    static guint undo_sequence_type = 0;
    
    if( !undo_sequence_type ) {
        GtkTypeInfo type_info = {
            "UndoSequence",
            sizeof(UndoSequence),
            sizeof(UndoSequenceClass),
            (GtkClassInitFunc)undo_sequence_class_init,
            (GtkObjectInitFunc)undo_sequence_init,
            (GtkArgSetFunc)NULL,
            (GtkArgGetFunc)NULL
        };
        
        undo_sequence_type = 
            gtk_type_unique( gtk_object_get_type(), &type_info );
    }
    
    return undo_sequence_type;
}


static void undo_sequence_class_init( UndoSequenceClass *klass ) {
    GtkObjectClass *base = (GtkObjectClass*)klass;
    base->destroy = undo_sequence_destroy;
    
    undo_sequence_signals[UNDO_SEQUENCE_SIGNAL_CAN_UNDO] = 
        gtk_signal_new( "can_undo",
                        GTK_RUN_FIRST,
                        undo_sequence_get_type(),
                        GTK_SIGNAL_OFFSET( UndoSequenceClass, can_undo ),
                        gtk_marshal_NONE__BOOL,
                        GTK_TYPE_NONE, 1, GTK_TYPE_BOOL );
    
    undo_sequence_signals[UNDO_SEQUENCE_SIGNAL_CAN_REDO] = 
        gtk_signal_new( "can_redo",
                        GTK_RUN_FIRST,
                        undo_sequence_get_type(),
                        GTK_SIGNAL_OFFSET( UndoSequenceClass, can_redo ),
                        gtk_marshal_NONE__BOOL,
                        GTK_TYPE_NONE, 1, GTK_TYPE_BOOL );
    
    gtk_object_class_add_signals( base, undo_sequence_signals,
                                  UNDO_SEQUENCE_SIGNAL_LAST+1 );
    
    klass->can_undo = NULL;
    klass->can_redo = NULL;
    
    base_class = gtk_type_class( gtk_object_get_type() );
}

static void undo_sequence_init( UndoSequence *seq ) {
    seq->actions = g_array_new( FALSE, FALSE, sizeof(UndoAction) );
    seq->next_redo = 0;
    seq->group = NULL;
}

static void undo_sequence_destroy( GtkObject *object ) {
    UndoSequence *seq;
    
    g_return_if_fail( object != NULL );
    
    seq = UNDO_SEQUENCE(object);
    free_actions( seq->actions->len, (UndoAction*)seq->actions->data );
    if( seq->group ) gtk_object_destroy( GTK_OBJECT(seq->group) );
    g_array_free( seq->actions, TRUE );
    
    if (GTK_OBJECT_CLASS(base_class)->destroy) {
        (GTK_OBJECT_CLASS(base_class)->destroy)( object );
    }
}


UndoSequence *undo_sequence_new() {
    return UNDO_SEQUENCE( gtk_type_new( undo_sequence_get_type() ) );
}


void undo_sequence_clear( UndoSequence *seq ) {
    gboolean could_undo, could_redo;
    
    g_assert( seq->group == NULL );
    
    could_undo = undo_sequence_can_undo(seq);
    could_redo = undo_sequence_can_redo(seq);
    
    free_actions( seq->actions->len, (UndoAction*)seq->actions->data );
    g_array_set_size( seq->actions, 0 );
    seq->next_redo = 0;
    if( seq->group ) {
        gtk_object_destroy( GTK_OBJECT(seq->group) );
        seq->group = 0;
    }
    
    if( could_undo ) {
        gtk_signal_emit( GTK_OBJECT(seq),
                         undo_sequence_signals[UNDO_SEQUENCE_SIGNAL_CAN_UNDO],
                         FALSE );
    }
    if( could_redo ) {
        gtk_signal_emit( GTK_OBJECT(seq),
                         undo_sequence_signals[UNDO_SEQUENCE_SIGNAL_CAN_REDO],
                         FALSE );
    }
}


void undo_sequence_add_action( UndoSequence *seq, 
                               const UndoActionType *type, 
                               gpointer data ) 
{
    if( seq->group ) {
        undo_sequence_add_action( seq->group, type, data );
    } else {
        UndoAction action;
        gboolean could_undo = undo_sequence_can_undo(seq);
        gboolean could_redo = undo_sequence_can_redo(seq);
        
        if( seq->next_redo < seq->actions->len ) {
            free_actions( seq->actions->len - seq->next_redo,
                          (UndoAction*)seq->actions->data + seq->next_redo );
            
            g_array_set_size( seq->actions, seq->next_redo );
        }
        
        action.type = type;
        action.data = data;
        
        g_array_append_val( seq->actions, action );
        seq->next_redo++;
        
        if( !could_undo ) {
            gtk_signal_emit( GTK_OBJECT(seq),
                             undo_sequence_signals[UNDO_SEQUENCE_SIGNAL_CAN_UNDO],
                             TRUE );
        }
        if( could_redo ) {
            gtk_signal_emit( GTK_OBJECT(seq),
                             undo_sequence_signals[UNDO_SEQUENCE_SIGNAL_CAN_REDO],
                             FALSE );
        }
    }
}

void undo_sequence_start_group( UndoSequence *seq ) {
    if( seq->group ) {
        undo_sequence_start_group( seq->group );
    } else {
        seq->group = undo_sequence_new();
    }
}

void undo_sequence_end_group( UndoSequence *seq ) {
    g_assert( seq->group != NULL );
    
    if( seq->group->group ) {
        undo_sequence_end_group( seq->group );
    } else {
        UndoSequence *group = seq->group;
        seq->group = NULL;
        
        if( group->actions->len > 0 ) {
            undo_sequence_add_action( seq, &undo_action_group, group );
        } else {
            gtk_object_destroy( GTK_OBJECT(group) );
        }
    }
}

void undo_sequence_abort_group( UndoSequence *seq ) {
    g_assert( seq->group != NULL );
    
    if( seq->group->group ) {
        undo_sequence_abort_group( seq->group );
    } else {
        UndoSequence *group = seq->group;
        seq->group = 0;
        gtk_object_destroy( GTK_OBJECT(group) );
    }
}

gboolean undo_sequence_can_undo( UndoSequence *seq ) {
    return seq->next_redo > 0;
}

void undo_sequence_undo( UndoSequence *seq ) {
    UndoAction *action;
    gboolean could_redo;
    
    g_assert( seq->group == NULL );
    g_assert( undo_sequence_can_undo(seq) );
    
    could_redo = undo_sequence_can_redo(seq);
    
    seq->next_redo--;
    action = &g_array_index( seq->actions, UndoAction, seq->next_redo );
    (action->type->undo)( action->data );
    
    if( !could_redo ) {
        gtk_signal_emit( GTK_OBJECT(seq),
                         undo_sequence_signals[UNDO_SEQUENCE_SIGNAL_CAN_REDO],
                         TRUE );
    }
    if( !undo_sequence_can_undo(seq) ) {
        gtk_signal_emit( GTK_OBJECT(seq),
                         undo_sequence_signals[UNDO_SEQUENCE_SIGNAL_CAN_UNDO],
                         FALSE );
    }
}


gboolean undo_sequence_can_redo( UndoSequence *seq ) {
    return seq->next_redo < seq->actions->len;
}

void undo_sequence_redo( UndoSequence *seq ) {
    UndoAction *action;
    gboolean could_undo;
    
    g_assert( seq->group == NULL );
    g_assert( undo_sequence_can_redo(seq) );
    
    could_undo = undo_sequence_can_undo(seq);
    
    action = &g_array_index( seq->actions, UndoAction, seq->next_redo );
    seq->next_redo++;
    (action->type->redo)( action->data );
    
    if( !could_undo ) {
        gtk_signal_emit( GTK_OBJECT(seq),
                         undo_sequence_signals[UNDO_SEQUENCE_SIGNAL_CAN_UNDO],
                         TRUE );
    }
    if( !undo_sequence_can_redo(seq) ) {
        gtk_signal_emit( GTK_OBJECT(seq),
                         undo_sequence_signals[UNDO_SEQUENCE_SIGNAL_CAN_REDO],
                         FALSE );
    }
}


static void group_undo( UndoSequence *seq ) {
    while( undo_sequence_can_undo(seq) ) undo_sequence_undo(seq);
}


static void group_redo( UndoSequence *seq ) {
    while( undo_sequence_can_redo(seq) ) undo_sequence_redo(seq);
}


static void free_actions( int actc, UndoAction *actv ) {
    int i;
    
    for( i = 0; i < actc; i++ ) {
        if( (actv[i].type->free) ) (actv[i].type->free)( actv[i].data );
    }
}

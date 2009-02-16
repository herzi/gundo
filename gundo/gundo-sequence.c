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
/**
 * SECTION:gundosequence
 * @short_description: sequence of undo/redo actions
 *
 * A #GundoSequence contains a list of undoable/redoable actions. Action can be
 * added with gundo_sequence_add_action().
 * The list can be cleared by using gundo_sequence_clear().
 *
 * FIXME: write more
 */
 
#include <stdio.h>
#include <glib.h>
#include "gundo.h"

/**
 * GundoActionType:
 * @undo: Function called to undo the action.
 * @redo: Function called to redo the action.
 * @free: Function called to free the action_data.  Can be NULL, in which case
 * the action_data is not free'd.
 *
 * An GundoActionType defines the operations that can be applied to an undo
 * action that has been added to an GundoSequence.  All operations are of
 * type #GundoActionCallback.  That is, they take a pointer to some data
 * about the action and do not return a value.
 * 
 * Operations:
 * 
 * undo: Undoes an action of this type.
 * 
 * redo: Redoes an action of this type.
 * 
 * free: Frees the data about an action of this type. Can be %NULL, in which
 * case the data is not freed.
 * 
 * @see #gundo_sequence_add_action
 */

/**
 * GundoActionCallback:
 * @action_data: Data about the action.  The action_data pointer must be
 * cast to a concrete type by GundoActionCallback implementations.
 *
 * The type of function called to undo or redo an action or free data
 * associated with an action.
 * 
 * @see #GundoActionType
 */

enum {
	PROP_0,
	PROP_CAN_UNDO,
	PROP_CAN_REDO
};

typedef struct _UndoAction UndoAction;
struct _UndoAction {
    const GundoActionType *type;
    gpointer data;
};

static void gundo_sequence_class_init( GundoSequenceClass* );
static void gundo_sequence_init( GundoSequence* );
static void group_undo( GundoSequence *seq );
static void group_redo( GundoSequence *seq );
static void free_actions( int actc, UndoAction *actv );


static GundoActionType gundo_action_group = {
    (GundoActionCallback)group_undo,
    (GundoActionCallback)group_redo,
    (GundoActionCallback)g_object_unref
};

static void gs_history_iface_init(GundoHistoryIface* iface);

G_DEFINE_TYPE_WITH_CODE(GundoSequence, gundo_sequence, G_TYPE_OBJECT,
			G_IMPLEMENT_INTERFACE(GUNDO_TYPE_HISTORY, gs_history_iface_init));

static void gundo_sequence_init( GundoSequence *seq ) {
    seq->actions = g_array_new( FALSE, FALSE, sizeof(UndoAction) );
    seq->next_redo = 0;
    seq->group = NULL;
}

static void
gs_finalize(GObject *object) {
	GundoSequence *seq;

	g_return_if_fail(object);

	seq = GUNDO_SEQUENCE(object);
	free_actions(seq->actions->len, (UndoAction*)seq->actions->data);
	
	if(seq->group) {
		g_object_unref(G_OBJECT(seq->group));
		seq->group = NULL;
	}
	g_array_free(seq->actions, TRUE);

	if(G_OBJECT_CLASS(gundo_sequence_parent_class)->finalize) {
		G_OBJECT_CLASS(gundo_sequence_parent_class)->finalize(object);
	}
}

static void
gs_get_property(GObject* object, guint prop_id, GValue* value, GParamSpec* pspec) {
	GundoHistory* history = GUNDO_HISTORY(object);
	
	switch(prop_id) {
	case PROP_CAN_REDO:
		g_value_set_boolean(value, gundo_history_can_redo(history));
		break;
	case PROP_CAN_UNDO:
		g_value_set_boolean(value, gundo_history_can_undo(history));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
		break;
	}
}

static void
gs_set_property(GObject* object, guint prop_id, GValue const* value, GParamSpec* pspec) {
	switch(prop_id) {
	case PROP_CAN_REDO:
	case PROP_CAN_UNDO:
		// these two cannot be set
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
		break;
	}
}

static void
gundo_sequence_class_init(GundoSequenceClass *self_class) {
	GObjectClass *go_class = G_OBJECT_CLASS(self_class);
	go_class->finalize     = gs_finalize;
	go_class->get_property = gs_get_property;
	go_class->set_property = gs_set_property;

	gundo_history_install_properties(go_class, PROP_CAN_UNDO, PROP_CAN_REDO);
}


/**
 * gundo_sequence_new:
 *
 * Creates a new, empty GundoSequence.
 * 
 * The GundoSequence is destroyed by a call to gtk_object_destroy.  All
 * of the actions in a sequence are destroyed (by calling the free callback
 * of their GundoActionType) when the sequence is destroyed.
 * 
 * Returns a new GundoSequence object.
 */
GundoSequence*
gundo_sequence_new (void)
{
  return g_object_new (GUNDO_TYPE_SEQUENCE,
                       NULL);
}


/**
 * gundo_sequence_clear:
 * @seq: A #GundoSequence
 *
 * Free all actions in the undo sequence.
 */
void
gundo_sequence_clear(GundoSequence *seq) {
	gboolean could_undo, could_redo;

	g_return_if_fail(seq->group == NULL);

	could_undo = gundo_history_can_undo(GUNDO_HISTORY(seq));
	could_redo = gundo_history_can_redo(GUNDO_HISTORY(seq));

	free_actions( seq->actions->len, (UndoAction*)seq->actions->data );
	g_array_set_size( seq->actions, 0 );

	seq->next_redo = 0;
	if(seq->group) {
		g_object_unref(G_OBJECT(seq->group));
		seq->group = NULL;
	}

	if(could_undo) {
		// now we definitely can't undo
		g_object_notify(G_OBJECT(seq), "can-undo");
	}
	if(could_redo) {
		// now we definitely can't redo
		g_object_notify(G_OBJECT(seq), "can-redo");
	}
}


/**
 * gundo_sequence_add_action:
 * @seq: The undo sequence to which to add an action.
 * @type: The type of the action.
 * @data: Data about the action.  The data is destroyed by calling the
 * "free" callback of the action type.
 *
 * Adds an action to the end of the GundoSequence.  Any actions that
 * have been undone but not redone are destroyed.  If any groups have
 * been started, the action is appended to the end of the most recently
 * started group.
 */
void
gundo_sequence_add_action(GundoSequence* seq, GundoActionType const* type, gpointer data) {
	if( seq->group ) {
		gundo_sequence_add_action( seq->group, type, data );
	} else {
		UndoAction action;
		gboolean could_undo = gundo_history_can_undo(GUNDO_HISTORY(seq));
		gboolean could_redo = gundo_history_can_redo(GUNDO_HISTORY(seq));

		if( seq->next_redo < seq->actions->len ) {
			free_actions( seq->actions->len - seq->next_redo,
				      (UndoAction*)seq->actions->data + seq->next_redo );
		    
			g_array_set_size( seq->actions, seq->next_redo );
		}

		action.type = type;
		action.data = data;

		g_array_append_val( seq->actions, action );
		seq->next_redo++;

		if(!could_undo) {
			// now we definitely can undo
			g_object_notify(G_OBJECT(seq), "can-undo");
		}
		if(could_redo) {
			// now we definitely can't redo
			g_object_notify(G_OBJECT(seq), "can-redo");
		}

                gundo_history_changed (GUNDO_HISTORY (seq));
        }
}

/**
 * gundo_sequence_start_group:
 * @seq: a #GundoSequence
 * 
 * Starts a new group of actions that are treated as a single action.
 * E.g. a single call to undo and redo undoes or redoes all actions of
 * the group.  This is useful if a single action by the user causes
 * the program to issue multiple internal actions.  Groups can be
 * nested.
 */
void gundo_sequence_start_group( GundoSequence *seq ) {
    if( seq->group ) {
        gundo_sequence_start_group( seq->group );
    } else {
        seq->group = gundo_sequence_new();
    }
}

/**
 * gundo_sequence_end_group:
 * @seq: a #GundoSequence
 * 
 * Ends a group, adding it to the end of the undo sequence, or its parent
 * group.
 */
void gundo_sequence_end_group( GundoSequence *seq ) {
    g_return_if_fail( seq->group != NULL );
    
    if( seq->group->group ) {
        gundo_sequence_end_group( seq->group );
    } else {
        GundoSequence *group = seq->group;
        seq->group = NULL;
        
        if( group->actions->len > 0 ) {
            gundo_sequence_add_action( seq, &gundo_action_group, group );
        } else {
            g_object_unref(G_OBJECT(group));
        }
    }
}

/**
 * gundo_sequence_abort_group:
 * @seq: a #GundoSequence
 *
 * Aborts the construction of a group, freeing all of its actions.
 */
void gundo_sequence_abort_group( GundoSequence *seq ) {
    g_return_if_fail( seq->group != NULL );
    
    if( seq->group->group ) {
        gundo_sequence_abort_group( seq->group );
    } else {
        GundoSequence *group = seq->group;
        seq->group = 0;
        g_object_unref(G_OBJECT(group));
    }
}

static void
sequence_redo (GundoHistory* history)
{
        GundoSequence* seq = GUNDO_SEQUENCE (history);
	UndoAction *action;
	gboolean could_undo;

	g_return_if_fail( seq->group == NULL );
	g_return_if_fail( gundo_history_can_redo(GUNDO_HISTORY(seq) ));

	could_undo = gundo_history_can_undo(GUNDO_HISTORY(seq));

	action = &g_array_index( seq->actions, UndoAction, seq->next_redo );
	seq->next_redo++;
	(action->type->redo)( action->data );

	if(!could_undo) {
		// now we can definitely undo
		g_object_notify(G_OBJECT(seq), "can-undo");
	}
	if(!gundo_history_can_redo(GUNDO_HISTORY(seq))) {
		// so we can't redo anymore
		g_object_notify(G_OBJECT(seq), "can-redo");
	}
}


static void group_undo( GundoSequence *self ) {
	GundoHistory* history = GUNDO_HISTORY(self);
	while(gundo_history_can_undo(history)) {
		gundo_history_undo(history);
	}
}


static void
group_redo(GundoSequence *self)
{
	GundoHistory* history = GUNDO_HISTORY(self);
	while(gundo_history_can_redo (history)) {
		gundo_history_redo (history);
	}
}


static void free_actions( int actc, UndoAction *actv ) {
    int i;
    
    for( i = 0; i < actc; i++ ) {
        if( (actv[i].type->free) ) (actv[i].type->free)( actv[i].data );
    }
}

/* GundoHistory implementation */

static gboolean
gs_can_redo(GundoHistory *history) {
	GundoSequence* self = GUNDO_SEQUENCE(history);
	return self->next_redo < self->actions->len;
}

static gboolean
gs_can_undo(GundoHistory *history) {
	GundoSequence* self = GUNDO_SEQUENCE(history);
	return self->next_redo > 0;
}

static guint
sequence_get_n_redos (GundoHistory* history)
{
  return GUNDO_SEQUENCE (history)->actions->len - GUNDO_SEQUENCE (history)->next_redo;
}

static guint
sequence_get_n_changes (GundoHistory* history)
{
  if (GUNDO_SEQUENCE (history)->next_redo)
    return GUNDO_SEQUENCE (history)->actions->len;
  else
    return GUNDO_SEQUENCE (history)->next_redo;
}

static void
gs_undo(GundoHistory* history) {
	GundoSequence* self;
	UndoAction   * action;
	gboolean       could_redo;

	self = GUNDO_SEQUENCE(history);

	g_return_if_fail(self->group == NULL);
	g_return_if_fail(gundo_history_can_undo(history));

	could_redo = gundo_history_can_redo(history);

	self->next_redo--;
	action = &g_array_index( self->actions, UndoAction, self->next_redo );
				(action->type->undo)( action->data );

	if(!could_redo) {
		// now we definitely can redo
		g_object_notify(G_OBJECT(self), "can-redo");
	}
	if(!gundo_history_can_undo(history)) {
		// so, we can't undo anymore
		g_object_notify(G_OBJECT(self), "can-undo");
	}
}

static void
gs_history_iface_init (GundoHistoryIface* iface)
{
  iface->can_redo      = gs_can_redo;
  iface->can_undo      = gs_can_undo;

  iface->get_n_redos   = sequence_get_n_redos;
  iface->get_n_undos   = sequence_get_n_changes;

  iface->undo          = gs_undo;
  iface->redo          = sequence_redo;
}



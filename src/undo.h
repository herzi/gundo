#ifndef __UNDO_H__
#define __UNDO_H__
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

#include <gtk/gtk.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _UndoSequence UndoSequence;
struct _UndoSequence
{
    GtkObject base;
    GArray *actions;
    int next_redo;
    UndoSequence *group;
};


typedef struct _UndoSequenceClass UndoSequenceClass;
struct _UndoSequenceClass
{
    GtkObjectClass base;
    
    void (*can_undo)( UndoSequence*, gboolean );
    void (*can_redo)( UndoSequence*, gboolean );
};



/*  An UndoActionType defines the operations that can be applied to an undo
 *  action that has been added to an UndoSequence.  All operations are of
 *  type UndoActionCallback.  That is, they take a pointer to some data
 *  about the action and do not return a value.
 *  
 *  Operations:
 *    undo: Undoes an action of this type.
 *    redo: Redoes an action of this type.
 *    free: Frees the data about an action of this type.  Can be NULL, in
 *          which case the data is not freed.
 */
typedef void (*UndoActionCallback)( gpointer action_data );
typedef struct _UndoActionType UndoActionType;
struct _UndoActionType {
    UndoActionCallback undo; /* Undo the action */
    UndoActionCallback redo; /* Redo the action */
    UndoActionCallback free; /* Free the action_data; can be NULL */
};



#define UNDO_TYPE_SEQUENCE \
    (undo_sequence_get_type())
#define UNDO_SEQUENCE(object) \
    (GTK_CHECK_CAST((object), UNDO_TYPE_SEQUENCE, UndoSequence) )
#define UNDO_SEQUENCE_CLASS(klass) \
    (GTK_CHECK_CLASS_CAST( (klass), UNDO_TYPE_SEQUENCE, GtkWidgetClass ))
#define UNDO_IS_SEQUENCE(object) \
    (GTK_CHECK_TYPE( (object), UNDO_TYPE_SEQUENCE ))
#define UNDO_IS_SEQUENCE_CLASS(klass) \
    (GTK_CHECK_CLASS_TYPE( (klass), UNDO_TYPE_SEQUENCE ))

guint undo_sequence_get_type();


/*  Creates a new, empty UndoSequence.  
 *
 *  The UndoSequence is destroyed by a call to gtk_object_destroy.  All
 *  of the actions in a sequence are destroyed (by calling the free callback
 *  of their UndoActionType) when the sequence is destroyed.
 */
UndoSequence *undo_sequence_new();

/*  Adds an action to the end of the UndoSequence.  Any actions that
 *  have been undone but not redone are destroyed.  If any groups have
 *  been started, the action is appended to the end of the most recently
 *  started group.
 */
void undo_sequence_add_action( UndoSequence *seq, 
                               const UndoActionType *type, 
                               gpointer data );

/*  Returns TRUE if there are actions that can be undone, FALSE otherwise.
 */
gboolean undo_sequence_can_undo( UndoSequence *seq );

/*  Returns TRUE if there are actions that have already been undone, and
 *  can therefore be redone, FALSE otherwise.
 */
gboolean undo_sequence_can_redo( UndoSequence *seq );

/*  Undoes the action at the end of the action sequence.
 *
 *  Pre: no group is being constructed && undo_sequence_can_undo(seq).
 */
void undo_sequence_undo( UndoSequence *seq );

/*  Redoes the last action that was undone.
 *  
 *  Pre: no group is being constructed && undo_sequence_can_redo(seq).
 */
void undo_sequence_redo( UndoSequence *seq );

/*  Starts a new group of actions that are treated as a single action.
 *  E.g. a single call to undo and redo undoes or redoes all actions of
 *  the group.  This is useful if a single action by the user causes
 *  the program to issue multiple internal actions.  Groups can be
 *  nested.
 */
void undo_sequence_start_group( UndoSequence *seq );

/*  Ends a group, adding it to the end of the undo sequence, or its parent
 *  group.
 */
void undo_sequence_end_group( UndoSequence *seq );

/*  Aborts the construction of a group, freeing all of its actions.
 */
void undo_sequence_abort_group( UndoSequence *seq );

/*  Frees all actions in the undo sequence.
 *
 *  Pre:  no group is being constructed.
 */
void undo_sequence_clear( UndoSequence *seq );


#ifdef __cplusplus
}
#endif
#endif





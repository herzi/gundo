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


typedef struct _GundoSequence GundoSequence;
struct _GundoSequence
{
    GtkObject base;
    GArray *actions;
    int next_redo;
    GundoSequence *group;
};


typedef struct _GundoSequenceClass GundoSequenceClass;
struct _GundoSequenceClass
{
    GtkObjectClass base;
    
    void (*can_undo)( GundoSequence*, gboolean );
    void (*can_redo)( GundoSequence*, gboolean );
};



/*  An GundoActionType defines the operations that can be applied to an undo
 *  action that has been added to an GundoSequence.  All operations are of
 *  type UndoActionCallback.  That is, they take a pointer to some data
 *  about the action and do not return a value.
 *  
 *  Operations:
 *    undo: Undoes an action of this type.
 *    redo: Redoes an action of this type.
 *    free: Frees the data about an action of this type.  Can be NULL, in
 *          which case the data is not freed.
 */
typedef void (*GundoActionCallback)( gpointer action_data );
typedef struct _GundoActionType GundoActionType;
struct _GundoActionType {
    GundoActionCallback undo; /* Undo the action */
    GundoActionCallback redo; /* Redo the action */
    GundoActionCallback free; /* Free the action_data; can be NULL */
};



#define GUNDO_TYPE_SEQUENCE \
    (gundo_sequence_get_type())
#define GUNDO_SEQUENCE(object) \
    (GTK_CHECK_CAST((object), GUNDO_TYPE_SEQUENCE, GundoSequence) )
#define GUNDO_SEQUENCE_CLASS(klass) \
    (GTK_CHECK_CLASS_CAST( (klass), GUNDO_TYPE_SEQUENCE, GtkWidgetClass ))
#define GUNDO_IS_SEQUENCE(object) \
    (GTK_CHECK_TYPE( (object), GUNDO_TYPE_SEQUENCE ))
#define GUNDO_IS_SEQUENCE_CLASS(klass) \
    (GTK_CHECK_CLASS_TYPE( (klass), GUNDO_TYPE_SEQUENCE ))

guint gundo_sequence_get_type();


/*  Creates a new, empty GundoSequence.  
 *
 *  The GundoSequence is destroyed by a call to gtk_object_destroy.  All
 *  of the actions in a sequence are destroyed (by calling the free callback
 *  of their GundoActionType) when the sequence is destroyed.
 */
GundoSequence *gundo_sequence_new();

/*  Adds an action to the end of the GundoSequence.  Any actions that
 *  have been undone but not redone are destroyed.  If any groups have
 *  been started, the action is appended to the end of the most recently
 *  started group.
 */
void gundo_sequence_add_action( GundoSequence *seq, 
                                const GundoActionType *type, 
                                gpointer data );

/*  Returns TRUE if there are actions that can be undone, FALSE otherwise.
 */
gboolean gundo_sequence_can_undo( GundoSequence *seq );

/*  Returns TRUE if there are actions that have already been undone, and
 *  can therefore be redone, FALSE otherwise.
 */
gboolean gundo_sequence_can_redo( GundoSequence *seq );

/*  Undoes the action at the end of the action sequence.
 *
 *  Pre: no group is being constructed && undo_sequence_can_undo(seq).
 */
void gundo_sequence_undo( GundoSequence *seq );

/*  Redoes the last action that was undone.
 *  
 *  Pre: no group is being constructed && undo_sequence_can_redo(seq).
 */
void gundo_sequence_redo( GundoSequence *seq );

/*  Starts a new group of actions that are treated as a single action.
 *  E.g. a single call to undo and redo undoes or redoes all actions of
 *  the group.  This is useful if a single action by the user causes
 *  the program to issue multiple internal actions.  Groups can be
 *  nested.
 */
void gundo_sequence_start_group( GundoSequence *seq );

/*  Ends a group, adding it to the end of the undo sequence, or its parent
 *  group.
 */
void gundo_sequence_end_group( GundoSequence *seq );

/*  Aborts the construction of a group, freeing all of its actions.
 */
void gundo_sequence_abort_group( GundoSequence *seq );

/*  Frees all actions in the undo sequence.
 *
 *  Pre:  no group is being constructed.
 */
void gundo_sequence_clear( GundoSequence *seq );


#ifdef __cplusplus
}
#endif
#endif





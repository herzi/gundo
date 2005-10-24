#warning "FIXME: add lgpl header"

#ifndef GUNDO_TREE_MODEL_UNDO_H
#define GUNDO_TREE_MODEL_UNDO_H

#include <glib-object.h>

G_BEGIN_DECLS

typedef struct _GObject GundoTreeModelUndo;
typedef struct _GObjectClass GundoTreeModelUndoClass;

#define GUNDO_TYPE_TREE_MODEL_UNDO         (gundo_tree_model_undo_get_type())
#define GUNDO_TREE_MODEL_UNDO(i)           (G_TYPE_CHECK_INSTANCE_CAST())
#define GUNDO_TREE_MODEL_UNDO_CLASS(c)     (G_TYPE_CHECK_CLASS_CAST())
#define GUNDO_IS_TREE_MODEL_UNDO(i)        (G_TYPE_CHECK_INSTANCE_TYPE())
#define GUNDO_IS_TREE_MODEL_UNDO_CLASS(c)  (G_TYPE_CHECK_CLASS_TYPE())
#define GUNDO_TREE_MODEL_UNDO_GET_CLASS(i) (G_TYPE_INSTANCE_GET_CLASS())

G_END_DECLS

#endif /* !GUNDO_TREE_MODEL_UNDO */

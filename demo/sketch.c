

#include "gtk/gtk.h"
#include "gundo.h"
#include "gundo_ui.h"


/*  A Stroke holds information about a single stroke of the user's "pen"
 *  on the drawing area.
 */
typedef struct Stroke Stroke;
struct Stroke {
    GArray *points;
    gboolean is_visible;
};


static Stroke *stroke_new();
static void stroke_destroy( Stroke *stroke );
static void stroke_add_point( Stroke *stroke, gint16 x, gint16 y );

static void cb_undo_stroke( Stroke *data );
static void cb_redo_stroke( Stroke *data );
static void cb_free_stroke( Stroke *stroke );

static void cb_undo_clicked( GtkWidget *widget, gpointer data );
static void cb_redo_clicked( GtkWidget *widget, gpointer data );
static void cb_clear_clicked( GtkWidget *widget, gpointer data );

static gboolean cb_expose( GtkWidget *view, 
                           GdkEventExpose *ev, 
                           gpointer data );
static gboolean cb_start_stroke( GtkWidget *widget,
                                 GdkEventButton *ev,
                                 gpointer data );
static gboolean cb_end_stroke( GtkWidget *widget,
                               GdkEventButton *ev,
                               gpointer data );
static gboolean cb_add_stroke_point( GtkWidget *widget,
                                     GdkEventMotion *ev,
                                     gpointer data );

static void cb_quit( GtkWidget *widget, gpointer data );


/*  Globals
 */

static GtkWidget *window, *sketch; /* GUI components */
static Stroke *current_stroke;     /* Stroke currently being draw, or 0 */
static GPtrArray *visible_strokes; /* Strokes drawn in window and not undone */
static GundoSequence *actions;     /* Undoable actions performed by the user */


static GundoActionType stroke_action_type = {
    (GundoActionCallback)cb_undo_stroke,
    (GundoActionCallback)cb_redo_stroke,
    (GundoActionCallback)cb_free_stroke
};


/*  Management of Stroke objects
 */

static Stroke *stroke_new() {
    Stroke *stroke = g_new( Stroke, 1 );
    stroke->points = g_array_new( FALSE, FALSE, sizeof(GdkPoint) );
    stroke->is_visible = TRUE;
}

static void stroke_destroy( Stroke *stroke ) {
    g_array_free( stroke->points, TRUE );
    g_free( stroke );
}

static void stroke_add_point( Stroke *stroke, gint16 x, gint16 y ) {
    GdkPoint pt;
    pt.x = x;
    pt.y = y;
    g_array_append_val( stroke->points, pt );
}

static void stroke_draw( GdkDrawable *d, GdkGC *gc, Stroke *stroke ) {
    gdk_draw_lines( d, gc,
                    (GdkPoint*)stroke->points->data, stroke->points->len );
}


/*  Functions that respond to user input to create strokes, and display
 *  the strokes drawn by the user when the window needs repainting.
 */

static void draw_strokes( GtkWidget *view ) {
    int i;
    for( i = 0; i < visible_strokes->len; i++ ) {
        Stroke *stroke = (Stroke*)g_ptr_array_index( visible_strokes, i );
        stroke_draw( view->window, view->style->black_gc, stroke );
    }
}

static gboolean cb_expose( GtkWidget *view, 
                           GdkEventExpose *ev, 
                           gpointer data )
{
    draw_strokes(view);
    return TRUE;
}

static gboolean cb_start_stroke( GtkWidget *widget,
                                 GdkEventButton *ev,
                                 gpointer data )
{
    current_stroke = stroke_new();
    stroke_add_point( current_stroke, ev->x, ev->y );
}

static gboolean cb_end_stroke( GtkWidget *widget,
                               GdkEventButton *ev,
                               gpointer data )
{
    g_ptr_array_add( visible_strokes, current_stroke );
    gundo_sequence_add_action( actions, &stroke_action_type, current_stroke );
    current_stroke = 0;
}

static gboolean cb_add_stroke_point( GtkWidget *widget,
                                     GdkEventMotion *ev,
                                     gpointer data )
{
    stroke_add_point( current_stroke, ev->x, ev->y );
    stroke_draw( widget->window, widget->style->black_gc, current_stroke );
}


/*  Callbacks from the undo sequence to undo/redo user actions
 */

static void cb_undo_stroke( Stroke *stroke ) {
    stroke->is_visible = FALSE;
    g_ptr_array_set_size( visible_strokes, visible_strokes->len-1 );
    gdk_window_clear( sketch->window );
    draw_strokes( sketch );
}

static void cb_redo_stroke( Stroke *stroke ) {
    stroke->is_visible = TRUE;
    g_ptr_array_add( visible_strokes, stroke );
    stroke_draw( sketch->window, sketch->style->black_gc, stroke );
}

static void cb_free_stroke( Stroke *stroke ) {
    if( !(stroke->is_visible) ) stroke_destroy( stroke );
}


/*  Callbacks from the undo/redo GUI buttons.
 */

static void cb_undo_clicked( GtkWidget *widget, gpointer data ) {
    gundo_sequence_undo( actions );
}

static void cb_redo_clicked( GtkWidget *widget, gpointer data ) {
    gundo_sequence_redo( actions );
}

static void cb_clear_clicked( GtkWidget *widget, gpointer data ) {
    int i;
    
    gundo_sequence_clear( actions );
    
    for( i = 0; i < visible_strokes->len; i++ ) {
        stroke_destroy( (Stroke*)g_ptr_array_index( visible_strokes, i ) );
    }
    
    g_ptr_array_set_size( visible_strokes, 0 );

    gdk_window_clear( sketch->window );
}


/*  Initialisation
 */

void init_sketch() {
    GtkWidget *hbox;
    GtkWidget *vbox;
    GtkWidget *button;
    
    visible_strokes = g_ptr_array_new();
    actions = gundo_sequence_new();
    
    window = gtk_window_new( GTK_WINDOW_TOPLEVEL );
    gtk_window_set_title( GTK_WINDOW(window), "Undo Demo - Sketch" );
    gtk_window_set_policy( GTK_WINDOW(window), TRUE, TRUE, TRUE );
    
    hbox = gtk_hbox_new( FALSE, 2 );
    button = gtk_button_new_with_label( "Quit" );
    gtk_box_pack_start( GTK_BOX(hbox), button, FALSE, FALSE, 0 );
    gtk_signal_connect( GTK_OBJECT(button), "clicked",
                        GTK_SIGNAL_FUNC(cb_quit), NULL );
    button = gtk_button_new_with_label( "Undo" );
    gundo_make_undo_sensitive( button, actions );
    gtk_box_pack_start( GTK_BOX(hbox), button, FALSE, FALSE, 0 );
    gtk_signal_connect( GTK_OBJECT(button), "clicked",
                        GTK_SIGNAL_FUNC(cb_undo_clicked), NULL );
    button = gtk_button_new_with_label( "Redo" );
    gundo_make_redo_sensitive( button, actions );
    gtk_box_pack_start( GTK_BOX(hbox), button, FALSE, FALSE, 0 );
    gtk_signal_connect( GTK_OBJECT(button), "clicked",
                        GTK_SIGNAL_FUNC(cb_redo_clicked), NULL );
    button = gtk_button_new_with_label( "Clear" );
    gundo_make_undo_sensitive( button, actions );
    gtk_box_pack_start( GTK_BOX(hbox), button, FALSE, FALSE, 0 );
    gtk_signal_connect( GTK_OBJECT(button), "clicked",
                        GTK_SIGNAL_FUNC(cb_clear_clicked), NULL );
    
    sketch = gtk_drawing_area_new();
    gtk_drawing_area_size( GTK_DRAWING_AREA(sketch), 256, 256 );
    gtk_widget_set_events( sketch, 
                           gtk_widget_get_events(sketch) 
                               |GDK_EXPOSURE_MASK|GDK_BUTTON_MOTION_MASK
                               |GDK_BUTTON_PRESS_MASK
                               |GDK_BUTTON_RELEASE_MASK );
    
    vbox = gtk_vbox_new( FALSE, 2 );
    gtk_box_pack_start( GTK_BOX(vbox), hbox, TRUE, FALSE, 0 );
    gtk_box_pack_start( GTK_BOX(vbox), sketch, TRUE, FALSE, 0 );
    
    gtk_container_add( GTK_CONTAINER(window), vbox );
    
    gtk_signal_connect( GTK_OBJECT(sketch), "expose_event",
                        GTK_SIGNAL_FUNC(cb_expose), NULL );
    gtk_signal_connect( GTK_OBJECT(sketch), "button-press-event",
                        GTK_SIGNAL_FUNC(cb_start_stroke), NULL );
    gtk_signal_connect( GTK_OBJECT(sketch), "button-release-event",
                        GTK_SIGNAL_FUNC(cb_end_stroke), NULL );
    gtk_signal_connect( GTK_OBJECT(sketch), "motion-notify-event",
                        GTK_SIGNAL_FUNC(cb_add_stroke_point), NULL );
    
    gtk_signal_connect( GTK_OBJECT(window), "delete_event", 
                        GTK_SIGNAL_FUNC(cb_quit), NULL );
    
    gtk_widget_show_all( window );
}

void quit_sketch() {
    int i;
    
    gtk_widget_destroy( window );
    if( current_stroke ) stroke_destroy( current_stroke );
    
    for( i = 0; i < visible_strokes->len; i++ ) {
        stroke_destroy( (Stroke*)g_ptr_array_index( visible_strokes, i ) );
    }
    g_ptr_array_free( visible_strokes, TRUE );
    
    gtk_object_destroy( GTK_OBJECT(actions) );
}


static void cb_quit( GtkWidget *widget, gpointer data ) {
    gtk_main_quit();
}



int main( int argc, char **argv ) {
    gtk_init( &argc, &argv );
    init_sketch();
    gtk_main();
    quit_sketch();
    return 0;
}

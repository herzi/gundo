
#include <stdio.h>
#include "undo.h"

#ifndef VERBOSE
#define VERBOSE 0
#endif


typedef struct TestData TestData;
struct TestData {
    int *count;
};

static void undo_inc( gpointer p ) {
    (*((TestData*)p)->count)--;
}

static void redo_inc( gpointer p ) {
    (*((TestData*)p)->count)++;
}

static void free_data( gpointer p ) {
    g_free(p);
}

static UndoActionType test_undo_action = { undo_inc, redo_inc, free_data };

static int count = 0;


static void check_value( int n, const char *test_id ) {
    if( count != n ) {
        fprintf( stderr, "%s: FAILED: count is %i, expected %i\n", 
                 test_id, count, n );
        exit(1);
    } else {
        if( VERBOSE ) fprintf( stdout, "%s: OK\n", test_id );
    }
}


static TestData *test_undo_data() {
    TestData *d = g_new(TestData,1);
    d->count = &count;
    return d;
}

static void do_inc( UndoSequence *seq ) {
    count++;
    undo_sequence_add_action( seq, &test_undo_action, test_undo_data() );
}

static void test_undo() {
    UndoSequence *seq = undo_sequence_new();
    
    count = 0;
    
    do_inc(seq);
    check_value( 1, "added undo action" );
    undo_sequence_undo( seq );
    check_value( 0, "undid increment" );
    undo_sequence_redo( seq );
    check_value( 1, "redid increment" );
    undo_sequence_undo( seq );
    check_value( 0, "undid increment again" );
    undo_sequence_redo( seq );
    check_value( 1, "redid increment again" );
    
    gtk_object_destroy( GTK_OBJECT(seq) );
    check_value( 1, "freed undo sequence" );
}

static void test_groups() {
    UndoSequence *seq = undo_sequence_new();
    
    count = 0;
    
    do_inc(seq);
    check_value( 1, "added undo action" );
    
    undo_sequence_start_group( seq );
    do_inc( seq );
    do_inc( seq );
    do_inc( seq );
    do_inc( seq );
    undo_sequence_end_group( seq );
    
    check_value( 5, "performed a group of undoable actions" );
    undo_sequence_undo( seq );
    check_value( 1, "undid the group" );
    undo_sequence_undo( seq );
    check_value( 0, "undid the initial single action" );
    undo_sequence_redo( seq );
    check_value( 1, "redid the initial single action" );
    undo_sequence_redo( seq );
    check_value( 5, "redid the group of actions" );
    
    gtk_object_destroy( GTK_OBJECT(seq) );
    check_value( 5, "freed undo sequence" );
}

int main( int argc, char **argv ) {
    gtk_init( &argc, &argv );
    test_undo();
    test_groups();
    printf( "%s: OK\n", argv[0] );
    return 0;
}

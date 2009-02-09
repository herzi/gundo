# Makefile for Eggs

noinst_PROGRAMS=sketch
SRCDIR = $(top_srcdir)/gundo

CFLAGS = $(GUNDO_CFLAGS) $(DEBUG_CFLAGS) -I$(SRCDIR)
LDFLAGS = $(GUNDO_LIBS) $(top_builddir)/gundo/libgundo.la

sketch_SOURCES=\
	sketch.c \
	$(NULL)


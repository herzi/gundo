# Makefile for Eggs

check_PROGRAMS=tundo
TESTS=$(check_PROGRAMS)

SRCDIR = ../gundo

CFLAGS = $(GUNDO_CFLAGS) $(DEBUG_CFLAGS) -I$(SRCDIR)
LDFLAGS = $(GUNDO_LIBS) $(top_builddir)/gundo/libgundo.la

tundo_SOURCES=\
	tundo.c \
	$(NULL)


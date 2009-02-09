# Makefile for Eggs

noinst_PROGRAMS=sketch

sketch_CPPFLAGS=\
	$(GUNDO_CFLAGS) \
	$(DEBUG_CFLAGS) \
	-I$(top_srcdir)/gundo \
	$(NULL)
sketch_LDADD=\
	$(GUNDO_LIBS) \
	$(top_builddir)/gundo/libgundo.la \
	$(NULL)
sketch_SOURCES=\
	sketch.c \
	$(NULL)


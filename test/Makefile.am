# Makefile for Eggs

check_PROGRAMS=tundo
TESTS=$(check_PROGRAMS)

tundo_CPPFLAGS=\
	$(GUNDO_CFLAGS) \
	$(DEBUG_CFLAGS) \
	-I$(top_srcdir)/gundo \
	$(NULL)
tundo_LDADD=\
	$(GUNDO_LIBS) \
	$(top_builddir)/gundo/libgundo.la \
	$(NULL)
tundo_SOURCES=\
	tundo.c \
	$(NULL)


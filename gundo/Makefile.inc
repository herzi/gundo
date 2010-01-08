# http://sourceware.org/autobook/autobook/autobook_91.html
# "Library Versioning"

lib_LTLIBRARIES+=libgundo.la

gundo_HEADERS=\
	gundo/gundo.h \
	gundo/gundo-history.h	\
	gundo/gundo-history-view.h \
	gundo/gundo-sequence.h \
	$(NULL)
gundodir=$(includedir)/gundo-0.2

libgundo_la_CPPFLAGS=\
	$(GUNDO_CFLAGS) \
	$(WARN_CFLAGS) \
	$(DEBUG_CFLAGS) \
	-I$(top_srcdir)/gundo \
	$(NULL)
libgundo_la_SOURCES=\
	$(gundo_HEADERS) \
	gundo/gobject-helpers.h \
	gundo/gundo-history.c \
	gundo/gundo-history-view.c \
	gundo/gundo-sequence.c \
	$(NULL)
libgundo_la_LDFLAGS=\
	-version-info 2:0:2 \
	$(NULL)
libgundo_la_LIBADD=\
	$(GUNDO_LIBS) \
	$(NULL)

# vim:set ft=automake:

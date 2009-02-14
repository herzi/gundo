# Makefile for libundo

# http://sourceware.org/autobook/autobook/autobook_91.html
# "Library Versioning"

lib_LTLIBRARIES=libgundo.la

gundo_HEADERS=\
	gundo.h \
	gundo-history.h	\
	gundo-history-view.h \
	gundo-sequence.h \
	$(NULL)
gundodir=$(includedir)/gundo-0.2

libgundo_la_CPPFLAGS=\
	$(GUNDO_CFLAGS) \
	$(WARN_CFLAGS) \
	$(DEBUG_CFLAGS) \
	$(NULL)
libgundo_la_SOURCES=\
	$(gundo_HEADERS) \
	gobject-helpers.h \
	gundo-history.c \
	gundo-history-view.c \
	gundo-sequence.c \
	$(NULL)
libgundo_la_LDFLAGS=\
	-version-info 2:0:2 \
	$(NULL)
libgundo_la_LIBADD=\
	$(GUNDO_LIBS) \
	$(NULL)



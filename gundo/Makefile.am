# Makefile for libundo

# http://sourceware.org/autobook/autobook/autobook_91.html
# "Library Versioning"

lib_LTLIBRARIES=libgundo.la

gundo_HEADERS=\
	gundo.h \
	gundo-ui.h \
	$(NULL)
gundodir=$(includedir)/gundo-0.2

libgundo_la_SOURCES=\
	$(gundo_HEADERS) \
	gundo.c \
	gundo-ui.c \
	$(NULL)
libgundo_la_LDFLAGS=\
	-version-info 2:0:2 \
	$(NULL)

CFLAGS = $(shell gtk-config --cflags gtk) $(DEBUG_CFLAGS)
LDFLAGS = -shared $(shell gtk-config --libs gtk)


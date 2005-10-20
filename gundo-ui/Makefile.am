# gundo-ui/Makefile.am for gundo

lib_LTLIBRARIES=libgundo-ui.la

libgundo_ui_la_CPPFLAGS=\
	$(GUNDO_UI_CFLAGS) \
	$(DEBUG_CFLAGS) \
	-I$(top_srcdir)/gundo \
	$(NULL)
libgundo_ui_la_LDFLAGS=\
	-version-info 2:0:2 \
	$(NULL)
libgundo_ui_la_LIBADD=\
	$(GUNDO_UI_LIBS) \
	$(top_builddir)/gundo/libgundo.la \
	$(NULL)
libgundo_ui_la_SOURCES=\
	gtk-helpers.c \
	gtk-helpers.h \
	gundo-ui.c \
	gundo-ui.h \
	gundo-tool-undo.c \
	gundo-tool-undo.h \
	$(NULL)


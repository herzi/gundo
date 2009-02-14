# gundo-ui/Makefile.am for gundo

lib_LTLIBRARIES=libgundo-ui.la

libgundo_ui_la_CPPFLAGS=\
	$(GUNDO_UI_CFLAGS) \
	$(WARN_CFLAGS) \
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
	gundo-popup-model.c \
	gundo-popup-model.h \
	gundo-redo-tool.c \
	gundo-redo-tool.h \
	gundo-tree-model-undo.c \
	gundo-tree-model-undo.h \
	gundo-ui.c \
	gundo-ui.h \
	gundo-undo-model.c \
	gundo-undo-model.h \
	gundo-undo-tool.c \
	gundo-undo-tool.h \
	$(NULL)


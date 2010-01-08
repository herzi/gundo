lib_LTLIBRARIES+=libgundo-ui.la

libgundo_ui_la_CPPFLAGS=\
	$(GUNDO_UI_CFLAGS) \
	$(WARN_CFLAGS) \
	$(DEBUG_CFLAGS) \
	-I$(top_srcdir)/gundo \
	-I$(top_srcdir)/gundo-ui \
	$(NULL)
libgundo_ui_la_LDFLAGS=\
	-version-info 2:0:2 \
	$(NULL)
libgundo_ui_la_LIBADD=\
	$(GUNDO_UI_LIBS) \
	$(top_builddir)/gundo/libgundo.la \
	$(NULL)
libgundo_ui_la_SOURCES=\
	gundo-ui/gtk-helpers.c \
	gundo-ui/gtk-helpers.h \
	gundo-ui/gundo-popup-model.c \
	gundo-ui/gundo-popup-model.h \
	gundo-ui/gundo-redo-model.c \
	gundo-ui/gundo-redo-model.h \
	gundo-ui/gundo-redo-tool.c \
	gundo-ui/gundo-redo-tool.h \
	gundo-ui/gundo-tool.c \
	gundo-ui/gundo-tool.h \
	gundo-ui/gundo-ui.c \
	gundo-ui/gundo-ui.h \
	gundo-ui/gundo-undo-model.c \
	gundo-ui/gundo-undo-model.h \
	gundo-ui/gundo-undo-tool.c \
	gundo-ui/gundo-undo-tool.h \
	$(NULL)

# vim:set ft=automake:

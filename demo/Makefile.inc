noinst_PROGRAMS+=sketch

sketch_CPPFLAGS=\
	$(GUNDO_UI_CFLAGS) \
	$(WARN_CFLAGS) \
	$(DEBUG_CFLAGS) \
	-I$(top_srcdir)/gundo \
	-I$(top_srcdir)/gundo-ui \
	$(NULL)
sketch_LDADD=\
	$(GUNDO_UI_LIBS) \
	$(top_builddir)/gundo/libgundo.la \
	$(top_builddir)/gundo-ui/libgundo-ui.la \
	$(NULL)
sketch_SOURCES=\
	demo/main.c \
	demo/sketch.c \
	demo/sketch.h \
	demo/sketch-window.c \
	demo/sketch-window.h \
	demo/stroke.h \
	demo/stroke.c \
	$(NULL)

# vim:set ft=automake:

noinst_PROGRAMS+=tundo
TESTS+=tundo

tundo_CPPFLAGS=\
	$(GUNDO_CFLAGS) \
	$(WARN_CFLAGS) \
	$(DEBUG_CFLAGS) \
	-I$(top_srcdir)/gundo \
	$(NULL)
tundo_LDADD=\
	$(GUNDO_LIBS) \
	$(top_builddir)/gundo/libgundo.la \
	$(NULL)
tundo_SOURCES=\
	test/tundo.c \
	$(NULL)

# vim:set ft=automake:

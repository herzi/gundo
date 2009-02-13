## Process this file with automake to produce Makefile.in
NULL=

# The name of the module, e.g. 'glib'.
DOC_MODULE=gundo

# The top-level SGML file. Change it if you want.
DOC_MAIN_SGML_FILE=$(DOC_MODULE)-docs.sgml

# The directory containing the source code. Relative to $(srcdir).
# gtk-doc will search all .c & .h files beneath here for inline comments
# documenting functions and macros.
DOC_SOURCE_DIR=../gundo

# Extra options to pass to gtkdoc-scanobj or gtkdoc-scangobj.
SCANGOBJ_OPTIONS=--type-init-func="g_type_init()"

# Extra options to supply to gtkdoc-scan.
SCAN_OPTIONS=

# Extra options to supply to gtkdoc-mkdb.
MKDB_OPTIONS=--sgml-mode --output-format=xml

# Extra options to supply to gtkdoc-fixref.
FIXXREF_OPTIONS=

# Used for dependencies.
HFILE_GLOB=$(top_srcdir)/gundo/*.h
CFILE_GLOB=$(top_srcdir)/gundo/*.c

# Header files to ignore when scanning.
IGNORE_HFILES=\
	$(NULL)

# Images to copy into HTML directory.
HTML_IMAGES=\
	$(NULL)

# Extra SGML files that are included by $(DOC_MAIN_SGML_FILE).
content_files=\
	$(NULL)

# CFLAGS and LDFLAGS for compiling scan program. Only needed if your app/lib
# contains GtkObjects/GObjects and you want to document signals and properties.
GTKDOC_CFLAGS=\
	-I$(top_srcdir)	\
	$(GUNDO_CFLAGS)	\
	$(NULL)
GTKDOC_LIBS=\
	$(top_builddir)/gundo/libgundo-0.4.la	\
	$(NULL)

publish: all
	rsync -ave ssh --delete html blaubeermuffin.de:/home/herzi/public_html/criawips/reference

include $(top_srcdir)/gtk-doc.make


## Process this file with automake to produce Makefile.in

# We require automake 1.6 at least.
AUTOMAKE_OPTIONS = 1.6

# This is a blank Makefile.am for using gtk-doc.
# Copy this to your project's API docs directory and modify the variables to
# suit your project. See the GTK+ Makefiles in gtk+/docs/reference for examples
# of using the various options.

# The name of the module, e.g. 'glib'.
DOC_MODULE=gundo

# The top-level SGML file. You can change this if you want to.
DOC_MAIN_SGML_FILE=$(DOC_MODULE)-docs.sgml

# The directory containing the source code. Relative to $(srcdir).
# gtk-doc will search all .c & .h files beneath here for inline comments
# documenting the functions and macros.
# e.g. DOC_SOURCE_DIR=../../../gtk
DOC_SOURCE_DIR=..

# Extra options to pass to gtkdoc-scangobj. Not normally needed.
SCANGOBJ_OPTIONS=--type-init-func="g_type_init();//gtk_init(&argc,&argv)"

# Extra options to supply to gtkdoc-scan.
# e.g. SCAN_OPTIONS=--deprecated-guards="GTK_DISABLE_DEPRECATED"
SCAN_OPTIONS=

# Extra options to supply to gtkdoc-mkdb.
# e.g. MKDB_OPTIONS=--sgml-mode --output-format=xml
MKDB_OPTIONS=--sgml-mode --output-format=xml

# Extra options to supply to gtkdoc-mktmpl
# e.g. MKTMPL_OPTIONS=--only-section-tmpl
MKTMPL_OPTIONS=

# Extra options to supply to gtkdoc-fixref. Not normally needed.
# e.g. FIXXREF_OPTIONS=--extra-dir=../gdk-pixbuf/html --extra-dir=../gdk/html
FIXXREF_OPTIONS=

# Used for dependencies. The docs will be rebuilt if any of these change.
# e.g. HFILE_GLOB=$(top_srcdir)/gtk/*.h
# e.g. CFILE_GLOB=$(top_srcdir)/gtk/*.c
HFILE_GLOB=$(top_srcdir)/gundo*/*.h
CFILE_GLOB=$(top_srcdir)/gundo*/*.c

# Header files to ignore when scanning.
# e.g. IGNORE_HFILES=gtkdebug.h gtkintl.h
IGNORE_HFILES=\
	config.h \
	gobject-helpers.h \
	gtk-helpers.h \
	sketch.h \
	sketch-window.h \
	stroke.h \
	$(NULL)

# Images to copy into HTML directory.
# e.g. HTML_IMAGES=$(top_srcdir)/gtk/stock-icons/stock_about_24.png
HTML_IMAGES=\
	$(NULL)

# Extra SGML files that are included by $(DOC_MAIN_SGML_FILE).
# e.g. content_files=running.sgml building.sgml changes-2.0.sgml
content_files=\
	private-classes.xml \
	tool-items.xml \
	tree-models.xml \
	$(NULL)

# SGML files where gtk-doc abbrevations (#GtkWidget) are expanded
# These files must be listed here *and* in content_files
# e.g. expand_content_files=running.sgml
expand_content_files=

# CFLAGS and LDFLAGS for compiling gtkdoc-scangobj with your library.
# Only needed if you are using gtkdoc-scangobj to dynamically query widget
# signals and properties.
# e.g. INCLUDES=-I$(top_srcdir) -I$(top_builddir) $(GTK_DEBUG_FLAGS)
# e.g. GTKDOC_LIBS=$(top_builddir)/gtk/$(gtktargetlib)
INCLUDES=\
	-I$(top_srcdir)/gundo \
	-I$(top_srcdir)/gundo-ui \
	$(GUNDO_UI_CFLAGS) \
	$(NULL)
GTKDOC_LIBS=\
	$(top_builddir)/libgundo.la \
	$(top_builddir)/libgundo-ui.la \
	$(NULL)

# This includes the standard gtk-doc make rules, copied by gtkdocize.
include $(top_srcdir)/gtk-doc.make

# Other files to distribute
# e.g. EXTRA_DIST += version.xml.in
EXTRA_DIST +=

# Files not to distribute
# for --rebuild-types in $(SCAN_OPTIONS), e.g. $(DOC_MODULE).types
# for --rebuild-sections in $(SCAN_OPTIONS) e.g. $(DOC_MODULE)-sections.txt
# DISTCLEANFILES=$(DOC_MODULE).types

# Comment this out if you want your docs-status tested during 'make check'
# TESTS = $(GTKDOC_CHECK)

.NOTPARALLEL: # make sure GNU make doesn't use parallel jobs

check-local:
	@echo "TEST  gundo-docs.sgml (all sections)"
	@grep FILE $(srcdir)/gundo-sections.txt | sed 's/<FILE>\(.*\)<\/FILE>/\1/' | while read file; do \
		if ! xmllint --path ".:$(srcdir)" --xinclude $(srcdir)/gundo-docs.sgml | grep -q "xml:base=\"xml/$$file\.xml\""; then \
			echo "gundo-docs.sgml is missing an include for $$file"; \
			exit 1; \
		fi; \
	done
	@echo "TEST  gundo-docs.sgml (valid docbook)"
	@xmllint --path ".:$(srcdir)" --xinclude $(srcdir)/gundo-docs.sgml | \
		sed 's@ xml:base="[^"]*\.\(x\|sg\)ml"@@' | \
		sed 's@xmlns:[[:alpha:]]*="[^"]*"@@' | \
		xmllint --postvalid --noout - 2>&1 | \
		grep -v "IDREF attribute linkend references an unknown ID \"\(Atk.*\|gboolean\|gchar\|GInitiallyUnowned\|GInterface\|GObject.*\|gpointer\|Gtk.*\|GType\(Interface\)\?\|guint\|void\|\(FALSE\|GTK.*\|NULL\|TRUE\)--CAPS\)\"" > sliff
	@if test 1 != `cat sliff | wc -l`; then \
		echo "******" >&2; \
		echo "*** Please fix the documentation:" >&2; \
		echo "******" >&2; \
		cat sliff; \
		exit 1; \
	fi
	@rm -f sliff
	@echo "TEST  gundo.types"
	@if ! (grep INCLUDE $(srcdir)/gundo-sections.txt | sed 's@^<INCLUDE>\(.*\)</INCLUDE>$$@\1@' | sort -u | sed 's/.*/#include <\0>/'; echo; grep get_type$$ $(srcdir)/gundo-sections.txt | sort -r) | diff -burp $(srcdir)/gundo.types -; then \
		echo "******" >&2; \
		echo "*** The file gundo.types looks bad. Please adjust it according to the patch above" >&2; \
		echo "******" >&2; \
		exit 1; \
	fi

# FIXME: make sure _get_type() functions are listed in private sections
# FIXME: make sure every section has its own INCLUDE tag


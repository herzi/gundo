AC_INIT(gundo,0.3.1,mailto:herzi@gnome-de.org)
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)

m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])
AM_PROG_CC_C_O

CXX=no
GCJ=no
F77=no

AC_PROG_LIBTOOL
AC_PROG_INTLTOOL


PKG_CHECK_MODULES(GUNDO,[
		gobject-2.0
		])

PKG_CHECK_MODULES(GUNDO_UI,[
		gtk+-2.0
		])

dnl  -----------------
dnl | compile options |-------------------------------------------------------
dnl  -----------------

GNOME_COMPILE_WARNINGS

dnl  --------------------
dnl | installation paths |----------------------------------------------------
dnl  --------------------

AC_MSG_CHECKING(prefix)

if test "x${prefix}" = "xNONE"; then
  PACKAGE_PREFIX="${ac_default_prefix}"
else
  PACKAGE_PREFIX="${prefix}"
fi
AC_MSG_RESULT($PACKAGE_PREFIX)

dnl  ------------------------------
dnl | internationalisation support |------------------------------------------
dnl  ------------------------------

dnl Please keep the languages in alphabetical order
ALL_LINGUAS="de"

GETTEXT_PACKAGE=gundo
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE",
		[The package name for gettext])
AC_SUBST(GETTEXT_PACKAGE)
AM_GLIB_GNU_GETTEXT

PACKAGE_LOCALE_DIR="${PACKAGE_PREFIX}/${DATADIRNAME}/locale"
AC_DEFINE_UNQUOTED(PACKAGE_LOCALE_DIR,"$PACKAGE_LOCALE_DIR",
		[The package's locale path for gettext])

GTK_DOC_CHECK()

DEBUG_CFLAGS=""
AC_ARG_ENABLE(debug,
              AS_HELP_STRING([--enable-debug],[enable debugging (default: no)]),
              [DEBUG_CFLAGS="-g -Wall -DDEBUG"],
              [DEBUG_CFLAGS=-O2])
AC_SUBST([DEBUG_CFLAGS])

AC_OUTPUT([
  Makefile
  doc/Makefile
  gundo/Makefile
  gundo-ui/Makefile
  po/Makefile.in
  test/Makefile
])


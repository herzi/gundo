AC_INIT(gundo,0.3.1,mailto:herzi@gnome-de.org)
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)

AM_PROG_CC_C_O

CXX=no
F77=no
AC_PROG_LIBTOOL

PKG_CHECK_MODULES(GUNDO,[gtk+-2.0])

GTK_DOC_CHECK()

DEBUG_CFLAGS=""
AC_ARG_ENABLE(debug,
              AS_HELP_STRING([--enable-debug],[enable debugging (default: no)]),
              [DEBUG_CFLAGS="-g -Wall -DDEBUG"],
              [DEBUG_CFLAGS=-O2])
AC_SUBST([DEBUG_CFLAGS])

AC_OUTPUT([
  Makefile
  demo/Makefile
  doc/Makefile
  gundo/Makefile
  test/Makefile
])


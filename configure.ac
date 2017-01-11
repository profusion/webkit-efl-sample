AC_INIT([webkit-efl-sample], [4], [http://profusion.mobi/])

AC_PREREQ([2.60])
AC_CONFIG_SRCDIR([configure.ac])

AM_INIT_AUTOMAKE([1.6 foreign dist-xz -Wall -Wextra subdir-objects])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_PROG_CC
PKG_PROG_PKG_CONFIG

PKG_CHECK_MODULES([WEBKIT_EFL], [ecore evas ecore-evas ewebkit2])
PKG_CHECK_VAR([EWEBKIT2_DATADIR], [ewebkit2], [datadir])

AC_DEFINE_UNQUOTED([EWEBKIT2_DATADIR], ["${EWEBKIT2_DATADIR}"], [WebKit-EFL data dir from pkg-config])

WINDOW_SIZE=800x600
AC_ARG_WITH([window-size],
            AS_HELP_STRING([--with-window-size=WIDTHxHEIGHT], [Default window size, WIDTHxHEIGHT, defaults ${WINDOW_SIZE}]),
         [WINDOW_SIZE=${withval}])

AC_DEFINE_UNQUOTED([DEFAULT_WIDTH], `echo $WINDOW_SIZE | cut -dx -f1`)
AC_DEFINE_UNQUOTED([DEFAULT_HEIGHT], `echo $WINDOW_SIZE | cut -dx -f2`)

URL="http://profusion.mobi/"
AC_ARG_WITH([url],
            AS_HELP_STRING([--with-url=URL],[Default URL.]),
            [URL=${withval}])
AC_DEFINE_UNQUOTED([DEFAULT_URL], ["${URL}"], [Default URL to load])

AC_OUTPUT([
Makefile
])

AC_INIT([webkit-efl-sample], [2], [http://profusion.mobi/])

AC_PREREQ([2.60])
AC_CONFIG_SRCDIR([configure.ac])

AM_INIT_AUTOMAKE([1.6 foreign dist-xz -Wall -Wextra subdir-objects])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_PROG_CC
PKG_PROG_PKG_CONFIG

PKG_CHECK_MODULES([WEBKIT_EFL], [ecore evas ecore-evas ewebkit2])

AC_OUTPUT([
Makefile
])

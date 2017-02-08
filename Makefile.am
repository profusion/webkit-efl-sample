MAINTAINERCLEANFILES = Makefile.in

bin_PROGRAMS = src/webkit-efl-sample

src_webkit_efl_sample_SOURCES = src/webkit-efl-sample.c
src_webkit_efl_sample_CPPFLAGS = @WEBKIT_EFL_CFLAGS@
src_webkit_efl_sample_LDADD = @WEBKIT_EFL_LIBS@

extensionpkgdir = $(libdir)/webkit-extensions
extensionpkg_LTLIBRARIES = src/libwebkit-efl-extension-sample.la

src_libwebkit_efl_extension_sample_la_SOURCES = src/webkit-efl-extension-sample.c
src_libwebkit_efl_extension_sample_la_CFLAGS = @WEBKIT_EXTENSION_EFL_CFLAGS@
src_libwebkit_efl_extension_sample_la_LIBADD = @WEBKIT_EXTENSION_EFL_LIBS@

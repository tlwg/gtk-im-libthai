if PLATFORM_WIN32
no_undefined = -no-undefined
endif

INCLUDES = \
	-DG_DISABLE_DEPRECATED	 			\
	-DGDK_PIXBUF_DISABLE_DEPRECATED			\
	-DGDK_DISABLE_DEPRECATED			\
	-DGTK_DISABLE_DEPRECATED 			\
	@GTK_DEP_CFLAGS@ \
	@LIBTHAI_DEP_CFLAGS@


LDADDS = \
	@GTK_DEP_LIBS@ \
	@LIBTHAI_DEP_LIBS@


moduledir = $(libdir)/gtk-2.0/$(GTK_BINARY_VERSION)/immodules

im_thai_libthai_la_LDFLAGS = -rpath $(moduledir) -avoid-version -module $(no_undefined)
im_thai_libthai_la_SOURCES = 		\
	gtkimcontextthai.c 	\
	gtkimcontextthai.h	\
	imthai-libthai.c
im_thai_libthai_la_LIBADD = $(LDADDS)

# Running this if cross compiling or if DESTDIR is set is going to
# not work at all, so skip it.
install-data-hook:
	@if test -z "$(DESTDIR)" ; then \
	  echo $(mkinstalldirs) $(DESTDIR)$(sysconfdir)/gtk-2.0 ; \
	  $(mkinstalldirs) $(DESTDIR)$(sysconfdir)/gtk-2.0 ; \
	  echo "gtk-query-immodules-2.0 > $(DESTDIR)$(sysconfdir)/gtk-2.0/gtk.immodules" ; \
	  gtk-query-immodules-2.0 > $(DESTDIR)$(sysconfdir)/gtk-2.0/gtk.immodules ; \
	else \
	  echo "***" ; \
	  echo "*** Warning: gtk.immodules not built" ; \
	  echo "***" ; \
	  echo "*** Generate this file manually on host" ; \
	  echo "*** system using gtk-query-immodules-2.0" ; \
	  echo "***" ; \
        fi

module_LTLIBRARIES =				\
	im-thai-libthai.la


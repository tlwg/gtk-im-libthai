if PLATFORM_WIN32
no_undefined = -no-undefined
endif

MODULE_SRC =			\
	gtkimcontextthai.c 	\
	gtkimcontextthai.h	\
	imthai-libthai.c

if HAS_GTK2
INSTALL_GTK2 = yes

gtk2moduledir = $(libdir)/gtk-2.0/$(GTK2_BINARY_VERSION)/immodules

gtk2module_LTLIBRARIES =	\
	im-thai-libthai2.la

im_thai_libthai2_la_LDFLAGS = -rpath $(gtk2moduledir) -avoid-version -module $(no_undefined)
im_thai_libthai2_la_SOURCES = $(MODULE_SRC)
im_thai_libthai2_la_CPPFLAGS = $(GTK2_CFLAGS) $(LIBTHAI_CFLAGS)
im_thai_libthai2_la_LIBADD = $(GTK2_LIBS) $(LIBTHAI_LIBS)
else
INSTALL_GTK2 = no
endif

if HAS_GTK3
INSTALL_GTK3 = yes

gtk3moduledir = $(libdir)/gtk-3.0/$(GTK3_BINARY_VERSION)/immodules

gtk3module_LTLIBRARIES =	\
	im-thai-libthai3.la

im_thai_libthai3_la_LDFLAGS = -rpath $(gtk3moduledir) -avoid-version -module $(no_undefined)
im_thai_libthai3_la_SOURCES = $(MODULE_SRC)
im_thai_libthai3_la_CPPFLAGS = $(GTK3_CFLAGS) $(LIBTHAI_CFLAGS)
im_thai_libthai3_la_LIBADD = $(GTK3_LIBS) $(LIBTHAI_LIBS)
else
INSTALL_GTK3 = no
endif

# Running this if cross compiling or if DESTDIR is set is going to
# not work at all, so skip it.
install-data-hook:
	@if test -z "$(DESTDIR)" ; then \
	  if [ "X$(INSTALL_GTK2)" = "Xyes" ]; then \
	    echo $(mkinstalldirs) $(libdir)/gtk-2.0/$(GTK2_BINARY_VERSION) ; \
	    $(mkinstalldirs) $(libdir)/gtk-2.0/$(GTK2_BINARY_VERSION) ; \
	    echo "gtk-query-immodules-2.0 > $(libdir)/gtk-2.0/$(GTK2_BINARY_VERSION)/gtk.immodules" ; \
	    gtk-query-immodules-2.0 > $(libdir)/gtk-2.0/$(GTK2_BINARY_VERSION)/gtk.immodules ; \
	  fi ; \
	  if [ "X$(INSTALL_GTK3)" = "Xyes" ]; then \
	    echo "gtk-query-immodules-3.0 --update-cache" ; \
	    gtk-query-immodules-3.0 --update-cache ; \
	  fi \
	else \
	  echo "***" ; \
	  echo "*** Warning: gtk.immodules not built" ; \
	  echo "***" ; \
	  echo "*** Generate this file manually on host" ; \
	  echo "*** system using gtk-query-immodules-2.0" ; \
	  echo "*** and/or gtk-query-immodules-3.0" ; \
	  echo "***" ; \
        fi


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
	gtk2/im-thai-libthai.la

gtk2_im_thai_libthai_la_LDFLAGS = \
	-rpath $(gtk2moduledir) -avoid-version -module $(no_undefined)
gtk2_im_thai_libthai_la_SOURCES = $(MODULE_SRC)
gtk2_im_thai_libthai_la_CPPFLAGS = $(GTK2_CFLAGS) $(LIBTHAI_CFLAGS)
gtk2_im_thai_libthai_la_LIBADD = $(GTK2_LIBS) $(LIBTHAI_LIBS)
else
INSTALL_GTK2 = no
endif

if HAS_GTK3
INSTALL_GTK3 = yes

gtk3moduledir = $(libdir)/gtk-3.0/$(GTK3_BINARY_VERSION)/immodules

gtk3module_LTLIBRARIES =	\
	gtk3/im-thai-libthai.la

gtk3_im_thai_libthai_la_LDFLAGS = \
	-rpath $(gtk3moduledir) -avoid-version -module $(no_undefined)
gtk3_im_thai_libthai_la_SOURCES = $(MODULE_SRC)
gtk3_im_thai_libthai_la_CPPFLAGS = $(GTK3_CFLAGS) $(LIBTHAI_CFLAGS)
gtk3_im_thai_libthai_la_LIBADD = $(GTK3_LIBS) $(LIBTHAI_LIBS)
else
INSTALL_GTK3 = no
endif

# Running this if cross compiling or if DESTDIR is set is going to
# not work at all, so skip it.
install-data-hook: update-modules

uninstall-hook: update-modules

update-modules:
	@if test -z "$(DESTDIR)" ; then \
	  if [ "X$(INSTALL_GTK2)" = "Xyes" ]; then \
	    GTK2_IM_MODULE_FILE=$(libdir)/gtk-2.0/$(GTK2_BINARY_VERSION)/gtk.immodules ; \
	    if ! test -f $${GTK2_IM_MODULE_FILE} ; then \
	      GTK2_IM_MODULE_FILE=$(sysconfdir)/gtk-2.0/gtk.immodules ; \
	    fi ; \
	    if test -f $${GTK2_IM_MODULE_FILE} ; then \
	      CMD="gtk-query-immodules-2.0 > $${GTK2_IM_MODULE_FILE}" ; \
	      echo $${CMD} ; eval $${CMD} ; \
	    fi \
	  fi ; \
	  if [ "X$(INSTALL_GTK3)" = "Xyes" ]; then \
	    GTK3_IM_MODULE_FILE=`dirname $(gtk3moduledir)`/immodules.cache ; \
	    if test -f $${GTK3_IM_MODULE_FILE}; then \
	      CMD="GTK_IM_MODULE_FILE=$${GTK3_IM_MODULE_FILE}" ; \
	      CMD="$${CMD} gtk-query-immodules-3.0 --update-cache" ; \
	      echo $${CMD} ; eval $${CMD} ; \
	    fi \
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


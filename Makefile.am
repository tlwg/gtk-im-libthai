ACLOCAL_AMFLAGS = -I m4

AM_DISTCHECK_CONFIGURE_FLAGS = \
	"--with-gtk2-immoduledir=$$dc_install_base/gtk-2.0/immodules"	\
	"--with-gtk3-immoduledir=$$dc_install_base/gtk-3.0/immodules"	\
	$(NULL)

EXTRA_DIST =	\
	build-aux/git-version-gen	\
	$(NULL)

SUBDIRS = src

dist-hook:
	echo $(VERSION) > $(distdir)/VERSION


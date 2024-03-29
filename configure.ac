# require autoconf 2.52
AC_PREREQ([2.71])

# Process this file with autoconf to produce a configure script.
AC_INIT([gtk-im-libthai],
        [m4_esyscmd(build-aux/git-version-gen)],
        [https://github.com/tlwg/gtk-im-libthai/issues])

## Versions of dependencies
GTK2_REQUIRED_VERSION=2.21.8
GTK3_REQUIRED_VERSION=3.0.0
LIBTHAI_REQUIRED_VERSION=0.1.2

dnl Initialize automake stuff
AM_INIT_AUTOMAKE(dist-xz no-dist-gzip)
AC_CONFIG_SRCDIR([src/imthai-libthai.c])
AC_CONFIG_MACRO_DIR([m4])

dnl Initialize libtool
AC_PROG_CC
LT_INIT([disable-static])

AC_MSG_CHECKING([for some Win32 platform])
case "$host" in
  *-*-mingw*|*-*-cygwin*)
    platform_win32=yes
    ;;
  *)
    platform_win32=no
    ;;
esac
AC_MSG_RESULT([$platform_win32])
AM_CONDITIONAL(PLATFORM_WIN32, test "$platform_win32" = "yes")

# Checks for programs.
AC_PROG_INSTALL
AC_PROG_MAKE_SET
PKG_PROG_PKG_CONFIG

# Check commandline options
AC_ARG_ENABLE(gtk2,
              [AS_HELP_STRING([--disable-gtk2],
                              [disable GTK+ 2 immodule])],
              enable_gtk2="no", enable_gtk2="yes")
AC_ARG_ENABLE(gtk3,
              [AS_HELP_STRING([--disable-gtk3],
                              [disable GTK+ 3 immodule])],
              enable_gtk3="no", enable_gtk3="yes")
if test "X$enable_gtk2" = "Xno" && test "X$enable_gtk3" = "Xno"; then
  AC_MSG_ERROR([Neither GTK+ 2 or GTK+ 3 is enable. Please enable at least one.])
fi

#
# Checks for required libraries
#
has_gtk2=no
has_gtk3=no
if test "X$enable_gtk2" = "Xyes"; then
  PKG_CHECK_MODULES(GTK2, gtk+-2.0 >= $GTK2_REQUIRED_VERSION,
                    has_gtk2="yes", has_gtk2="no")
fi
if test "X$enable_gtk3" = "Xyes"; then
  PKG_CHECK_MODULES(GTK3, gtk+-3.0 >= $GTK3_REQUIRED_VERSION,
                    has_gtk3="yes", has_gtk3="no")
fi
if test "X$has_gtk2" = "Xno" && test "X$has_gtk3" = "Xno"; then
  AC_MSG_ERROR([Neither gtk+-2.0 >= $GTK2_REQUIRED_VERSION nor gtk+-3.0 >= $GTK3_REQUIRED_VERSION was found.])
fi

AM_CONDITIONAL(HAS_GTK2,test "X$has_gtk2" = "Xyes")
if test "X$has_gtk2" = "Xyes"; then
  GTK2_LIBDIR=`$PKG_CONFIG --variable=libdir gtk+-2.0`
  GTK2_BINARY_VERSION=`$PKG_CONFIG --variable=gtk_binary_version gtk+-2.0`
else
  GTK2_LIBDIR=
  GTK2_BINARY_VERSION=
fi
AC_ARG_WITH(gtk2-immoduledir,
            [AS_HELP_STRING([--with-gtk2-immoduledir=DIR],
                            [where to install IM module for GTK+ 2 [GTK2_LIBDIR/gtk-2.0/GTK2_BINARY_VERSION/immodules]])],
            [gtk2moduledir="$withval"],
            [gtk2moduledir="$GTK2_LIBDIR/gtk-2.0/$GTK2_BINARY_VERSION/immodules"])
AC_SUBST(gtk2moduledir)

AM_CONDITIONAL(HAS_GTK3,test "X$has_gtk3" = "Xyes")
if test "X$has_gtk3" = "Xyes"; then
  GTK3_LIBDIR=`$PKG_CONFIG --variable=libdir gtk+-3.0`
  GTK3_BINARY_VERSION=`$PKG_CONFIG --variable=gtk_binary_version gtk+-3.0`
else
  GTK3_LIBDIR=
  GTK3_BINARY_VERSION=
fi
AC_ARG_WITH(gtk3-immoduledir,
            [AS_HELP_STRING([--with-gtk3-immoduledir=DIR],
                            [where to install IM module for GTK+ 3 [GTK3_LIBDIR/gtk-3.0/GTK3_BINARY_VERSION/immodules]])],
            [gtk3moduledir="$withval"],
            [gtk3moduledir="$GTK3_LIBDIR/gtk-3.0/$GTK3_BINARY_VERSION/immodules"])
AC_SUBST(gtk3moduledir)

# Fallback exclusion
AC_ARG_ENABLE(fallback,
              [AS_HELP_STRING([--disable-fallback],
                              [assume surrounding text support in all GTK+ apps and disable fallback for its absence (Be careful!)])],
              no_fallback="yes", no_fallback="no")
AM_CONDITIONAL(NO_FALLBACK,test "X$no_fallback" = "Xyes")

PKG_CHECK_MODULES(LIBTHAI, libthai >= $LIBTHAI_REQUIRED_VERSION)

AC_CONFIG_FILES([Makefile
  src/Makefile
])
AC_OUTPUT


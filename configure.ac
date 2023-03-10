dnl Process this file with autoconf to produce a configure script.
AC_INIT([which],[2.21])
AC_CONFIG_SRCDIR(which.c)
AM_INIT_AUTOMAKE
AC_CONFIG_HEADERS(config.h)
AM_MAINTAINER_MODE

dnl Because we use cwautomacros.
CW_AUTOMACROS

which_cv_iberty=yes
AC_ARG_ENABLE(iberty,
  [  --disable-iberty        do not try to find and use the iberty library],
  which_cv_iberty=$enableval)

dnl Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_LANG([C])
AC_USE_SYSTEM_EXTENSIONS

dnl Checks for libraries.
if test "$which_cv_iberty" = yes ; then
  AC_CHECK_STATICLIB(libiberty.a, xmalloc)
fi

dnl Checks for header files.
AC_CHECK_INCLUDES_DEFAULT
AC_PROG_EGREP

AC_CHECK_HEADERS(unistd.h string.h libiberty.h)
AC_HEADER_STAT

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T

dnl Checks for library functions.
AC_TYPE_GETGROUPS
AC_CHECK_FUNCS(strchr memcpy getopt_long getcwd getwd sysconf getgroups eaccess)

if test "$ac_cv_lib_static_libiberty_a_xmalloc" = no \
     -o "$which_cv_iberty" = no ; then
  AC_DEFINE([NEED_XMALLOC], 1, [Define if you need xmalloc().])
fi

dnl Additional stuff needed for the tilde directory
AC_CHECK_PROG(AR, ar, ar, ar)
AC_PROG_RANLIB
AC_CHECK_HEADERS(stdlib.h memory.h pwd.h strings.h)
AC_CHECK_FUNCS(getpwent getpwnam getpwuid)
BASH_CHECK_GETPW_FUNCS

dnl Generate output files.
AC_CONFIG_FILES([Makefile maintMakefile tilde/Makefile])
AC_OUTPUT

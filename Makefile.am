bin_PROGRAMS = which
which_SOURCES = getopt.c getopt1.c bash.c which.c \
		bash.h sys.h getopt.h posixstat.h
LDADD = $(top_builddir)/tilde/libtilde.a
man_MANS = which.1
BUILT_SOURCES = README EXAMPLES
EXTRA_DIST = $(man_MANS) $(BUILT_SOURCES)
SUBDIRS = tilde

DISTCLEANFILES=config.cache config.log config.h

# --------------- Maintainer's Section

MAINTAINERCLEANFILES=aclocal.m4 Makefile.in config.h.in stamp-h.in configure \
        ChangeLog EXAMPLES which-2.spec index.html cvslog-*.html README

## Bug in automake: Can't use `if MAINTAINER_MODE'
@MAINTAINER_MODE_TRUE@include $(srcdir)/maintMakefile

## Can't put this in the maintMakefile for some reason.
maintainer-clean-local:
	echo "all:" > Makefile
	echo -e "\t(cd tilde; $(MAKE) all)" >> Makefile
	echo -e "\tautoheader" >> Makefile
	echo -e "\tautomake --foreign" >> Makefile
	echo -e "\taclocal" >> Makefile
	echo -e "\tautoconf" >> Makefile
	echo -e "\tautoheader" >> Makefile
	echo -e "\tconfigure --enable-maintainer-mode" >> Makefile

## Can someone tell me what I am doing wrong, why do I need this?
$(top_builddir)/tilde/libtilde.a:
	( cd $(top_builddir)/tilde; $(MAKE) libtilde.a )

AUTOMAKE_OPTIONS = 1.3 foreign
noinst_LIBRARIES = libtilde.a
libtilde_a_SOURCES = tilde.c shell.c tilde.h ansi_stdlib.h

MAINTAINERCLEANFILES=Makefile.in aclocal.m4 configure

maintainer-clean-local:
	echo "all:" > Makefile
	echo -e "\taclocal" >> Makefile
	echo -e "\tautoheader" >> Makefile
	echo -e "\tautoconf" >> Makefile
	echo -e "\tautomake" >> Makefile

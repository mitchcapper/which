AUTOMAKE_OPTIONS = 1.3 foreign
noinst_LIBRARIES = libtilde.a
libtilde_a_SOURCES = tilde.c shell.c tilde.h ansi_stdlib.h
BUILT_SOURCES = aclocal.m4

MAINTAINERCLEANFILES=Makefile.in aclocal.m4 configure

maintainer-clean-local:
	echo "all:" > Makefile
	echo -e "\tautoheader" >> Makefile
	echo -e "\tautomake --foreign" >> Makefile
	echo -e "\taclocal" >> Makefile
	echo -e "\tautoconf" >> Makefile
	echo -e "\tautoheader" >> Makefile
	echo -e "\tconfigure --enable-maintainer-mode" >> Makefile

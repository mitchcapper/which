# Name and version of the pacakge
PACKAGE:=${shell grep AM_INIT_AUTOMAKE configure.in | cut -d, -f1 | cut -d'(' -f2 | sed -e 's% %%g'}
VERSION:=${shell grep AM_INIT_AUTOMAKE configure.in | cut -d, -f2 | cut -d')' -f1 | sed -e 's% %%g'}

# Do NOT include the following CVS files in the tar:
EXCLUDE_CVS_FILES=README.in .indent.pro test

# Put the following generated files also in the tar:
INCLUDE_NON_CVS_FILES=README EXAMPLES

# The directory where to put the release:
WWWDIR=$(HOME)/www/$(PACKAGE)

# RPM specifics:
REDHAT:=$(shell grep '^%_topdir' rpm/macros | cut -d \  -f 2)
SPECFILE=$(PACKAGE)-$(MAJOR_VERSION).spec
TAG=V$(MAJOR_VERSION)_$(MINOR_VERSION)
TARGET=i386
REL=1

#-----------------------------------------------------------------------------
srcdir=.
include $(PROTODIR)/maintainer/PTmakefile

# Generating the front page of the distribution site:
index.html: EXAMPLES which.1 index.html.in
	grep -B2000 '^CHANGELOGLIST' index.html.in | sed -e 's/@VERSION@/$(VERSION)/g' | grep -v '^CHANGELOGLIST' > index.html
	( VER=$(VERSION); \
	  while test "$$VER" != "2.0" ; do \
	    echo "<LI><A HREF="cvslog-$$VER.html">Version $$VER</A>" >> index.html; \
	    VER=`echo "$$VER" | awk -F. '{ printf("%d.%d", $$1, $$2 - 1); }'`; \
	  done \
	)
	grep -A2000 '^CHANGELOGLIST' index.html.in | grep -v '^CHANGELOGLIST' | grep -B2000 '^MANPAGE' | grep -v '^MANPAGE' >> index.html
	groff -man -Tascii which.1 | sed -e 's/.//g' | head -n 60 | tail -n 57 >> index.html
	groff -man -Tascii which.1 | sed -e 's/.//g' | head -n 128 | tail -n 56 | grep -B2000 '^ ' >> index.html
	grep -A2000 '^MANPAGE' index.html.in | grep -v '^MANPAGE' | grep -B2000 '^EXAMPLES' | grep -v '^EXAMPLES' >> index.html
	cat EXAMPLES >> index.html
	grep -A2000 '^EXAMPLES' index.html.in | grep -v '^EXAMPLES' >> index.html

EXAMPLES: which
	( cd test; whichtest check; ) || exit -1;
	( cd test; whichtest generate; )

README: which.1 README.in
	cp README.in README
	groff -man -Tascii which.1 | sed -e 's/.//g' | head -n 60 | tail -n 57 >> README
	groff -man -Tascii which.1 | sed -e 's/.//g' | head -n 128 | tail -n 56 | grep -B2000 '^ ' >> README

# This is executed when you type `make' (without target) before running configure.
setup-dist::
	configure --prefix=/usr


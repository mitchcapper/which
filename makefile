# which maintainers makefile
#
# Copyright (C) 1999
#
# 1024/624ACAD5 1997/01/26 Carlo Wood, Run on IRC <carlo@runaway.xs4all.nl>
# Key fingerprint = 32 EC A7 B6 AC DB 65 A6  F6 F6 55 DD 1C DC FF 61
#

srcdir=.
ifeq (Makefile,$(wildcard Makefile))
include Makefile
endif

VER:=${shell grep AM_INIT_AUTOMAKE configure.in | cut -d, -f2 | cut -d')' -f1 | sed -e 's% %%g'}
MAJOR_VERSION:=${shell echo "$(VER)" | cut -d. -f1}
MINOR_VERSION:=${shell echo "$(VER)" | cut -d. -f2}
TAG=V$(MAJOR_VERSION)_$(MINOR_VERSION)
PREVTAG=`cat .prevtag`
REL=1
REDHAT:=$(shell grep '^%_topdir' rpm/macros | cut -d \  -f 2)

.PHONY: release tar rpm

setup-dist:
	make README
	automake
	aclocal
	autoconf
	configure --prefix=/usr

release: tar index.html cvslog rpm
	install -m 644 -o carlo $(REDHAT)/SRPMS/which-$(VER)-$(REL).src.rpm /home/carlo/www/which
	install -m 644 -o carlo $(REDHAT)/RPMS/i386/which-$(VER)-$(REL).i386.rpm /home/carlo/www/which
	install -m 644 -o carlo $(REDHAT)/SOURCES/which-$(VER).tar.gz /home/carlo/www/which
	install -m 644 -o carlo index.html /home/carlo/www/which
	install -m 644 -o carlo cvslog-$(VER)*.html /home/carlo/www/which
	cvs tag $(TAG)
	echo "$(TAG)" > .prevtag
	date +%j > .release_day

tar: $(srcdir)/config.h.in $(ACLOCAL_M4) $(srcdir)/stamp-h.in ChangeLog README index.html Makefile.in configure which-2.spec
	rm -rf /tmp/which-$(VER)
	mkdir /tmp/which-$(VER)
	cp -p ChangeLog README index.html Makefile.in configure aclocal.m4 stamp-h.in config.h.in which-2.spec /tmp/which-$(VER)
	cp -pr .deps /tmp/which-$(VER)
	( for i in `find . -type d ! -name CVS ! -name .deps -print`; do \
	  files=`grep '^/' $$i/CVS/Entries | sed -e 's%^/%%' -e 's%/.*$$%%'`; \
	  if [ "$$i" != "." ]; then \
	    mkdir /tmp/which-$(VER)/$$i; \
	  fi; \
	  for j in $$files; do \
	    cp -p $$i/$$j /tmp/which-$(VER)/$$i/$$j; \
	  done; \
	done; \
	)
	rm /tmp/which-$(VER)/.cvsignore /tmp/which-$(VER)/makefile
	rm /tmp/which-$(VER)/README.in /tmp/which-$(VER)/index.html.in
	rm /tmp/which-$(VER)/.indent.pro /tmp/which-$(VER)/index.html
	rm /tmp/which-$(VER)/which-2.spec.in
	rm -rf /tmp/which-$(VER)/rpm
	tar czf $(REDHAT)/SOURCES/which-$(VER).tar.gz -C /tmp which-$(VER)
	rm -rf /tmp/which-$(VER)

README: which.1 README.in
	cp README.in README
	groff -man -Tascii which.1 | sed -e 's/.//g' | head -n 60 | tail -n 57 >> README
	groff -man -Tascii which.1 | sed -e 's/.//g' | head -n 128 | tail -n 56 | grep -B2000 '^ ' >> README

index.html: EXAMPLES which.1 index.html.in
	grep -B2000 '^CHANGELOGLIST' index.html.in | sed -e 's/@VERSION@/$(VER)/g' | grep -v '^CHANGELOGLIST' > index.html
	( VER=$(VER); \
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

cvslog:
	@( \
	   D1=`cat .release_day`; \
	   D2=`date +%j`; \
	   DD=`echo "$$D1 $$D2" | awk '{ printf("%d\n", $$2 - $$1) }'`; \
	   echo "Last release was $$DD days ago."; \
	   cvs2html -e -r$(PREVTAG):HEAD -o cvslog-$(VER); \
	   rm cvslog-$(VER)_rpm*; \
	 )

rpm: tar
	rpm --rcfile /usr/lib/rpm/rpmrc:rpm/rpmrc --target i386 --sign --clean -ba which-2.spec

which-2.spec: which-2.spec.in
	sed -e 's/@VERSION@/$(VER)/g' which-2.spec.in | grep -B2000 '@CHANGELOG@' | grep -v '@CHANGELOG@' > which-2.spec
	echo -n "%changelog" >> which-2.spec
	cvs log which-2.spec.in | \
	  grep -A2000 '^-----' | \
	  egrep -v '^-----|^=====|^revision' | \
	  awk -F'[ ;]' --assign lastdate="" '{ if ($$0~/^date: /) { \
	    if (lastdate != $$2) { \
	      lastdate=$$2; \
	      printf("\n* "); \
	      system("echo -n `date --date \""$$2" "$$3"\" \"+%a %b %d %Y\"`"); \
	      printf(" Carlo Wood <carlo@gnu.org>\n"); \
	    } \
	  } else if ($$0~/^[A-Z]/) { printf("- %s\n", $$0); } else if ($$0~/./) { printf("  %s\n", $$0); } else {print} }' >> which-2.spec
	grep -A2000 '@CHANGELOG@' which-2.spec.in | grep -v '@CHANGELOG@' >> which-2.spec

.PHONY: ChangeLog
ChangeLog:
	@( echo "/usr/local/bin/cvs \`echo \"\$$*\" | sed -e 's%1970%1990%'\`" > cvs; \
	    chmod 755 cvs; )
	rcs2log > ChangeLog
	@rm cvs

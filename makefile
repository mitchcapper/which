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
REL=1

.PHONY: release tar

setup-dist:
	make README
	automake
	aclocal
	autoconf

release: tar index.html cvslog
	install -m 644 -o carlo which-$(VER).tar.gz /home/carlo/www/which
	install -m 644 -o carlo index.html /home/carlo/www/which
	install -m 644 -o carlo cvslog-$(VER)*.html /home/carlo/www/which

tar: ChangeLog README
	rm -rf /tmp/which-$(VER)
	mkdir /tmp/which-$(VER)
	cp -p ChangeLog README index.html Makefile.in configure aclocal.m4 stamp-h.in /tmp/which-$(VER)
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
	rm -rf /tmp/which-$(VER)/rpm
	tar czf which-$(VER).tar.gz -C /tmp which-$(VER)
	rm -rf /tmp/which-$(VER)

README: which.1 README.in
	cp README.in README
	groff -man -Tascii which.1 | sed -e 's/.//g' | head -n 59 | tail -n 56 >> README
	groff -man -Tascii which.1 | sed -e 's/.//g' | head -n 128 | tail -n 56 | grep -B2000 '^ ' >> README

index.html: EXAMPLES which.1 index.html.in
	grep -B2000 '^MANPAGE' index.html.in | grep -v '^MANPAGE' > index.html
	groff -man -Tascii which.1 | sed -e 's/.//g' | head -n 59 | tail -n 56 >> index.html
	groff -man -Tascii which.1 | sed -e 's/.//g' | head -n 128 | tail -n 56 | grep -B2000 '^ ' >> index.html
	grep -A2000 '^MANPAGE' index.html.in | grep -v '^MANPAGE' | grep -B2000 '^EXAMPLES' | grep -v '^EXAMPLES' >> index.html
	cat EXAMPLES >> index.html
	grep -A2000 '^EXAMPLES' index.html.in | grep -v '^EXAMPLES' >> index.html

cvslog:
	@( cvs2html -e -D7 -o cvslog-$(VER) )

.PHONY: ChangeLog
ChangeLog:
	@( echo "/usr/local/bin/cvs \`echo \"\$$*\" | sed -e 's%1970%1990%'\`" > cvs; \
	    chmod 755 cvs; )
	rcs2log > ChangeLog
	@rm cvs

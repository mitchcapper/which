# which maintainers makefile
#
# Copyright (C) 1999
#
# 1024/624ACAD5 1997/01/26 Carlo Wood, Run on IRC <carlo@runaway.xs4all.nl>
# Key fingerprint = 32 EC A7 B6 AC DB 65 A6  F6 F6 55 DD 1C DC FF 61
#

include Makefile

MAJOR_VERSION=2
MINOR_VERSION=0
VER:=$(MAJOR_VERSION).$(MINOR_VERSION)
REL=1

.PHONY: release tar

release: rpm
	$(install) -m 644 -o carlo which-$(VER).tar.gz /home/carlo/www/which
	@( \
	  MINVER=`echo $(MINOR_VERSION) | awk -- '{ printf ("%d", $$0 + 1) }'`; \
	  echo "MINOR_VERSION=$$MINVER"; \
	  cp makefile makefile.bak && \
	  sed -e s%'^MINOR_VERSION=.*$$'%MINOR_VERSION=$$MINVER% makefile.bak > makefile || exit 1; \
	  rm makefile.bak; \
	)

tar:
	rm -rf /tmp/which-$(VER)
	mkdir /tmp/which-$(VER)
	( for i in `find . -type d ! -name CVS -print`; do \
	  files=`grep '^/' $$i/CVS/Entries | sed -e 's%^/%%' -e 's%/.*$$%%'`; \
	  if [ "$$i" != "." ]; then \
	    mkdir /tmp/which-$(VER)/$$i; \
	  fi; \
	  for j in $$files; do \
	    cp $$i/$$j /tmp/which-$(VER)/$$i/$$j; \
	  done; \
	done; \
	)
	rm /tmp/which-$(VER)/.cvsignore /tmp/which-$(VER)/makefile
	rm -rf /tmp/which-$(VER)/rpm
	tar czf which-$(VER).tar.gz -C /tmp which-$(VER)
	rm -rf /tmp/which-$(VER)

CC=gcc
CFLAGS=-Wall -g
prefix=/usr

which: which.o bash.o
	$(CC) -o which which.o bash.o /usr/lib/libreadline.a /usr/lib/libiberty.a

install:
	install -d -o root -m 755 -g bin       $(prefix)/bin
	install -s -o root -m 755 -g bin which $(prefix)/bin
	install -d -o root -m 755 -g root         $(prefix)/man/man1
	install    -o root -m 644 -g root which.1 $(prefix)/man/man1

clean:
	rm -f core *.orig *.o which troep cvslog* which-2.*.tar.gz

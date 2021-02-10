CC=gcc
CFLAGS=-pipe -O2 -Wall
LIBS=-lcrypto -lz
PREFIX=/usr/local
MANPREFIX=$(PREFIX)/share/man

make: 9it

all: 9it

%.o: %.c headers/%.h
	$(CC) $(CFLAGS) -c $<

9it: 9it.c fileutils.o options.o objutils.o nodeutils.o globals.o commands.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

install: all
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f 9it $(DESTDIR)$(PREFIX)/bin
	chmod 775 $(DESTDIR)$(PREFIX)/bin/9it
	mkdir -p $(DESTDIR)$(MANPREFIX)/man1
	cp -f 9it.1 $(DESTDIR)$(MANPREFIX)/man1

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/9it \
		$(DESTDIR)$(MANPREFIX)/man1/9it.1

clean:
	rm 9it *.o

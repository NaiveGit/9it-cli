CC=gcc
CFLAGS=-pipe -O2 -Wall
LIBS=-lcrypto -lz
INSTALL_PATH = /usr/local

make: 9it

all: 9it

%.o: %.c headers/%.h
	$(CC) $(CFLAGS) -c $<

9it: 9it.c fileutils.o options.o objutils.o nodeutils.o globals.o commands.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

install: all
	mkdir -p ${INSTALL_PATH}/bin
	cp -f 9it ${INSTALL_PATH}/bin
	chmod 775 ${INSTALL_PATH}/bin/9it
	mkdir -p ${INSTALL_PATH}/man/man1
	cp -f 9it.1 ${INSTALL_PATH}/man/man1

uninstall:
	rm -f ${INSTALL_PATH}/bin/9it \
		${INSTALL_PATH}/man/man1/9it.1

clean:
	rm 9it *.o

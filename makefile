CC=gcc
CFLAGS=-pipe -O2 -Wall
LIBS=-lcrypto -lz
INSTALL_PATH=/usr/bin
DOC_PATH=/usr/share/man/man1

make: 9it

all: 9it

%.o: %.c headers/%.h
	$(CC) $(CFLAGS) -c $<

9it: 9it.c fileutils.o options.o objutils.o nodeutils.o globals.o commands.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

install: all
	mkdir -p ${INSTALL_PATH}
	cp -f 9it ${INSTALL_PATH}
	chmod 775 ${INSTALL_PATH}/9it
	mkdir -p ${DOC_PATH}
	cp -f 9it.1 ${DOC_PATH}

uninstall:
	rm -f ${INSTALL_PATH}/9it \
		${DOC_PATH}/9it.1

clean:
	rm 9it *.o

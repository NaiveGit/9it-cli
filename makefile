CC=gcc

INSTALL_PATH = /usr/local


make: 9it

all: 9it

# %.o: %.c headers/%.h
# 	$(CC) -c $^

globals.o: globals.c headers/globals.h
	$(CC) -c globals.c

fileutils.o: fileutils.c headers/fileutils.h
	$(CC) -c fileutils.c

options.o: options.c headers/options.h
	$(CC) -c options.c

objutils.o: objutils.c headers/objutils.h
	$(CC) -c objutils.c

nodeutils.o: nodeutils.c headers/nodeutils.h
	$(CC) -c nodeutils.c

commands.o: commands.c headers/commands.h
	$(CC) -c commands.c

9it: 9it.c fileutils.o options.o objutils.o nodeutils.o globals.o commands.o
	$(CC) -o $@ $^ -lcrypto -lz 

install: all
	mkdir -p ${INSTALL_PATH}/bin
	cp -f 9it ${INSTALL_PATH}/bin
	chmod 775 ${INSTALL_PATH}/bin/9it
	mkdir -p ${INSTALL_PATH}/man/man1
	mkdir -p ${INSTALL_PATH}/man/man3
	cp -f docs/9it.1 ${INSTALL_PATH}/man/man1
	cp -f docs/9it-objutils.3 ${INSTALL_PATH}/man/man3
	
uninstall:
	rm -f ${INSTALL_PATH}/bin/9it \
		${INSTALL_PATH}/man/man1/9it.1 \
		${INSTALL_PATH}/man/man3/9it-objutils.3

clean:
	rm 9it

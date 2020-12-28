CC=gcc

make: 9it

# %.o: %.c headers/%.h
# 	$(CC) -c $^

globals.o: headers/globals.h
	$(CC) -c $^

fileutils.o: fileutils.c headers/fileutils.h
	$(CC) -c fileutils.c

options.o: options.c headers/options.h
	$(CC) -c options.c

objutils.o: objutils.c headers/objutils.h
	$(CC) -c objutils.c

nodeutils.o: nodeutils.c headers/nodeutils.h
	$(CC) -c nodeutils.c

9it: 9it.c fileutils.o options.o objutils.o nodeutils.o
	$(CC) -o $@ $^ -lcrypto -lz

clean:
	rm 9it

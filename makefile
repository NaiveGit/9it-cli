CC=gcc

make: 9it

# %.o: %.c headers/%.h
# 	$(CC) -c $^

globals.o: headers/globals.h
	$(CC) -c $^

fileutils.o: fileutils.c headers/fileutils.h
	$(CC) -c $^

options.o: options.c headers/options.h
	$(CC) -c $^

objutils.o: objutils.c headers/objutils.h
	$(CC) -c $^

9it: 9it.c fileutils.o options.o objutils.o
	$(CC) -lcrypto -lz -o $@ $^

clean:
	rm 9it

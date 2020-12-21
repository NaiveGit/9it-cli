CC=gcc

make: 9it

options.o: options.c headers/options.h
	$(CC) -c options.c

objutils.h: objutils.c headers/objutils.h
	$(CC) -c objutils.c

9it: 9it.c options.o
	$(CC) -lcrypto -o $@ $^

clean:
	rm 9it

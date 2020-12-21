CC=gcc

make: 9it

options.o: options.c options.h
	$(CC) -c options.c

9it: 9it.c options.o
	$(CC) -o $@ $^

clean:
	rm 9it

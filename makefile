CC=gcc

make: 9it

%.o: %.c headers/%.h
	$(CC) -c $^

9it: 9it.c fileutils.o options.o objutils.o
	$(CC) -lcrypto -lz -o $@ $^

clean:
	rm 9it

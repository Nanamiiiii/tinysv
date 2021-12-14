# Makefile
MAKE = make
CC = gcc
CFLAGS = -O2 -Wall -I../include
TESTS = hashmap hashmap_int

.PHONY: all
all:
	$(MAKE) $(TESTS)

.PHONY: hashmap
hashmap: hashmap_test.o ../src/hashmap.o
	$(CC) hashmap_test.o ../src/hashmap.o -o hashmap_test

hashmap_test.o: hashmap_test.c
	$(CC) hashmap_test.c -c $(CFLAGS)

../src/hashmap.o: ../src/Makefile
	$(MAKE) -C ../src hashmap.o

.PHONY: hashmap_int
hashmap_int: hashmap_int_test.o ../src/hashmap_int.o
	$(CC) hashmap_int_test.o ../src/hashmap_int.o -o hashmap_int_test

hasmap_int_test.o: hashmap_int_test.c
	$(CC) hashmap_int_test.c -c $(CFLAGS)

../src/hashmap_int.o: ../src/Makefile
	$(MAKE) -C ../src hashmap_int.o

.PHONY: clean
clean:
	rm -f *.o *_test

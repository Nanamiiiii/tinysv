# Makefile

MAKE = make
CC = gcc
CFLAGS = -O2 -Wall -fPIC -I../include -c
LDFLAGS = -shared
OBJS = default_module.so
SRCS = $(OBJS:.so=.c)

.PHONY: all
all: $(OBJS)

%.so: %.o hashmap.o logger.o
	$(CC) $(LDFLAGS) $*.o hashmap.o logger.o -o $*.so

hashmap.o: ../src/hashmap.c
	$(CC) $(CFLAGS) $< -o ./$@

logger.o: ../src/logger.c
	$(CC) $(CFLAGS) $< -o ./$@

%.o: %.c
	$(CC) $(CFLAGS) $*.c -o ./$@

.PHONY: clean
clean:
	rm -f $(OBJS)
	rm -f hashmap.o logger.o

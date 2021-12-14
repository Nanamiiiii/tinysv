# Makefile

MAKE = make
CC = gcc
CFLAGS = -O2 -Wall -I../include -shared
OBJS = default_module.so
SRCS = $(OBJS:.so=.c)

.PHONY: all
all: $(OBJS)

%.so: %.c
	$(CC) $(CFLAGS) $*.c -o $*.so

.PHONY: clean
clean:
	rm -f $(OBJS)

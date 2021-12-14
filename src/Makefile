# Makefile
MAKE = make
CC = gcc
CFLAGS = -O2 -Wall -I../include
LDFLAGS = -lpthread -lexpat -ldl
TARGET = ../tinysv
OBJS = main.o logger.o hashmap.o hashmap_int.o config.o module.o
SRCS = $(OBJS:.o=.c)

.PHONY: all
all: $(OBJS) $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c $*.c $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)

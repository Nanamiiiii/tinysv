# Makefile

CC = gcc
CFLAGS = -O2 -Wall -lpthread
TARGET = tinysv
OBJS = main.o logger.o hashmap.o
SRCS = $(OBJS:.o=.c)

all: $(OBJS) $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) -c $(SRCS)

clean:
	rm -f $(OBJS) $(TARGET)

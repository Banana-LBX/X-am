CC = gcc

CFLAGS = -Wall -Wextra -std=c11 -I. -Isrc
LIBS = -lraylib -lm -lpthread -ldl -lrt

TARGET = app

SRC = $(wildcard src/*.c)

all:
	$(CC) $(SRC) $(CFLAGS) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)

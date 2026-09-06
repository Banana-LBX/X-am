CC = gcc

CFLAGS = -Wall -Wextra -std=c11
LIBS = -lraylib -lm -lpthread -ldl -lrt

TARGET = app
	SRC = main.c

all:
	$(CC) $(SRC) $(CFLAGS) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)


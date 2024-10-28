CC = gcc
CFLAGS = -pthread -Wall -Wextra
TARGET = assignment3

all: $(TARGET)

$(TARGET): assignment3.c
	$(CC) $(CFLAGS) -o $(TARGET) assignment3.c

clean:
	rm -f $(TARGET)

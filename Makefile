CC = gcc
CFLAGS = -pthread
TARGET = assignment3
# TEST_TARGET = Test_assignment3

all: $(TARGET) $(TEST_TARGET)

$(TARGET): assignment3.c
	$(CC) $(CFLAGS) -o $(TARGET) assignment3.c

# $(TEST_TARGET): Test_assignment3.c
# 	$(CC) -o $(TEST_TARGET) Test_assignment3.c

clean:
	rm -f $(TARGET) $(TEST_TARGET)

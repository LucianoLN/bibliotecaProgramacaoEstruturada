CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:.c=.o)
TARGET = biblioteca

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	-rm -f $(OBJS) $(TARGET)

run: all
	./$(TARGET)

test: all
	$(CC) $(CFLAGS) -Itests -o test/tests tests/test_books.c $(SRCS)
	./test/tests

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
TARGET = parser_visualizer
OBJS = main.o parser.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

main.o: main.c parser.h
	$(CC) $(CFLAGS) -c main.c

parser.o: parser.c parser.h
	$(CC) $(CFLAGS) -c parser.c

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run

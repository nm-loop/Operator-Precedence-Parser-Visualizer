# ============================================
# Makefile for Operator Precedence Parser
# ============================================

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
TARGET = parser_visualizer
OBJS = main.o parser.o

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# Compile main.c
main.o: main.c parser.h
	$(CC) $(CFLAGS) -c main.c

# Compile parser.c
parser.o: parser.c parser.h
	$(CC) $(CFLAGS) -c parser.c

# Clean build files
clean:
	rm -f $(OBJS) $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Debug with gdb
debug: $(TARGET)
	gdb ./$(TARGET)

# Memory leak check with valgrind
memcheck: $(TARGET)
	valgrind --leak-check=full ./$(TARGET)

.PHONY: all clean run debug memcheck

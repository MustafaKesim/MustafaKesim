# Makefile for Adventure Game

# Compiler and flags
CC = gcc
CFLAGS = -Wall -g

# Target executable
TARGET = adventure_game

# Source file
SRC = adventuregame.c

# Default rule to build the game
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Run the game
run: $(TARGET)
	./$(TARGET)

# Clean up compiled files
clean:
	rm -f $(TARGET)
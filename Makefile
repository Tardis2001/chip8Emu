
CC = gcc

CFLAGS = -Wall -Wextra -std=c99

RAYLIB_CFLAGS = -I/usr/include  
RAYLIB_LIBS   = -L/usr/lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Project name
TARGET = chip8

# Source and object files
SRC_DIR = src
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(CFLAGS) $(RAYLIB_CFLAGS) $(RAYLIB_LIBS)

# Compile
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(RAYLIB_CFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(OBJS) $(TARGET)

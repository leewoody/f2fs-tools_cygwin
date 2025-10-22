# Makefile for fsck_dir - Simplified F2FS directory tree viewer

CC = gcc
CFLAGS = -Wall -O2 -I./include
LIBS = -lm

# Source files
LIB_SOURCES = libf2fs_simplified.c
FSCK_SOURCES = fsck_dir.c
SOURCES = $(LIB_SOURCES) $(FSCK_SOURCES)
OBJECTS = $(SOURCES:.c=.o)
TARGET = fsck_dir

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(LIBS)

# Compile source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJECTS) $(TARGET)

# Install the executable
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

# Uninstall the executable
uninstall:
	rm -f /usr/local/bin/$(TARGET)

.PHONY: all clean install uninstall
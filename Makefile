# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c99

# Directories
INCDIR = include
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# Source files
SRC = $(wildcard $(SRCDIR)/*.c)

# Object files
OBJ = $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Executable
TARGET = $(BINDIR)/vdisk

# Include directories
INCLUDES = -I$(INCDIR)

# Default target
all: $(TARGET)

# Create directories if they don't exist
$(OBJDIR):
    mkdir -p $(OBJDIR)

$(BINDIR):
    mkdir -p $(BINDIR)

# Build the target
$(TARGET): $(OBJ) | $(BINDIR)
    $(CC) $(CFLAGS) $(OBJ) -o $@

# Build object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
    $(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean up
clean:
    rm -rf $(OBJDIR) $(BINDIR)

# Phony targets
.PHONY: all clean
compiler ?=gcc
OUTPUT := out
BUILD_DIR := build

CC := $(CROSS_COMPILE)gcc

all: clean writer



writer: writer.c
	@$(CC) writer.c -o writer

clean:
	@rm -f *.o 
	@rm -f writer


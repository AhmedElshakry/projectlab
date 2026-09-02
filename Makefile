# Smart Home Console - G9
# Day 03 midterm - Microcontroller-Based Systems
# Author: Ahmed Ellamie <ahmed.ellamiee@gmail.com>

CC      = gcc
CFLAGS  = -std=c99 -Wall -Wextra -Iinclude
TARGET  = house
SRC     = main.c src/house.c src/render.c src/ui.c src/platform.c src/demo.c
OBJ     = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

run: $(TARGET)
	./$(TARGET)

clean:
	-rm -f $(OBJ) $(TARGET) $(TARGET).exe

.PHONY: all run clean

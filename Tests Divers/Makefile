CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -fsanitize=address
TARGET = test
LDFLAGS = -g -fsanitize=address

#SRC = main.c
SRC = $(wildcard src/*.c)

OBJ = $(SRC:.c=.o)
all: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $(TARGET)
	./test


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	-rm *.o
	-rm $(TARGET)
.PHONY: all clean

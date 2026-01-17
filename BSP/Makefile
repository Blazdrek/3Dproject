TARGET = mon_fichier
SRC = $(wildcard src/*.c)

$(TARGET) : $(SRC)
	clear
	gcc $(SRC) $(shell sdl2-config --cflags --libs) -lm -o bin/$(TARGET)
	./bin/$(TARGET)
CFLAGS = -Iassets -lm $(shell sdl2-config --cflags --libs)
SRC = $(wildcard src/*.c)
TARGET = editeur.out

$(TARGET) : $(SRC)
	clear
	gcc $(SRC) $(CFLAGS) -o bin/$(TARGET)
	./bin/$(TARGET)
	@echo "\n<Programme lancé avec succès>\n"


SHELL := /bin/bash

build:
	g++ src/*.cpp -o gameengine $$(sdl2-config --cflags --libs) \
		-I/mingw64/include -L/mingw64/lib -I"./lib/" \
		-lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua -o gameengine

run:
	./gameengine;
clean:
	rm gameengine;
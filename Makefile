SHELL := /bin/bash

build:
	g++ src/*.cpp -o gameengine $$(sdl2-config --cflags --libs) -lSDL2;
run:
	./gameengine;
clean:
	rm gameengine;
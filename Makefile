SHELL := /bin/bash

build:
	g++ src/*.cpp -o gameengine $$(sdl2-config --cflags --libs);
run:
	./gameengine;
clean:
	rm gameengine;
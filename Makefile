SOURCES = ./src/main.cpp

default:
	g++ $(SOURCES) -o ./build/game.exe -g -O1 -Wall -Wno-missing-braces -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm 
	./build/game.exe
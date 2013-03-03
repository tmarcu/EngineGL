CC=g++
CFLAGS=-c -Wall -g
LDFLAGS=`sdl-config --cflags --libs` -lGL -lGLU -lSDL

sdlprog: game_engine.o camera.o model_loader.o game.o
	$(CC) -o sdlprog game_engine.o camera.o model_loader.o game.o $(LDFLAGS)
    
game_engine.o: game_engine.cpp game_engine.h structs.h
	$(CC) $(CFLAGS) game_engine.cpp

game.o: game.cpp game.h
	$(CC) $(CFLAGS) game.cpp

camera.o: camera.cpp camera.h structs.h
	$(CC) $(CFLAGS) camera.cpp
    
model_loader.o: model_loader.cpp model_loader.h structs.h
	$(CC) $(CFLAGS) model_loader.cpp 
    
clean:
	rm -rf *o *.gch sdlprog


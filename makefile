CC=g++
CFLAGS=-c -Wall -g
LDFLAGS=`sdl-config --cflags --libs` -lGL -lGLU -lSDL
INCLUDES=$(CUR_DIR)include/

sdlprog: game_engine.o camera.o model_loader.o game.o
	$(CC) -o sdlprog game_engine.o camera.o model_loader.o game.o $(LDFLAGS)
    
game_engine.o: src/game_engine.cpp include/game_engine.h include/structs.h
	$(CC) $(CFLAGS) src/game_engine.cpp -I$(INCLUDES)

game.o: src/game.cpp include/game.h
	$(CC) $(CFLAGS) src/game.cpp -I$(INCLUDES)

camera.o: src/camera.cpp include/camera.h include/structs.h
	$(CC) $(CFLAGS) src/camera.cpp -I$(INCLUDES)
    
model_loader.o: src/model_loader.cpp include/model_loader.h include/structs.h
	$(CC) $(CFLAGS) src/model_loader.cpp  -I$(INCLUDES)
    
clean:
	rm -rf *o *.gch sdlprog


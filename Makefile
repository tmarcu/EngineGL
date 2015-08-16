CC=g++
CFLAGS=-c -Wall -O3 -march=corei7-avx
LDFLAGS= -lGLEW -lGL -lGLU -lSDL2
INCLUDES=$(CUR_DIR)include/

sdlprog: src/game_engine.o src/camera.o src/model.o src/game.o
	$(CC) -o sdlprog src/game_engine.o src/camera.o src/model.o src/game.o -I$(INCLUDES) $(LDFLAGS)
    
src/game_engine.o: src/game_engine.cpp include/game_engine.h include/structs.h
	$(CC) $(CFLAGS) $< -I$(INCLUDES) -o $@

src/game.o: src/game.cpp include/game.h
	$(CC) $(CFLAGS) $< -I$(INCLUDES) -o $@

src/camera.o: src/camera.cpp include/camera.h include/structs.h
	$(CC) $(CFLAGS) $< -I$(INCLUDES) -o $@
    
src/model.o: src/model.cpp include/model.h include/structs.h
	$(CC) $(CFLAGS) $< -I$(INCLUDES) -o $@
    
clean:
	rm -rf *.o src/*.o *.gch sdlprog

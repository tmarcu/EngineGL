/*
 * EngineGL
 * Copyright (C) 2013 Tudor Marcu. All rights reserved.
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
//#include <SDL_OpenGL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include "structs.h"
#include "camera.h"
#include "model_loader.h"
void ReadData(char *filename, int numPoints, Polygon *triangle);
/* Game functions */
int InitializeGame();
int StartGame();
void EndGame();
void HandleCycle();
void HandleKeyPress(SDL_keysym *keysym);
void HandleKeyPressCamera(SDL_keysym *keysym, Camera *camera);
void MouseButtonUp(int x, int y, int button);
void MouseButtonDown(int x, int y, int button);
void RenderGame();

class GameEngine
{
public:
	/* Constructors and destructor */
	GameEngine(int screen_width, int screen_height, int screen_bpp);

	virtual ~GameEngine();

	/* Game engine methods */
	static GameEngine *GetEngine() { return game_engine;};

	int get_videoflags() { return videoflags_;};
	void set_videoflags(int videoflags);

	SDL_Surface *get_surface() { return surface_;};
	void set_surface(SDL_Surface *surface);

	const SDL_VideoInfo *get_kvideoinfo() { return kvideoinfo_;};

	Camera *get_camera() { return camera_;};
	ModelLoader *get_mode() { return model_;};

	int SetupSDL(int sdl_hardware,	/* Change to use hardware or software mode */
				int screen_width, 
				int screen_height,
				int screen_bpp,
				char *title, char *icon);
	int InitializeGL();		/* Set up all of the openGL needed */
	void QuitProgram();		/* Free appropriate memory */
	void Render();			/* Render the scene after all drawing done */
	void HandleEvent(SDL_Event event);  /* Handle incoming events */
	void HandleKeystate();		/* Handle multiple keypresses */
	void CameraLook();		/* Equivalent of gluLookAt*/
	void ResizeWindow(int width, int height);

private:
	int screen_width_;
	int screen_height_;
	int screen_bpp_;
	Uint8 *keystate_;
	const SDL_VideoInfo *kvideoinfo_;
	SDL_Surface *surface_;
	Camera *camera_;
	ModelLoader *model_;

	static Vector3D camera_position_;
	static Vector3D camera_center_;
	static Vector3D camera_up_;

protected:
	static GameEngine *game_engine;
	int videoflags_;
};














































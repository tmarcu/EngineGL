/*
 * EngineGL
 * Copyright (C) 2013 Tudor Marcu. All rights reserved.
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
//#include <SDL_OpenGL.h>
#include <GL/glew.h>
//#include <GL/gl.h>
//#include <GL/glu.h>

#include <math.h>
#include "structs.h"
#include "camera.h"
#include "model_loader.h"

/* Game functions */
bool InitializeGame(void);
bool StartGame(void);
void EndGame(void);
void HandleCycle(void);
void HandleKeyPress(SDL_Keysym *keysym);
void HandleKeyPressCamera(SDL_Keysym *keysym, Camera *camera);
void MouseButtonUp(int x, int y, int button);
void MouseButtonDown(int x, int y, int button);
void RenderGame(void);

class GameEngine
{
public:
	/* Constructors and destructor */
	GameEngine(int screen_width, int screen_height, int screen_bpp);

	virtual ~GameEngine();

	/* Game engine methods */
	static GameEngine *GetEngine() { return game_engine;};
	bool IsActive() { return isactive;};
	bool IsRunning() { return isrunning;};
	void  SetActive(bool a) { isactive = a;};
	void SetRunning(bool r) { isrunning = r;};

	SDL_Window *GetWindow(void) { return window_;};
	void SetGLContext(SDL_GLContext *context);
	Camera *GetCamera() { return camera_;};
	ModelLoader *get_model() { return model_;};

	bool SetupSDL(const int screen_width, const int screen_height);
	bool InitializeGL(void);		/* Set up all of the openGL needed */
	void QuitProgram(void);		/* Free appropriate memory */
	void Render(SDL_Window *window);			/* Render the scene after all drawing done */
	void HandleEvent(SDL_Event event);  /* Handle incoming events */
	void HandleKeystate(void);		/* Handle multiple keypresses */
	void CameraLook(void);		/* Equivalent of gluLookAt*/
	void ResizeWindow(int width, int height);

private:
	bool isactive;
	bool isrunning;
	int screen_width_;
	int screen_height_;
	int screen_bpp_;
	const Uint8 *keystate_;
	SDL_Window *window_;
	SDL_GLContext glcontext_;
	Camera *camera_;
	ModelLoader *model_;

	static Vector3D camera_position_;
	static Vector3D camera_center_;
	static Vector3D camera_up_;

protected:
	static GameEngine *game_engine;
	int videoflags_;
};

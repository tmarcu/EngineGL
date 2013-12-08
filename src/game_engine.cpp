/*
 * EngineGL
 * Copyright (C) 2013 Tudor Marcu. All rights reserved.
 */

#include <iostream>

#include "game_engine.h"

#define DEBUG 1  /* Used for enabling verbose output to test program */

GameEngine *GameEngine::game_engine = NULL;

/* What we use to initialize the camera */
Vector3D GameEngine::camera_position_ = {0.0f, 0.0f, 1.0f};
Vector3D GameEngine::camera_center_ = {0.0f, 0.0f, 1.0f};
Vector3D GameEngine::camera_up_ = {0.0f, 1.0f, 0.0f};

/* Constructors and destructors */
GameEngine::GameEngine(int screen_width, int screen_height, int screen_bpp)
{
	game_engine = this;
	screen_width_ = screen_width;
	screen_height_ = screen_height;
	screen_bpp_ = screen_bpp;
}

GameEngine::~GameEngine(void)
{
	SDL_Quit();
}

/* Load our model and set up the main GL parameters */
int GameEngine::InitializeGL(void)
{
	camera_ = new Camera(camera_position_, camera_center_, camera_up_, 0.01f, 0.1f);

	/* Enable smooth shading in our program */
	glShadeModel(GL_SMOOTH);

	/* Set the background black */
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	/* Set up depth buffer */
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

	/* Which depth test we should use */
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);


	/* Gives us pretty calculations for perspective */
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	return 0;
}

/* Release all memory and handles to program before destroying it */
void GameEngine::QuitProgram(void)
{
	if (keystate_ != NULL)
		delete keystate_;
	if (camera_ != NULL)	
		delete(camera_);
	SDL_DestroyWindow(window_);
	SDL_GL_DeleteContext(glcontext_);
}

/* function to reset our viewport after a window resize */
void GameEngine::ResizeWindow(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	/* Calculate The Aspect Ratio And Set The Clipping Volume */
	if (height == 0)
		height = 1;
	gluPerspective(45.0f, (float)width/height, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/* Set gluLookAt dynamically to what the camera coordinates/vectors are */
void GameEngine::CameraLook(void)
{
	gluLookAt(camera_->GetCameraPosition().x,
		  camera_->GetCameraPosition().y,
		  camera_->GetCameraPosition().z,
		  camera_->GetCameraPosition().x + camera_->GetCameraCenter().x, 
		  camera_->GetCameraPosition().y + camera_->GetCameraCenter().y, 
		  camera_->GetCameraPosition().z + camera_->GetCameraCenter().z,
		  0, 1, 0);
}

/* Handle the keystates so that we can do multiple keypresses */
void GameEngine::HandleKeystate(void)
{
	keystate_ = SDL_GetKeyboardState(NULL);

	if(keystate_[SDLK_w])
		camera_->MoveCamera(UP);
	if(keystate_[SDLK_s])
		camera_->MoveCamera(DOWN);
	if(keystate_[SDLK_a])
		camera_->MoveCamera(LEFT);
	if(keystate_[SDLK_d])
		camera_->MoveCamera(RIGHT);
}

/* Drawing code */
void GameEngine::Render(SDL_Window *window)
{
	/* Clear The Screen And The Depth Buffer*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Change to model matrix */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); /* Reset the current modelview matrix */
	GameEngine::GetEngine()->CameraLook(); /* Where gluLookAt(...) would normally appear */

	/* Draw whatever is specified for the program */
	RenderGame();
	glPushMatrix();	/* Push matrix stack */
	glTranslatef(3.0f, 0.0f, 5.0f);	/* Begin 3 right, 0 up, 5 back (towards screen) */
	glBegin( GL_TRIANGLES );             /* Drawing Using Triangles       */
	glColor3f(   1.0f,  0.0f,  0.0f ); /* Red                           */
	glVertex3f(  0.0f,  1.0f,  0.0f ); /* Top Of Triangle (Front)       */
	glColor3f(   0.0f,  1.0f,  0.0f ); /* Green                         */
	glVertex3f( -1.0f, -1.0f,  1.0f ); /* Left Of Triangle (Front)      */
	glColor3f(   0.0f,  0.0f,  1.0f ); /* Blue                          */
	glVertex3f(  1.0f, -1.0f,  1.0f ); /* Right Of Triangle (Front)     */

	glColor3f(   1.0f,  0.0f,  0.0f ); /* Red                           */
	glVertex3f(  0.0f,  1.0f,  0.0f ); /* Top Of Triangle (Right)       */
	glColor3f(   0.0f,  0.0f,  1.0f ); /* Blue                          */
	glVertex3f(  1.0f, -1.0f,  1.0f ); /* Left Of Triangle (Right)      */
	glColor3f(   0.0f,  1.0f,  0.0f ); /* Green                         */
	glVertex3f(  1.0f, -1.0f, -1.0f ); /* Right Of Triangle (Right)     */

	glColor3f(   1.0f,  0.0f,  0.0f ); /* Red                           */
	glVertex3f(  0.0f,  1.0f,  0.0f ); /* Top Of Triangle (Back)        */
	glColor3f(   0.0f,  1.0f,  0.0f ); /* Green                         */
	glVertex3f(  1.0f, -1.0f, -1.0f ); /* Left Of Triangle (Back)       */
	glColor3f(   0.0f,  0.0f,  1.0f ); /* Blue                          */
	glVertex3f( -1.0f, -1.0f, -1.0f ); /* Right Of Triangle (Back)      */

	glColor3f(   1.0f,  0.0f,  0.0f ); /* Red                           */
	glVertex3f(  0.0f,  1.0f,  0.0f ); /* Top Of Triangle (Left)        */
	glColor3f(   0.0f,  0.0f,  1.0f ); /* Blue                          */
	glVertex3f( -1.0f, -1.0f, -1.0f ); /* Left Of Triangle (Left)       */
	glColor3f(   0.0f,  1.0f,  0.0f ); /* Green                         */
	glVertex3f( -1.0f, -1.0f,  1.0f ); /* Right Of Triangle (Left)      */
	glEnd();  
	glPopMatrix(); /* Pop matrix stack */

	/* Actually draw everything to the screen */
	SDL_GL_SwapWindow(window);
}

bool GameEngine::SetupSDL(const int screen_width, const int screen_height)
{
	/* Initialize SDL */
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Video initialization failed: %s\n", SDL_GetError());
		return false;
	}

	window_ = SDL_CreateWindow("SDLPROG", SDL_WINDOWPOS_CENTERED,
					SDL_WINDOWPOS_CENTERED,
					screen_width, screen_height,
					SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if (window_ == NULL) {
		std::cerr << "Failed to create window!" << SDL_GetError() << std::endl;
		return false;
	}

	SDL_GLContext glcontext = SDL_GL_CreateContext(window_);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	return true;
}

/* Mainly handle window events */
void GameEngine::HandleEvent(SDL_Event event)
{
	switch(event.type) {
	case SDL_WINDOWEVENT_SIZE_CHANGED:
		std::cout << "OMG WINDOW RESIZE" << std::endl;
		ResizeWindow(event.window.data1, event.window.data2);
		break;
	case SDL_KEYDOWN:
		HandleKeyPress(&event.key.keysym);
		break;
		case SDL_MOUSEBUTTONUP:
		MouseButtonUp(event.button.x, event.button.y, event.button.button);
		break;
	case SDL_MOUSEBUTTONDOWN:
		MouseButtonDown(event.button.x, event.button.y, event.button.button);
		break;
	case SDL_MOUSEMOTION:
		/* Fix: Never stored the relative values, which base the camera
		* movement off of the displacement. Without this, WarpMouse will
		* add/subtract the amount the mouse moves to the camera.
		*/
		camera_->SetPrevPosition(event.motion.xrel, event.motion.yrel);
		camera_->MouseMoved(event.motion.x, event.motion.y, HALF_WIDTH, HALF_HEIGHT);
		break;
	case SDL_QUIT:
		GameEngine::GetEngine()->QuitProgram();
		break;
	default:
		break;
	}
}

int main(int argc, char *argv[])
{
	SDL_Event event;
	SDL_Window *window = NULL;
	bool done = false;
	bool isActive = true;

	/* Only run the program if we successfully create the game engine */
	if (InitializeGame()) {
		if (GameEngine::GetEngine()->InitializeGL()) {
			fprintf(stderr, "Could not initialize OpenGL");
			GameEngine::GetEngine()->QuitProgram();
		}


		/* Initialize SDL */
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			fprintf(stderr, "Video initialization failed: %s\n", SDL_GetError());
			return -1;
		}

		if (GameEngine::GetEngine()->SetupSDL(640, 480) != true) {
			return -1;
		}

		window = GameEngine::GetEngine()->GetWindow();

		/* Enable or disable for real FPS view */
		SDL_ShowCursor(SDL_DISABLE);

		/* Resize our initial window */
		GameEngine::GetEngine()->ResizeWindow(SCREEN_WIDTH, SCREEN_HEIGHT);
			SDL_SetRelativeMouseMode(SDL_TRUE);

		/* Start event loop to handle program now */
		while (!done) {
			/* Handle whatever events may be in the queue */
			while (SDL_PollEvent(&event)) {
				GameEngine::GetEngine()->HandleEvent(event);
			}

			/* this makes sure the camera will still have speed when it reaches the minimum  */
			if(GameEngine::GetEngine()->get_camera()->GetMoveSpeed() <= 0.05f)
				GameEngine::GetEngine()->get_camera()->SetMoveSpeed(0.05f);

			GameEngine::GetEngine()->HandleKeystate(); 
			/* Draw scene */
			if (isActive) {
				GameEngine::GetEngine()->Render(window); // draw our scene
			}
		}
	}
	/* Properly release everything and exit */
	GameEngine::GetEngine()->QuitProgram();
	EndGame();

	return 0;
}

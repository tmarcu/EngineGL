/*
 * EngineGL
 * Copyright (C) 2013 Tudor Marcu. All rights reserved.
 */

#include "game_engine.h"

#define DEBUG 1  /* Used for enabling verbose output to test program */

GameEngine *GameEngine::game_engine = NULL;

FILE *file;
char filename[] = "ch.obj_new";
int numVertices;
Polygon *triangle;
Vector3D *points;

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

GameEngine::~GameEngine()
{
	SDL_Quit();
}

/* Load our model and set up the main GL parameters */
int GameEngine::InitializeGL()
{
	camera_ = new Camera(camera_position_, camera_center_, camera_up_, 0.01f, 0.1f);
	model_ = new ModelLoader();
	model_->countVertices(file, filename);
	numVertices = model_->get_vertices();
	triangle = new Polygon[numVertices/3];
	ReadData(filename, numVertices, triangle);

	/* Enable smooth shading in our program */
	glShadeModel(GL_SMOOTH);

	/* Set the background black */
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	/* Set up depth buffer */
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

	/* Which depth test we should use */
	glDepthFunc(GL_LEQUAL);

	/*glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); */


	/* Gives us pretty calculations for perspective */
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	return 0;
}

/* Release all memory and handles to program before destroying it */
void GameEngine::QuitProgram()
{
	if (points != NULL)
		free(points);
	if (keystate_ != NULL)
		delete keystate_;
	if (surface_ != NULL)
		delete surface_;
	if (camera_ != NULL)	
		delete(camera_);
	if (triangle != NULL)
		delete[] triangle;
}

/* function to reset our viewport after a window resize */
void GameEngine::ResizeWindow(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	/* Calculate The Aspect Ratio And Set The Clipping Volume */
	if (height == 0) height = 1;
	gluPerspective(45.0f, (float)width/(float)height, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/* Set gluLookAt dynamically to what the camera coordinates/vectors are */
void GameEngine::CameraLook()
{
	gluLookAt(camera_->GetCameraPosition().x,camera_->GetCameraPosition().y,
		  camera_->GetCameraPosition().z,
		  camera_->GetCameraPosition().x + camera_->GetCameraCenter().x, 
		  camera_->GetCameraPosition().y + camera_->GetCameraCenter().y, 
		  camera_->GetCameraPosition().z + camera_->GetCameraCenter().z,
		  0, 1, 0);
}

/* Handle the keystates so that we can do multiple keypresses */
void GameEngine::HandleKeystate(void)
{
	keystate_ = SDL_GetKeyState(0);

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
void GameEngine::Render()
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
	glBegin( GL_TRIANGLES );               /* Draw A Quad                      */
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
	glEnd( );  
	glPopMatrix(); /* Pop matrix stack */
	glPushMatrix();
	// glScalef(2.0f, 2.0f, 2.0f);
	glBegin(GL_POINTS);
	int vNum;
	for(vNum = 0; vNum < numVertices/3; vNum++) {
		for(int i = 0; i < 3; i++)
			glVertex3f(triangle[vNum].vertex[i].x,
			triangle[vNum].vertex[i].y, triangle[vNum].vertex[i].z);
	}
	glEnd();
	glPopMatrix();

	/* Actually draw everything to the screen */
	SDL_GL_SwapBuffers();
}

void GameEngine::set_videoflags(int videoflags)
{
	GameEngine::GetEngine()->videoflags_ = videoflags;
}

void GameEngine::set_surface(SDL_Surface *surface)
{
	GameEngine::GetEngine()->surface_ = surface;
}

int GameEngine::SetupSDL(int sdl_hardware, int screen_width, int screen_height, int screen_bpp, char *title, char *icon)
{
	int videoflags;
	int surface_type;
	int accel_type;
	if (sdl_hardware) {
		surface_type = SDL_HWSURFACE;
		accel_type = SDL_HWACCEL;
	} else {
		surface_type = SDL_SWSURFACE;
	}
	/* Initialize SDL */
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Video initialization failed: %s\n", SDL_GetError());
		return -1;
	}

	/* Get video info */
	kvideoinfo_ = SDL_GetVideoInfo();

	if (!GameEngine::GetEngine()->get_kvideoinfo()) {
		fprintf(stderr, "Could not get video info: %s\n", SDL_GetError());
		return -1;
	}

	/* Set up appropriate optoins for video */
	videoflags  = SDL_OPENGL;          /* Enable OpenGL in SDL */
	videoflags |= SDL_GL_DOUBLEBUFFER; /* Enable double buffering */
	videoflags |= SDL_HWPALETTE;       /* Store the palette in hardware */
	videoflags |= SDL_RESIZABLE;       /* Enable window resizing */

	/* Store the surface in memory using hardware */
	videoflags |= surface_type;

	/* Use hardware acceleration for drawing. 
	* Should check if it's supported first and switch to software if not */
	videoflags |= accel_type;

	GameEngine::set_videoflags(videoflags);
	/* Double buffering for smoother rendering */
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	/* Set window title and icon name */
	SDL_WM_SetCaption(title,icon);
	/* Control window position */
	putenv(strdup("SDL_VIDEO_CENTERED=1")); 

	/* Get new SDL surface initialized */
	surface_ = SDL_SetVideoMode(screen_width, screen_height, screen_bpp, videoflags);
	/* Check if surface was created successfully */
	if (!surface_) {
		fprintf(stderr, "Video mode set failed: %s\n", SDL_GetError());
		return -1;
	}
	return 0;

}

/* Mainly handle window events */
void GameEngine::HandleEvent(SDL_Event event)
{
	switch(event.type) {
	case SDL_ACTIVEEVENT:
		/* this is in case we lose focus on the screen, 
		* and want to save the work only for when we are 

		* actually using the program. */

		/*if ( event.active.gain == 0 )
		isActive = 0;
		else

		isActive = 1;*/
		break;
	case SDL_VIDEORESIZE:
		surface_ = SDL_SetVideoMode(event.resize.w, event.resize.h,32,
				     GameEngine::GetEngine()->get_videoflags());
		if (!surface_) {
			fprintf(stderr, "Could not get a new surface after last resize: %s\n", SDL_GetError());
			GameEngine::GetEngine()->QuitProgram();
		}
		ResizeWindow(event.resize.w, event.resize.h);
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
	int done = 0;
	int isActive = 1;

	/* Only run the program if we successfully create the game engine */
	if (!InitializeGame()) {
		if (GameEngine::GetEngine()->InitializeGL()) {
			fprintf(stderr, "Could not initialize OpenGL");
			GameEngine::GetEngine()->QuitProgram();
		}

		if (GameEngine::GetEngine()->SetupSDL(1, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, "tits","sdl.ico")) {
			fprintf(stderr, "Could not setup our SDL environment: %s\n", SDL_GetError());
			GameEngine::GetEngine()->QuitProgram();
		}

		/* Enable or disable for real FPS view */
		SDL_ShowCursor(SDL_DISABLE);

		/* Resize our initial window */
		GameEngine::GetEngine()->ResizeWindow(SCREEN_WIDTH, SCREEN_HEIGHT);

		/* Start event loop to handle program now */
		while (!done) {
			/* Handle whatever events may be in the queue */
			while (SDL_PollEvent(&event)) {
				GameEngine::GetEngine()->HandleEvent(event);
			}

			SDL_WarpMouse(HALF_WIDTH, HALF_HEIGHT);

			/* this makes sure the camera will still have speed when it reaches the minimum  */
			if(GameEngine::GetEngine()->get_camera()->GetMoveSpeed() <= 0.05f) GameEngine::GetEngine()->get_camera()->SetMoveSpeed(0.05f);
				GameEngine::GetEngine()->HandleKeystate(); 
			/* Draw scene */
			if (isActive) {
				GameEngine::GetEngine()->Render(); // draw our scene
			}
		}
	}
	/* Properly release everything and exit */
	GameEngine::GetEngine()->QuitProgram();
	EndGame();

	return 0;
}

/*
 * EngineGL
 * Copyright (C) 2013 Tudor Marcu. All rights reserved.
 */

#include <iostream>

#include "game_engine.h"

#define DEBUG 1  /* Used for enabling verbose output to test program */

GameEngine *GameEngine::game_engine = NULL;
ModelLoader model;

/* Constructors and destructors */
GameEngine::GameEngine(int screen_width, int screen_height, int screen_bpp)
{
	game_engine = this;
	screen_width_ = screen_width;
	screen_height_ = screen_height;
	screen_bpp_ = screen_bpp;
	isactive = false;
	isrunning = false;
}

GameEngine::~GameEngine(void)
{
	SDL_Quit();
}

/* Load our model and set up the main GL parameters */
bool GameEngine::InitializeGL(void)
{
	/* What we use to initialize the camera */
	Vector3D camera_position_ = {0.0f, 0.0f, 1.0f};
	Vector3D camera_center_ = {0.0f, 0.0f, 1.0f};
	Vector3D camera_up_ = {0.0f, 1.0f, 0.0f};

	camera_ = new Camera(camera_position_, camera_center_, camera_up_, 0.01f, 1.0f);

	/* Enable smooth shading in our program */
	glShadeModel(GL_SMOOTH);

	/* Set the background black */
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	/* Set up depth buffer */
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

	/* Which depth test we should use */
	glDepthFunc(GL_LEQUAL);

	/* Setup light attributes */
	GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 0.4f };
	GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, .8f };
	GLfloat specularLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat position[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	/* Setup light attributes */
	GLfloat ambientLight1[] = { 0.2f, 0.2f, 0.2f, 0.3f };
	GLfloat diffuseLight1[] = { 0.8f, 0.8f, 0.8, 0.7f };
	GLfloat specularLight1[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat position1[] = { 0.0f, 1.0f, -1.0f, 1.0f };

	/* Apply attributes to lights */
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight1);
	glLightfv(GL_LIGHT1, GL_POSITION, position1);

	/* Enable all the lights we want to use */
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	/* Set up our VAO */
	glGenVertexArrays(1, &model.vao); 
	glBindVertexArray(model.vao);

	/* Set up our VBOs and load the model(s) */
	glGenBuffers(1, &model.vbvert);
	glBindBuffer(GL_ARRAY_BUFFER, model.vbvert);  
	glBufferData(GL_ARRAY_BUFFER, model.vertices.size() * sizeof(struct Vector3D), &model.vertices[0], GL_STATIC_DRAW);
 
	glGenBuffers(1, &model.vbnorm);
	glBindBuffer(GL_ARRAY_BUFFER, model.vbnorm);
	glBufferData(GL_ARRAY_BUFFER, model.normals.size() * sizeof(struct Vector3D), &model.normals[0], GL_STATIC_DRAW);

	/* Set up the pointer for each VBO */
	glBindBuffer(GL_ARRAY_BUFFER, model.vbvert);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, model.vbnorm);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	/* Disable the VAO and VBO */
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
  	
	/* Gives us pretty calculations for perspective */
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	return true;
}

/* Release all memory and handles to program before destroying it */
void GameEngine::QuitProgram(void)
{
	if (camera_ != NULL)	
		delete(camera_);
	SDL_GL_DeleteContext(glcontext_);
	SDL_DestroyWindow(window_);
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
	gluPerspective(45.0f, (float)width/height, 0.1, 10000.0);
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

	if(keystate_[SDL_SCANCODE_W])
		camera_->MoveCamera(UP);
	if(keystate_[SDL_SCANCODE_S])
		camera_->MoveCamera(DOWN);
	if(keystate_[SDL_SCANCODE_A])
		camera_->MoveCamera(LEFT);
	if(keystate_[SDL_SCANCODE_D])
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

	/* Bind our VAO before we can use it */
	glBindVertexArray(model.vao);

	glDrawArrays(GL_TRIANGLES, 0, model.vertices.size());

	glBindVertexArray(0);

	/* Actually draw everything to the screen */
	SDL_GL_SwapWindow(window);
}

bool GameEngine::SetupSDL(const int screen_width, const int screen_height)
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
//	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
//      SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

	/* Initialize SDL */
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cerr << "Video initialization failed: %s\n" << SDL_GetError() << std::endl;
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

	glcontext_ = SDL_GL_CreateContext(window_);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetSwapInterval(1); // 0 = vsync off

	/* This is not needed for me... but lower opengl versions may need it */
//	glewExperimental = GL_TRUE;
	GLuint glewerr = glewInit();
	if ( glewerr != GLEW_OK )
	{
		std::cerr << "Failed to initialize GLEW." << std::endl;
		exit(EXIT_FAILURE);
	}

	return true;
}

/* Mainly handle window events */
void GameEngine::HandleEvent(SDL_Event event)
{
	switch(event.type) {
	case SDL_WINDOWEVENT_SIZE_CHANGED:
		std::cerr << "WINDOW RESIZE" << std::endl;
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
		camera_->SetPrevPosition(event.motion.xrel, event.motion.yrel);
		camera_->MouseMoved(event.motion.x, event.motion.y, SCREEN_HALF_WIDTH, SCREEN_HALF_HEIGHT);
		break;
	case SDL_QUIT:
		GameEngine::GetEngine()->SetActive(false);
		GameEngine::GetEngine()->SetRunning(false);
		break;
	default:
		break;
	}
}

int main(int argc, char *argv[])
{
	SDL_Event event;
	SDL_Window *window = NULL;

	if (argv[1] == NULL) {
		std::cerr << "ERROR: Please specify a model!" << std::endl;
		exit(EXIT_FAILURE);
	}

	model.loadmodel(argv[1]);	

	/* Only run the program if we successfully create the game engine */
	if (InitializeGame() != true) {
		std::cerr << "Could not initialize GameEngine!" << std::endl;
		exit(EXIT_FAILURE);
	}

	if (GameEngine::GetEngine()->SetupSDL(1024, 768) != true) {
		std::cerr << "SDL initialization failed: %s\n" << SDL_GetError() << std::endl;;
		exit(EXIT_FAILURE);
	}
	if (GameEngine::GetEngine()->InitializeGL() == false) {
		std::cerr << "Could not initialize OpenGL" << std::endl;
		GameEngine::GetEngine()->QuitProgram();
	}

	GameEngine::GetEngine()->SetRunning(true);
	GameEngine::GetEngine()->SetActive(true);

	/* InitializeGL does not preserve this */
	glEnable(GL_DEPTH_TEST);

	window = GameEngine::GetEngine()->GetWindow();

	/* Enable or disable for real FPS view */
	SDL_ShowCursor(SDL_DISABLE);

	/* Resize our initial window */
	GameEngine::GetEngine()->ResizeWindow(SCREEN_WIDTH, SCREEN_HEIGHT);

	/* Start event loop to handle program now */
	while (GameEngine::GetEngine()->IsRunning()) {
		/* Handle whatever events may be in the queue */
		while (SDL_PollEvent(&event)) {
			GameEngine::GetEngine()->HandleEvent(event);
		}

		/* this makes sure the camera will still have speed when it reaches the minimum  */
		if(GameEngine::GetEngine()->GetCamera()->GetMoveSpeed() <= 0.05f)
			GameEngine::GetEngine()->GetCamera()->SetMoveSpeed(0.05f);

		GameEngine::GetEngine()->HandleKeystate();

		/* Draw scene */
		if (GameEngine::GetEngine()->IsActive()) {
			SDL_WarpMouseInWindow(window, SCREEN_HALF_WIDTH, SCREEN_HALF_HEIGHT);
			GameEngine::GetEngine()->Render(window);
		}
	}
	/* Properly release everything and exit */
	GameEngine::GetEngine()->QuitProgram();
	EndGame();
	return 0;
}

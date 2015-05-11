/*
 * EngineGL
 * Copyright (C) 2015 Tudor Marcu. All rights reserved.
 */

#include <iostream>
#include <fstream>

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

#include "game_engine.h"

#define DEBUG 1  /* Used for enabling verbose output to test program */

#define BUFFER_OFFSET(offset) ((void *) (offset))

GameEngine *GameEngine::game_engine = NULL;
glm::mat4 modelmatrix;
glm::mat4 view;
glm::mat4 modelview;
glm::mat4 projection;
glm::mat4 normalmatrix;

unsigned int modelmatrixid;
unsigned int viewmatrixid;
unsigned int projectionmatrixid;
unsigned int normalmatrixid;
unsigned int shaderprogram;

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

static std::string ReadShader(const char *filename)
{
	std::ifstream input(filename);

	if (!input.good()) {
		printf("Failed to open file %s\n", filename);
		exit(EXIT_FAILURE);
	}

	return std::string(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());
}

static void InitializeShaders(void)
{
	const char *shader[1];
	unsigned int vertexID;
	unsigned int fragmentID;
	unsigned int program;
	int ret;
	std::string shaderfile;

	vertexID = glCreateShader(GL_VERTEX_SHADER);
	fragmentID = glCreateShader(GL_FRAGMENT_SHADER);

	shaderfile = ReadShader("shaders/vertexshader.vs");
	shader[0] = shaderfile.c_str();
	glShaderSource(vertexID, 1, shader, 0);

	shaderfile = ReadShader("shaders/fragmentshader.fs");
	shader[0] = shaderfile.c_str();
	glShaderSource(fragmentID, 1, shader, 0);

	glCompileShader(vertexID);
	glGetShaderiv(vertexID, GL_COMPILE_STATUS, &ret);
	if (ret != GL_TRUE) {
		printf("Failed to compile vertex shader!\n");
		GLchar InfoLog[1024];
		glGetShaderInfoLog(vertexID, sizeof(InfoLog), NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", GL_VERTEX_SHADER, InfoLog);
	}

	glCompileShader(fragmentID);
	glGetShaderiv(vertexID, GL_COMPILE_STATUS, &ret);
	if (ret != GL_TRUE) {
		printf("Failed to compile fragment shader!\n");
		GLchar InfoLog[1024];
		glGetShaderInfoLog(fragmentID, sizeof(InfoLog), NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", GL_FRAGMENT_SHADER, InfoLog);
	}

	program = glCreateProgram();
	GameEngine::GetEngine()->SetShader(program);
	shaderprogram = GameEngine::GetEngine()->GetShader();

	glAttachShader(program, vertexID);
	glAttachShader(program, fragmentID);
	glLinkProgram(program);
	glUseProgram(program);
}

void GameEngine::LoadModelVAO(Model *model)
{
	unsigned int buffer;
	unsigned int vPosition;
	unsigned int vNormal;
	int totalvertices;

	/* Set up our VAO */
	glGenVertexArrays(1, &model->vao);
	glBindVertexArray(model->vao);

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	totalvertices = model->vertices.size() + model->normals.size();
	glBufferData(GL_ARRAY_BUFFER, totalvertices*  sizeof(struct Vector3D), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, model->vertices.size() * sizeof(struct Vector3D), &model->vertices[0]);
	glBufferSubData(GL_ARRAY_BUFFER, model->vertices.size() * sizeof(struct Vector3D), model->normals.size() * sizeof(struct Vector3D), &model->normals[0]);

	vPosition = glGetAttribLocation(GameEngine::GetEngine()->GetShader(), "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	vNormal = glGetAttribLocation(GameEngine::GetEngine()->GetShader(), "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(model->vertices.size() * sizeof(struct Vector3D)));
}

/* Load our model and set up the main GL parameters */
bool GameEngine::InitializeGL(void)
{
	/* What we use to initialize the camera */
	Vector3D camera_position_ = {0.0f, 0.0f, 1.0f};
	Vector3D camera_center_ = {0.0f, 0.0f, -1.0f};
	Vector3D camera_up_ = {0.0f, 1.0f, 0.0f};
	unsigned int shaderprogram = GameEngine::GetEngine()->GetShader();
	unsigned int modelmatrixid = glGetUniformLocation(shaderprogram, "modelmatrix");
	unsigned int viewmatrixid = glGetUniformLocation(shaderprogram, "viewmatrix");
	unsigned int projectionmatrixid = glGetUniformLocation(shaderprogram, "projectmatrix");
	camera_ = new Camera(camera_position_, camera_center_, camera_up_, 0.01f, 1.0f);

	modelmatrix = glm::mat4(1.0f);
	view = glm::lookAt(
					    glm::vec3(camera_position_.x, camera_position_.y, camera_position_.z),
					    glm::vec3(camera_center_.x, camera_center_.y, camera_center_.z),
					    glm::vec3(camera_up_.x, camera_up_.y, camera_up_.z));
	projection = glm::perspective(45.0f, (float) SCREEN_WIDTH/ (float) SCREEN_HEIGHT, 0.1f, 10000.0f);

	glUniformMatrix4fv(modelmatrixid, 1, GL_FALSE, &modelmatrix[0][0]);
	glUniformMatrix4fv(viewmatrixid, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projectionmatrixid, 1, GL_FALSE, &projection[0][0]);

	/* Enable smooth shading in our program */
	glShadeModel(GL_SMOOTH);

	/* Set the background color */
	glClearColor(0.0f, 0.0f, 0.3f, 0.0f);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	/* Set up depth buffer */
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

	/* Which depth test we should use */
	glDepthFunc(GL_LEQUAL);

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
	/*gluLookAt(camera_->GetCameraPosition().x,
		  camera_->GetCameraPosition().y,
		  camera_->GetCameraPosition().z,
		  camera_->GetCameraPosition().x + camera_->GetCameraCenter().x, 
		  camera_->GetCameraPosition().y + camera_->GetCameraCenter().y, 
		  camera_->GetCameraPosition().z + camera_->GetCameraCenter().z,
		  0, 1, 0);
	*/
	view = glm::lookAt(
				glm::vec3(camera_->GetCameraPosition().x, camera_->GetCameraPosition().y, camera_->GetCameraPosition().z),
				glm::vec3(camera_->GetCameraPosition().x + camera_->GetCameraCenter().x, camera_->GetCameraPosition().y + camera_->GetCameraCenter().y, camera_->GetCameraPosition().z + camera_->GetCameraCenter().z),
				glm::vec3(0.0, 1.0, 0.0));
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
void GameEngine::Render(SDL_Window *window, Model *model)
{
	/* Clear The Screen And The Depth Buffer*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GameEngine::GetEngine()->CameraLook(); /* Where gluLookAt(...) would normally appear */
	modelmatrix = glm::mat4(1.0f);
	modelview = view * modelmatrix;
	normalmatrix = glm::transpose(glm::inverse(modelview));

	modelmatrixid = glGetUniformLocation(shaderprogram, "modelviewmatrix");
	viewmatrixid = glGetUniformLocation(shaderprogram, "viewmatrix");
	projectionmatrixid = glGetUniformLocation(shaderprogram, "projectionmatrix");
	normalmatrixid = glGetUniformLocation(shaderprogram, "normalmatrix");

	glUniformMatrix4fv(modelmatrixid, 1, GL_FALSE, &modelview[0][0]);
	glUniformMatrix4fv(viewmatrixid, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projectionmatrixid, 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(normalmatrixid, 1, GL_FALSE, &normalmatrix[0][0]);

	/* Draw whatever is specified for the program */
	RenderGame();

	/* Actually draw everything to the screen */
	SDL_GL_SwapWindow(window);
}

bool GameEngine::SetupSDL(const int screen_width, const int screen_height)
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	//SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

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

	glewExperimental = GL_TRUE;
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
		/*
		std::cerr << "WINDOW RESIZE" << std::endl;
		ResizeWindow(event.window.data1, event.window.data2);
		*/
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
	Model *model = new Model();
	model->loadmodel(argv[1]);

	/* Only run the program if we successfully create the game engine */
	if (InitializeGame() != true) {
		std::cerr << "Could not initialize GameEngine!" << std::endl;
		exit(EXIT_FAILURE);
	}

	if (GameEngine::GetEngine()->SetupSDL(1024, 768) != true) {
		std::cerr << "SDL initialization failed: %s\n" << SDL_GetError() << std::endl;;
		exit(EXIT_FAILURE);
	}

	GameEngine::GetEngine()->SetModel(model);

	InitializeShaders();

	if (GameEngine::GetEngine()->InitializeGL() == false) {
		std::cerr << "Could not initialize OpenGL" << std::endl;
		GameEngine::GetEngine()->QuitProgram();
	}

	GameEngine::GetEngine()->LoadModelVAO(GameEngine::GetEngine()->GetModel());


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
			GameEngine::GetEngine()->Render(window, GameEngine::GetEngine()->GetModel());
		}
	}
	/* Properly release everything and exit */
	GameEngine::GetEngine()->QuitProgram();
	EndGame();
	return 0;
}
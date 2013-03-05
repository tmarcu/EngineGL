/*
 * EngineGL
 * Copyright (C) 2013 Tudor Marcu. All rights reserved.
 */

#include "game.h"

/* Create a GameEngine object that will run the program */
int InitializeGame()
{
	g_game = new GameEngine(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP);
	if (g_game == 0)
		return -1;

	return 0;
}

int StartGame()
{
	return 0;
}

void MouseButtonUp(int x, int y, int button) {};
void MouseButtonDown(int x, int y, int button) {};

void RenderGame()
{
}

/* Read in the model data from an .obj file and store it */
void ReadData(char *file, int numPoints, Polygon *triangle) {
	FILE *mFile;
	mFile = fopen(file, "r");
	char d;
	int vNum;

	/* Iterate through the file
	 Scan for 3 points to make 1 Polygon */
	for(vNum = 0; vNum < numPoints/3; vNum++) {
		// Read 3 points before incrementing Polygons
		for(int i = 0; i < 3; i++) {
			fscanf(mFile, "%c\n%f\n%f\n%f\n", &d, &triangle[vNum].vertex[i].x, &triangle[vNum].vertex[i].y, &triangle[vNum].vertex[i].z);
		}/*
		for(int i = 0; i < 3; i++) {
			printf("%f\n%f\n%f\n",  triangle[vNum].vertex[i].x, triangle[vNum].vertex[i].y, triangle[vNum].vertex[i].z);
		}*/
	}

	fclose(mFile);
}

/* function to handle key press events */
void HandleKeyPress(SDL_keysym *keysym)
{
	switch (keysym->sym) {
	case SDLK_ESCAPE:
		g_game->QuitProgram();
		break;
	case SDLK_F1:
		//SDL_WM_ToggleFullScreen(g_game->get_surface()); /* ONLY works on X11 */
		//g_game->set_surface(SDL_SetVideoMode(1280, 800, SCREEN_BPP, SDL_FULLSCREEN));
		break;
	case SDLK_RIGHTBRACKET: /* Increase camera speed */
		g_game->get_camera()->SetLookSpeed(0.05f);
		break;
	case SDLK_LEFTBRACKET: /* Decrease camera speed only if it is greater than the minimum */
		if (g_game->get_camera()->GetLookSpeed() >= 0.05f)
			g_game->get_camera()->SetLookSpeed(g_game->get_camera()->GetLookSpeed()- 0.01f);
		break;
	case SDLK_F2:
		g_game->get_camera()->SetCameraPosition(0.0f, 0.0f, 1.0f);
		g_game->get_camera()->SetCameraCenter(0.0f, 0.0f, -1.0f);
		break;
	default:
		break;
	}

	return;
}

void EndGame()
{
	delete g_game;
}


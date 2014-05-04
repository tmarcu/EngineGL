/*
 * EngineGL
 * Copyright (C) 2013 Tudor Marcu. All rights reserved.
 */

#include "game.h"

/* Create a GameEngine object that will run the program */
bool InitializeGame()
{
	g_game = new GameEngine(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP);
	if (g_game == 0)
		return false;

	return true;
}

bool StartGame()
{
	return true;
}

void MouseButtonUp(int x, int y, int button) {};
void MouseButtonDown(int x, int y, int button) {};

void RenderGame()
{
}

/* function to handle key press events */
void HandleKeyPress(SDL_Keysym *keysym)
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
		g_game->GetCamera()->SetLookSpeed(0.05f);
		break;
	case SDLK_LEFTBRACKET: /* Decrease camera speed only if it is greater than the minimum */
		if (g_game->GetCamera()->GetLookSpeed() >= 0.05f)
			g_game->GetCamera()->SetLookSpeed(g_game->GetCamera()->GetLookSpeed()- 0.01f);
		break;
	case SDLK_F2:
		g_game->GetCamera()->SetCameraPosition(0.0f, 0.0f, 1.0f);
		g_game->GetCamera()->SetCameraCenter(0.0f, 0.0f, -1.0f);
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
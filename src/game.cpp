/*
 * EngineGL
 * Copyright (C) 2015 Tudor Marcu. All rights reserved.
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
	Model *model = g_game->GetModel();

	/* Bind our VAO before we can use it */
	glBindVertexArray(model->vao);

	glDrawArrays(GL_TRIANGLES, 0, model->vertices.size());

	glBindVertexArray(0);

}

/* function to handle key press events */
void HandleKeyPress(SDL_Keysym *keysym)
{
	unsigned int loc;
	unsigned int flags;

	switch (keysym->sym) {
	case SDLK_ESCAPE:
		g_game->QuitProgram();
		break;
	case SDLK_F1:
		flags = SDL_GetWindowFlags(g_game->GetWindow()) ^ SDL_WINDOW_FULLSCREEN_DESKTOP;
		SDL_SetWindowFullscreen(g_game->GetWindow(), flags);
		break;
	case SDLK_RIGHTBRACKET: /* Increase camera speed */
		g_game->GetCamera()->SetMoveSpeed(g_game->GetCamera()->GetMoveSpeed() + 0.01f);
		break;
	case SDLK_LEFTBRACKET: /* Decrease camera speed only if it is greater than the minimum */
		if (g_game->GetCamera()->GetMoveSpeed() >= 0.01f)
			g_game->GetCamera()->SetMoveSpeed(g_game->GetCamera()->GetMoveSpeed()- 0.01f);
		break;
	case SDLK_F2:
		g_game->GetCamera()->SetCameraPosition(0.0f, 0.0f, 1.0f);
		g_game->GetCamera()->SetCameraCenter(0.0f, 0.0f, -1.0f);
		break;
	case SDLK_f:
		if (g_game->GetModel()->isfilled == true) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			g_game->GetModel()->isfilled = false;
		} else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			g_game->GetModel()->isfilled = true;
		}
		break;
	case SDLK_1:
		loc = glGetUniformLocation(GameEngine::GetEngine()->GetShader(), "xscale");
		glProgramUniform4f(GameEngine::GetEngine()->GetShader(), loc, 3.0, 1.0, 1.0, 1.0);
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

#include "Game.h"

Game::Game( Window& window )
	:
	wnd( window ),
	gfx(wnd.GFX())
{
}

Game::~Game()
{
}

void Game::Go()
{
	// Timing stuff
	dt = ft.Mark();

	ProcessFrame();
}


void Game::ProcessFrame()
{
	// Code goes here
	gfx.BeginFrame();
	gfx.EndFrame();
}

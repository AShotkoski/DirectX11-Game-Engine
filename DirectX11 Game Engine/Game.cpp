#include "Game.h"

Game::Game( Window& window )
	:
	wnd( window ),
	gfx(wnd.GFX()),
	test(gfx)
{
}

Game::~Game()
{
}

void Game::Go()
{
	// Capture frame time
	dt = ft.Mark();

	ProcessFrame();
}


void Game::ProcessFrame()
{
	// Code goes here
	gfx.BeginFrame();

	test.Draw( gfx );
	test.Update( dt );

	gfx.EndFrame();
}

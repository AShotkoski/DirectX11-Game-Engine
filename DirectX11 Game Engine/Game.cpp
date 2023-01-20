#include "Game.h"

Game::Game( Window& window )
	:
	wnd( window )
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
	OutputDebugStringA( (" dt: " + std::to_string(dt) + '\n' ).c_str());
	OutputDebugStringA( ( " t: " + std::to_string( ft.GetTime() ) + '\n').c_str());
}

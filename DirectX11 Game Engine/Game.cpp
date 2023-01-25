#include "Game.h"

Game::Game( Window& window )
	:
	wnd( window ),
	gfx(wnd.GFX()),
	test(gfx)
{
	// TODO MAKE USE ACXTUAL STUFF NOT BAD STUFF
	gfx.SetProjection( DirectX::XMMatrixPerspectiveLH( 1.0f, 3.0f / 4.0f, 0.5f, 40.0f ) );
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

#include "Game.h"

Game::Game( Window& window)
	:
	wnd( window ),
	gfx(wnd.GFX()),
	cube(gfx,1.f,1.f,1.f,1.f)
{
	gfx.SetProjection( DirectX::XMMatrixPerspectiveLH( 1.0f, 1.f / wnd.GetAspectRatio(), 0.5f, 40.0f));
}

Game::~Game()
{
}

void Game::Go()
{
	// Capture frame time
	dt = ft.Mark();

	gfx.BeginFrame();
	ProcessFrame();
	gfx.EndFrame();
}


void Game::ProcessFrame()
{	
	cube.Update( dt );
	cube.Draw( gfx );
}

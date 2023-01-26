#include "Game.h"
#include <random>

Game::Game( Window& window)
	:
	wnd( window ),
	gfx(wnd.GFX())
{
	gfx.SetProjection( DirectX::XMMatrixPerspectiveLH( 1.0f, 1.f / wnd.GetAspectRatio(), 0.5f, 40.0f));
	std::mt19937 rng( std::random_device{}( ) );
	std::uniform_real_distribution<float> norm( -1.f, 1.f );
	std::uniform_real_distribution<float> size( 0.3f, 2.f );
	for ( int i = 0; i < 5; i++ )
	{
		cubes.emplace_back( std::make_unique<Cube>(
			gfx,
			size( rng ),
			norm( rng ),
			norm( rng ),
			norm( rng ),
			norm( rng ),
			norm( rng ),
			norm( rng ),
			norm( rng ),
			norm( rng ),
			norm( rng ) ) );
	}
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
	for ( auto& c : cubes )
	{
		c->Update( dt );
		c->Draw( gfx );
	}
}

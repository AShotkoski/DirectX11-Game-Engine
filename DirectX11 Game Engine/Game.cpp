#include "Game.h"
#include <random>

Game::Game( Window& window)
	:
	wnd( window ),
	gfx(wnd.GFX()),
	pl(gfx)
{
	gfx.SetProjection( DirectX::XMMatrixPerspectiveLH( 1.0f, 1.f / wnd.GetAspectRatio(), 0.5f, 60.0f));
	std::mt19937 rng( std::random_device{}( ) );
	std::normal_distribution<float> size( 0.6f, 0.6f );
	std::normal_distribution<float> rhodist( 5.f, 2.f );
	std::uniform_real_distribution<float> thetadist( -2.f, 2.f );
	std::uniform_real_distribution<float> phidist( -2.f, 2.f );
	std::uniform_real_distribution<float> dtheta( -2.f, 2.f );
	std::uniform_real_distribution<float> dphi( -2.f, 2.f );
	std::uniform_real_distribution<float> rot( -2.f, 2.f );
	for ( int i = 0; i < 3; i++ )
	{
		cubes.emplace_back( std::make_unique<Cube>(
			gfx,
			size(rng),
			rhodist( rng ),
			thetadist( rng ),
			phidist( rng ),
			dphi(rng),
			dtheta(rng),
			rot( rng ),
			rot( rng ),
			rot( rng ) ) );
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
	pl.Update( dt );
	pl.Draw( gfx );
}

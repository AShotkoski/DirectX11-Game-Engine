#include "Game.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include "NumberFactory.h"

Game::Game()
	:
	wnd( ScreenWidth, ScreenHeight, WindowTitle),
	gfx(wnd.GFX()),
	pl(gfx)
{
	//Set matrices
	gfx.SetProjection( DirectX::XMMatrixPerspectiveLH( 1.0f, 1.f / wnd.GetAspectRatio(), 0.5f, 60.0f));

	for ( int i = 0; i < 10; i++ )
	{
		float rho = NumberFactory::NormalReal( 5.0f, 1.0f, 0.1f, 20.f );
		float theta = NumberFactory::RandomReal( -2.0f, 2.0f );
		float phi = NumberFactory::RandomReal( -2.0f, 2.0f );
		float dtheta = NumberFactory::RandomReal( -2.0f, 2.0f );
		float dphi = NumberFactory::RandomReal( -2.0f, 2.0f );
		float dRot = NumberFactory::RandomReal( -2.0f, 2.0f );
		float size = NumberFactory::NormalReal( 0.6f, 0.8f, 0.2f, 2.0f );

		cubes.emplace_back(
			std::make_unique<Cube>( gfx, size, rho, theta, phi, dtheta, dphi, dRot, dRot, dRot ) );

	}
}

Game::~Game()
{
}

void Game::Go()
{
	// Capture frame time
	dt = ft.Mark() * timeFactor;

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

	// Control game-wide settings
	if ( ImGui::Begin( "Simulation Control" ) )
	{
		ImGui::SliderFloat( "Speed", &timeFactor, 0.0f, 4.0f );
	}
	ImGui::End();

	gfx.GetCamera().SpawnControlWindow();
}


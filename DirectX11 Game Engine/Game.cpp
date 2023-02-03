#include "Game.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include "NumberFactory.h"
#include "Colors.h"

Game::Game()
	:
	wnd( ScreenWidth, ScreenHeight, WindowTitle),
	gfx(wnd.GFX()),
	light(gfx, 0.25f)
{
	//Set matrices
	gfx.SetProjection( DirectX::XMMatrixPerspectiveLH( 1.0f, 1.f / wnd.GetAspectRatio(), 0.5f, 60.0f));

	for ( int i = 0; i < 64; i++ )
	{
		float size = NumberFactory::NormalReal( 1.0f, 1.f, 0.5f, 1.5f );
		float rho = NumberFactory::RandomReal( 5.0f, 25.0f );
		float theta = NumberFactory::RandomReal( -2.0f, 2.0f );
		float phi = NumberFactory::RandomReal( -2.0f, 2.0f );
		float dtheta = NumberFactory::RandomReal( -1.2f, 1.2f );
		float dphi = NumberFactory::RandomReal( -1.2f, 1.2f );
		float dRot = NumberFactory::RandomReal( -1.2f, 1.2f );
		DWORD col = NumberFactory::RandomInt<DWORD>( 0x00, 0xFFFFFFFF );


		cubes.emplace_back(
			std::make_unique<Cube>( gfx, size, rho, theta, phi, dtheta, dphi, dRot, dRot, dRot,
									DirectX::XMFLOAT3(&Color::MakeRgb( col ).el[0])));

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
	UpdateLogic();

	gfx.EndFrame();
}


void Game::UpdateLogic()
{	
	for ( auto& c : cubes )
	{
		c->Update( dt );
	}

	light.Bind(gfx);

	// Control game-wide settings
	if ( ImGui::Begin( "Simulation Control" ) )
	{
		ImGui::SliderFloat( "Speed", &timeFactor, 0.0f, 4.0f );
	}
	ImGui::End();

	gfx.GetCamera().SpawnControlWindow();
	light.SpawnControlWindow();
}

void Game::DrawFrame()
{
	for ( auto& c : cubes )
	{
		c->Draw( gfx );
	}
	light.Draw( gfx );
}
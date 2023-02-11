#include "Game.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include "NumberFactory.h"
#include "Colors.h"
#include "MathUtil.h"
namespace dx = DirectX;

Game::Game()
	:
	wnd( ScreenWidth, ScreenHeight, WindowTitle ),
	gfx( wnd.GFX() ),
	light( gfx, 0.25f ),
	testModel(gfx, "Models\\walls.dae")
{
	//Set matrices
	gfx.SetProjection( DirectX::XMMatrixPerspectiveLH( 1.0f, 1.f / wnd.GetAspectRatio(), 
													   NearClipping, FarClipping));

	for ( int i = 0; i < 96; i++ )
	{
		float size = NumberFactory::NormalReal( 1.0f, 1.f, 0.5f, 1.5f );
		float rho = NumberFactory::RandomReal( 3.0f, 25.0f );
		float theta = NumberFactory::RandomReal( -2.0f, 2.0f );
		float phi = NumberFactory::RandomReal( -2.0f, 2.0f );
		float dtheta = NumberFactory::RandomReal( -1.2f, 1.2f );
		float dphi = NumberFactory::RandomReal( -1.2f, 1.2f );
		float dRot = NumberFactory::RandomReal( -1.2f, 1.2f );
		float specPow = NumberFactory::RandomReal( 1.f, 120.2f );
		float specInt = NumberFactory::RandomReal( 0.0f, 2.5f );
		DWORD col = NumberFactory::RandomInt<DWORD>( 0x00, 0xFFFFFFFF );

		cubes.emplace_back(
			std::make_unique<Cube>( gfx, size, rho, theta, phi, dtheta, dphi, dRot, dRot, dRot,
									DirectX::XMFLOAT3(&Color::MakeRgb( col ).el[0]), specInt, specPow));
	}

	testModel.UpdateTransform( dx::XMMatrixTranslation( 0, -0.5f, 0 ) );
}

Game::~Game()
{
}

void Game::Go()
{
	// Capture frame time
	dt = ft.Mark();
	adj_dt = dt * timeFactor;

	gfx.BeginFrame();
	UpdateLogic();
	DrawFrame();

	if constexpr ( globals::enableImGui )
		DrawImGuis();

	gfx.EndFrame();
}

void Game::UpdateLogic()
{	
	for ( auto& c : cubes )
	{
		c->Update( adj_dt );
	}

	light.Bind(gfx);

	// Camera control
	ControlCamera();

	// Test code with kbd input
	while ( auto e = wnd.kbd.GetEvent() )
	{
		if ( e->GetType() == Keyboard::Event::Keydown )
		{
			switch ( e->GetVirtualKey() )
			{
				case 'I':
				{
					
					break;
				}
			}
		}
	}
}

void Game::DrawFrame()
{
	for ( auto& c : cubes )
	{
		c->Draw( gfx );
	}

	light.Draw( gfx );
	testModel.Draw( gfx );
}

void Game::DrawImGuis()
{
	// Control game-wide settings
	if ( ImGui::Begin( "Simulation Control" ) )
	{
		ImGui::SliderFloat( "Speed", &timeFactor, 0.0f, 4.0f );
		ImGui::Text( "%.f FPS", ImGui::GetIO().Framerate );
	}
	ImGui::End();

	gfx.GetCamera().SpawnControlWindow();
	light.SpawnControlWindow();
}

void Game::ControlCamera()
{
	DirectX::XMFLOAT3 dCampos = { 0, 0, 0 };

	// Hold space to go into camera control mode
	if ( wnd.kbd.KeyIsPressed( VK_SPACE ) )
	{
		gfx.GetCamera().EnableMouseControl();
		wnd.DisableCursor();
	}
	else
	{
		gfx.GetCamera().DisableMouseControl();
		wnd.EnableCursor();
		// Fast return and don't process keyboard input when not in mouse control mode,
		// be honest you don't need to move the camera in view mode.
		return;
	}

	if ( wnd.kbd.KeyIsPressed( 'W') )
	{
		dCampos.z += 1.f;
	}
	if ( wnd.kbd.KeyIsPressed( 'S') )
	{
		dCampos.z -= 1.f;
	}
	if ( wnd.kbd.KeyIsPressed( 'A') )
	{
		dCampos.x -= 1.f;
	}
	if ( wnd.kbd.KeyIsPressed( 'D') )
	{
		dCampos.x += 1.f;
	}
	if ( wnd.kbd.KeyIsPressed( 'R') )
	{
		dCampos.y += 1.f;
	}
	if ( wnd.kbd.KeyIsPressed( 'F') )
	{
		dCampos.y -= 1.f;
	}
	// Control camera movement speed with scrollwheel
	while ( auto e = wnd.mouse.GetEvent() )
	{
		if ( e->GetType() == Mouse::Event::ScrollUp )
		{
			gfx.GetCamera().UpdateMovementSpeed( 1.05f );
		}
		else if ( e->GetType() == Mouse::Event::ScrollDown )
		{
			gfx.GetCamera().UpdateMovementSpeed( 0.95f );
		}
	}

	if ( dCampos.x != 0 || dCampos.y != 0 || dCampos.z != 0 )
	{
		// Normalize camera movement vector so that diagonals are not twice as fast.
	    DirectX::XMStoreFloat3(&dCampos, DirectX::XMVector3Normalize( DirectX::XMLoadFloat3( &dCampos ) ));

		gfx.GetCamera().UpdatePosition(dCampos, dt);
	}
}

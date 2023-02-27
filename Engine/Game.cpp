#include "Game.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include "NumberFactory.h"
#include "Colors.h"
#include "MathUtil.h"
#include "DynamicCB.h"
namespace dx = DirectX;

Game::Game()
	: wnd( ScreenWidth, ScreenHeight, WindowTitle )
	, gfx( wnd.GFX() )
	, light( gfx, 0.15f, { 1.9f, 2.f, -2.f } )
	, cube0( gfx, { 1,1,1 }, { 6,0,0 }, 0,0,0  )
	, cube1( gfx, { 1,1,1 }, { -1,0,0 }, 0,0,0 )
	, sponza(gfx, "Models\\sponza\\sponza_sad.obj")
{
	//Set matrices
	gfx.SetProjection( DirectX::XMMatrixPerspectiveLH( 1.0f, 1.f / wnd.GetAspectRatio(), 
													   NearClipping, FarClipping));
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
	light.Bind(gfx);

	// Camera control
	ControlCamera();
}

void Game::DrawFrame()
{
	light.Draw( gfx );
	sponza.Draw( gfx );
	cube0.Draw( gfx );
	cube0.DrawOutline( gfx );
	cube1.Draw( gfx );
	cube1.DrawOutline( gfx );
}

void Game::DrawImGuis()
{
	imguilog.Draw( "log" );
	// Control game-wide settings controlled by adjusted dt
	if ( ImGui::Begin( "Simulation Control" ) )
	{
		ImGui::SliderFloat( "Speed", &timeFactor, 0.0f, 4.0f );
		ImGui::Text( "%.f FPS", ImGui::GetIO().Framerate );
	}
	ImGui::End();
	gfx.GetCamera().SpawnControlWindow();
	light.SpawnControlWindow();
	sponza.SpawnControlWindow();
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
	if ( wnd.kbd.KeyIsPressed( 'E') )
	{
		dCampos.y += 1.f;
	}
	if ( wnd.kbd.KeyIsPressed( 'Q') )
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

#pragma once
#include "Window.h"
#include "ImGuiManager.h"
#include "FrameTimer.h"
#include "Cube.h"
#include "FlappingPlane.h"

class Game
{
public:
	Game();
	~Game();
	Game( const Game& ) = delete;
	const Game& operator=( const Game& ) = delete;
	void Go();
private:
	void ProcessFrame();
private:
	ImGuiManager imguimanager;
	Window wnd;
	Graphics& gfx;
	FrameTimer ft;

	FlappingPlane pl; 
	std::vector<std::unique_ptr<Cube>> cubes;
private:
	float dt = 0.f;
	float timeFactor = 1.0f;

private:
	static constexpr UINT ScreenWidth = 800u;
	static constexpr UINT ScreenHeight = 600u;
	static constexpr const wchar_t* WindowTitle = L"DirectX Window";
};


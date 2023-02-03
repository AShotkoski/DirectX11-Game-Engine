#pragma once
#include "Window.h"
#include "ImGuiManager.h"
#include "FrameTimer.h"
#include "Cube.h"
#include "PointLight.h"

class Game
{
public:
	Game();
	~Game();
	Game( const Game& ) = delete;
	const Game& operator=( const Game& ) = delete;
	void Go();
private:
	void UpdateLogic();
	void DrawFrame();
	void ControlCamera();
private:
	ImGuiManager imguimanager;
	Window wnd;
	Graphics& gfx;
	FrameTimer ft;

	PointLight light;
	std::vector<std::unique_ptr<Cube>> cubes;
private:
	float dt = 0.f;
	float timeFactor = 1.0f;

private:
	static constexpr UINT ScreenWidth = 800u;
	static constexpr UINT ScreenHeight = 600u;
	static constexpr const wchar_t* WindowTitle = L"DirectX Window";
	static constexpr float NearClipping = 0.5f;
	static constexpr float FarClipping = 50.0f;
};


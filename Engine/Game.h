#pragma once
#include "Window.h"
#include "FrameTimer.h"
#include "PointLight.h"
#include "Model.h"
#include "ImGuiLog.h"
#include "Cube.h"
#include "FrameCommander.h"


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
	void DrawImGuis();
private:
	ImGuiLog imguilog; // Create first so it can be used in other ctors
	Window wnd;
	Graphics& gfx;
	FrameTimer ft;
	FrameCommander frame;

	PointLight light;
	Cube cube0;
	Cube cube1;
	Model sponza;

private:
	float dt = 0.f;
	float adj_dt = 0.f;
	float timeFactor = 1.0f;

private:
	static constexpr UINT ScreenWidth = 1272u;
	static constexpr UINT ScreenHeight = 954u;
	static constexpr const wchar_t* WindowTitle = L"DirectX Window";
	static constexpr float NearClipping = 0.5f;
	static constexpr float FarClipping = 85.0f;
};


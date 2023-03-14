#pragma once
#include "Core/Window.h"
#include "Util/FrameTimer.h"
#include "Graphics/PointLight.h"
#include "Graphics/Model.h"
#include "Util/ImGuiLog.h"
#include "Drawable/Cube.h"
#include "Graphics/RDG/TestGraph.h"

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
	RDG::TestGraph graph;

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
	static constexpr const wchar_t* WindowTitle = L"RyeEngine";
	static constexpr float NearClipping = 0.1f;
	static constexpr float FarClipping = 130.0f;
	static constexpr float Fov = 93.f;
};


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
	// Hold a ref to graphics for easier calling
	Graphics& gfx;
	FrameTimer ft;
	std::vector<std::unique_ptr<Cube>> cubes;
	FlappingPlane pl;
private:
	float dt = 0.f;
	float timeFactor = 1.0f;
};


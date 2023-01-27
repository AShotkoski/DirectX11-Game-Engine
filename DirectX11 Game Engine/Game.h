#pragma once
#include "Window.h"
#include "FrameTimer.h"
#include "Cube.h"
#include "FlappingPlane.h"

class Game
{
public:
	Game(Window& window);
	~Game();
	Game( const Game& ) = delete;
	const Game& operator=( const Game& ) = delete;
	void Go();
private:
	void ProcessFrame();
private:
	Window& wnd;
	// Hold a ref to graphics
	Graphics& gfx;
	FrameTimer ft;
	std::vector<std::unique_ptr<Cube>> cubes;
	FlappingPlane pl;
private:
	float dt = 0.f;
};


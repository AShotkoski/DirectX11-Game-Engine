#pragma once
#include "Window.h"
#include "FrameTimer.h"
#include "Cube.h"

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
	Cube cube;
private:
	float dt = 0.f;
};


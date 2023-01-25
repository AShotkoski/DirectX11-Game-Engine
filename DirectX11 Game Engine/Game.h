#pragma once
#include "Window.h"
#include "FrameTimer.h"
#include "TestDrawable.h"

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
	//test
	tDrawable test;
private:
	float dt = 0.f;
};


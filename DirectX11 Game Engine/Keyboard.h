#pragma once
#include "Win.h"
#include <bitset>

class Keyboard
{
	friend class Window;
public:
	Keyboard()
	{
	}
	bool KeyIsPressed( unsigned char VirtualKey ) const;
private:
	// Windows-called funcs
	void Keydown( WPARAM VK );
	void Keyup( WPARAM VK );
private:
	std::bitset<256u> keystates;
};


#include "Keyboard.h"

// Windows-called funcs

bool Keyboard::KeyIsPressed( unsigned char VirtualKey ) const
{
	return keystates[VirtualKey];
}

void Keyboard::Keydown( WPARAM VK )
{
	keystates[VK] = true;
}

void Keyboard::Keyup( WPARAM VK )
{
	keystates[VK] = false;
}

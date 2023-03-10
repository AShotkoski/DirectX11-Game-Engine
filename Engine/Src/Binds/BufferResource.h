#pragma once
#include <Graphics/Graphics.h>

class BufferResource
{
public:
	virtual ~BufferResource() = default;
	virtual void Clear( Graphics& gfx ) = 0;
	virtual void BindAsRT( Graphics& gfx ) = 0;
	virtual void BindAsRT( Graphics& gfx, BufferResource* ) = 0;
private:

};
#pragma once
#include <Graphics/Graphics.h>


class BufferResource
{
public:
	virtual ~BufferResource() = default;
	virtual void Clear( Graphics& gfx ) const = 0;
	virtual void BindAsRT( Graphics& gfx ) const = 0;
protected:
	virtual void BindAsRT( Graphics& gfx, ID3D11DepthStencilView* pDepthStencilView ) const = 0;
};
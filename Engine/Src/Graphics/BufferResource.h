#pragma once
#include <Graphics/Graphics.h>
#include <Binds/Bindable.h>

class BufferResource : public Bindable
{
public:
	virtual ~BufferResource() = default;
	virtual void Clear( Graphics& gfx ) const = 0;
protected:
	virtual void BindAsRT( Graphics& gfx, ID3D11DepthStencilView* pDepthStencilView ) const = 0;
};
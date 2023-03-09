#pragma once
#include <Graphics/GraphicsResource.h>

// todo override ctor to allow custom mipmapping

class DepthStencil : public GraphicsResource
{
	friend class Graphics;
public:
	DepthStencil( Graphics& gfx, UINT width, UINT height );
	void Clear(Graphics& gfx);
private:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthStencilView;
	
};


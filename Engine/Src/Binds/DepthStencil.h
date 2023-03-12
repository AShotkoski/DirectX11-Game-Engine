#pragma once
#include <Binds/Bindable.h>

// todo override ctor to allow custom mipmapping

class DepthStencil : public Bindable
{
	friend class Graphics;
	friend class RenderTarget;
public:
	DepthStencil( Graphics& gfx, UINT width, UINT height );
	virtual void Bind( Graphics& gfx ) override;
	void Clear(Graphics& gfx);
private:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthStencilView;
	
};


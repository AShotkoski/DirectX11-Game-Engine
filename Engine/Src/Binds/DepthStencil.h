#pragma once
#include <Binds/Bindable.h>
#include <Graphics/BufferResource.h>

// todo override ctor to allow custom mipmapping

class DepthStencil : public BufferResource
{
	friend class Graphics;
	friend class RenderTarget;
public:
	DepthStencil( Graphics& gfx, UINT width, UINT height );
	virtual void Bind( Graphics& gfx ) override;
	virtual void Clear(Graphics& gfx) const override;
protected:
	virtual void BindAsRT( Graphics& gfx, ID3D11DepthStencilView* pDepthStencilView ) const override;
private:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthStencilView;
	
};


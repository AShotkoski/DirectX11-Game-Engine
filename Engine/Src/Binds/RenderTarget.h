#pragma once
#include <Graphics/Graphics.h>
#include <Graphics/GraphicsResource.h>
#include <Graphics/BufferResource.h>

class DepthStencil;

class RenderTarget : public GraphicsResource, public BufferResource
{
public:
	RenderTarget( Graphics& gfx, UINT width, UINT height );
	void BindAsTex( Graphics& gfx, UINT slot ) const;
	virtual void Clear( Graphics& gfx ) const override;
	virtual void BindAsRT( Graphics& gfx ) const override;
	void BindAsRT( Graphics& gfx, const DepthStencil& ds ) const;
protected:
	virtual void BindAsRT( Graphics& gfx, ID3D11DepthStencilView* pDS ) const override;
	void BindNullShaderResource(Graphics& gfx) const;
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTargetView;
	UINT Width;
	UINT Height;
	// Used only for binding null shader rec
	mutable bool isResourceBound = false;
	mutable UINT SRVSlot = 0u;
};


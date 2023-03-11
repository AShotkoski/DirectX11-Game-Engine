#pragma once
#include <Graphics/Graphics.h>
#include <Graphics/GraphicsResource.h>
#include <Graphics/BufferResource.h>

class DepthStencil;

class RenderTarget : public GraphicsResource, public BufferResource
{
public:
	virtual void Clear( Graphics& gfx ) const override;
	virtual void BindAsRT( Graphics& gfx ) const override;
	void BindAsRT( Graphics& gfx, const DepthStencil& ds ) const;
protected:
	RenderTarget( Graphics& gfx, UINT width, UINT height );
	RenderTarget( Graphics& gfx, ID3D11Texture2D* pTex );
	virtual void BindAsRT( Graphics& gfx, ID3D11DepthStencilView* pDS ) const override;
	ID3D11RenderTargetView* pGetTargetView() const;
private:	
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTargetView;
	UINT Width;
	UINT Height;
};

class ShaderInputRenderTarget : public RenderTarget
{
public:
	ShaderInputRenderTarget( Graphics& gfx, UINT width, UINT height );
	void BindAsTex( Graphics& gfx, UINT slot ) const;
	using RenderTarget::BindAsRT;
protected:
	virtual void BindAsRT( Graphics& gfx, ID3D11DepthStencilView* pDS ) const override;
private:
	void BindNullShaderResource( Graphics& gfx ) const;
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
	// Used only for binding null shader rec
	mutable bool isResourceBound = false;
	mutable UINT SRVSlot = 0u;
};

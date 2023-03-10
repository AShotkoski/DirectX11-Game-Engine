#pragma once
#include <Graphics/Graphics.h>
#include <Graphics/GraphicsResource.h>


class DepthStencil;

class RenderTarget : public GraphicsResource
{
public:
	RenderTarget( Graphics& gfx, UINT width, UINT height );
	void BindAsTex( Graphics& gfx, UINT slot ) const;
	void BindAsRT( Graphics& gfx ) const;
	void BindAsRT( Graphics& gfx, const DepthStencil& ds ) const;
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTargetView;
	UINT Width;
	UINT Height;
	mutable bool isResourceBound = false;
};


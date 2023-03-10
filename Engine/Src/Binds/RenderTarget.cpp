#include "RenderTarget.h"
#include "DepthStencil.h"
#include <Macros.h>

RenderTarget::RenderTarget( Graphics& gfx, UINT width, UINT height )
	: Width( width )
	, Height( height )
{
	HRESULT hr;
	D3D11_TEXTURE2D_DESC tDesc = {};
	tDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	tDesc.ArraySize = 1;
	tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	tDesc.CPUAccessFlags = 0u;
	tDesc.Width = Width;
	tDesc.Height = Height;
	tDesc.MipLevels = 1;
	tDesc.Usage = D3D11_USAGE_DEFAULT;
	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = 0;
	tDesc.MiscFlags = 0u;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTex;
	THROW_FAILED_GFX( pGetDevice( gfx )->CreateTexture2D( &tDesc, nullptr, &pTex ) );

	D3D11_SHADER_RESOURCE_VIEW_DESC rDesc = {};
	rDesc.Format = tDesc.Format;
	rDesc.Texture2D.MipLevels = 1;
	rDesc.Texture2D.MostDetailedMip = 0;
	rDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	THROW_FAILED_GFX( pGetDevice( gfx )->CreateShaderResourceView( pTex.Get(), &rDesc, &pTextureView ) );

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = rDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D = D3D11_TEX2D_RTV{ 0 };
	THROW_FAILED_GFX( pGetDevice( gfx )->CreateRenderTargetView( pTex.Get(), &rtvDesc, &pTargetView ) );
}

void RenderTarget::BindAsTex( Graphics& gfx, UINT slot ) const
{
	isResourceBound = true;
	pGetContext( gfx )->PSSetShaderResources( slot, 1u, pTextureView.GetAddressOf() );
}

void RenderTarget::BindAsRT( Graphics& gfx ) const
{
	pGetContext( gfx )->OMSetRenderTargets( 1u, pTargetView.GetAddressOf(), nullptr );
}

void RenderTarget::BindAsRT( Graphics& gfx, const DepthStencil& ds ) const
{
	if ( isResourceBound )
	{
		ID3D11ShaderResourceView* const pSRV[1] = { NULL };
		pGetContext( gfx )->PSSetShaderResources( 0u, 1u, pSRV );
		isResourceBound = false;
	}
	pGetContext( gfx )->OMSetRenderTargets( 1u, pTargetView.GetAddressOf(), ds.pDepthStencilView.Get() );
}

#include "RenderTarget.h"
#include "DepthStencil.h"
#include <Macros.h>
#include <log.h>

RenderTarget::RenderTarget( Graphics& gfx, UINT width, UINT height )
	: Width( width )
	, Height( height )
{
	HRESULT hr;
	D3D11_TEXTURE2D_DESC tDesc = {};
	tDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	tDesc.ArraySize = 1;
	tDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
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

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = tDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D = D3D11_TEX2D_RTV{ 0 };
	THROW_FAILED_GFX( pGetDevice( gfx )->CreateRenderTargetView( pTex.Get(), &rtvDesc, &pTargetView ) );
}

RenderTarget::RenderTarget( Graphics& gfx, ID3D11Texture2D* pTex )
{
	HRESULT hr;

	D3D11_TEXTURE2D_DESC tDesc;
	pTex->GetDesc( &tDesc );
	Width = tDesc.Width;
	Height = tDesc.Height;

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D = D3D11_TEX2D_RTV{ 0 };
	THROW_FAILED_GFX( pGetDevice( gfx )->CreateRenderTargetView( pTex, &rtvDesc, &pTargetView ) );
}


void RenderTarget::Clear( Graphics& gfx ) const
{
	const FLOAT clearColor[4] = { 0.f,0.f,0.f,0.f };
	pGetContext( gfx )->ClearRenderTargetView( pTargetView.Get(), clearColor );
}

void RenderTarget::Bind( Graphics& gfx )
{
	BindAsRT( gfx, nullptr );
}

void RenderTarget::Bind( Graphics& gfx, const DepthStencil& ds )
{
	BindAsRT( gfx, ds.pDepthStencilView.Get()  );
}

void RenderTarget::BindAsRT( Graphics& gfx, ID3D11DepthStencilView* pDS ) const
{
	
	D3D11_VIEWPORT vp = {};
	vp.TopLeftX = (FLOAT)0;
	vp.TopLeftY = (FLOAT)0;
	vp.Width = (FLOAT)Width;
	vp.Height = (FLOAT)Height;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	pGetContext(gfx)->RSSetViewports(1u, &vp);
	pGetContext( gfx )->OMSetRenderTargets( 1u, pTargetView.GetAddressOf(), pDS );
}

ID3D11RenderTargetView* RenderTarget::pGetTargetView() const
{
	return pTargetView.Get();
}

ShaderInputRenderTarget::ShaderInputRenderTarget( Graphics& gfx, UINT width, UINT height )
	: RenderTarget(gfx,width,height)
{
	HRESULT hr;

	Microsoft::WRL::ComPtr<ID3D11Resource> pResource;
	pGetTargetView()->GetResource( &pResource );

	D3D11_SHADER_RESOURCE_VIEW_DESC rDesc = {};
	rDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	rDesc.Texture2D.MipLevels = 1;
	rDesc.Texture2D.MostDetailedMip = 0;
	rDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

	THROW_FAILED_GFX( pGetDevice( gfx )->CreateShaderResourceView( pResource.Get(), &rDesc, &pTextureView));
}

void ShaderInputRenderTarget::BindAsTex( Graphics& gfx, UINT slot ) const
{
	isResourceBound = true;
	SRVSlot = slot;
	pGetContext( gfx )->PSSetShaderResources( slot, 1u, pTextureView.GetAddressOf() );
}

void ShaderInputRenderTarget::BindAsRT( Graphics& gfx, ID3D11DepthStencilView* pDS ) const
{
	if ( isResourceBound )
	{
		BindNullShaderResource( gfx );
		isResourceBound = false;
	}
	RenderTarget::BindAsRT( gfx, pDS );
}

void ShaderInputRenderTarget::BindNullShaderResource( Graphics& gfx ) const
{
	ID3D11ShaderResourceView* const pSRV[1] = { NULL };
	pGetContext( gfx )->PSSetShaderResources( SRVSlot, 1u, pSRV );
}

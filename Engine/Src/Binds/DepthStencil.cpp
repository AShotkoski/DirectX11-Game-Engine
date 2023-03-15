#include "DepthStencil.h"
#include <log.h>
#include <Macros.h>

namespace WRL = Microsoft::WRL;

DepthStencil::DepthStencil( Graphics& gfx, UINT width, UINT height )
{
	HRESULT hr;
	// Create texture for depth/stencil
	WRL::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC         descDepth = {};
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0u;
	descDepth.MiscFlags = 0u;
	THROW_FAILED_GFX( pGetDevice(gfx)->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

	// View
	D3D11_DEPTH_STENCIL_VIEW_DESC DSviewDesc = {};
	DSviewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DSviewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DSviewDesc.Texture2D.MipSlice = 0u;
	THROW_FAILED_GFX( pGetDevice(gfx)->CreateDepthStencilView(
		pDepthStencil.Get(), &DSviewDesc, &pDepthStencilView ) );

}

void DepthStencil::Bind( Graphics& gfx )
{
	pGetContext( gfx )->OMSetRenderTargets( 0u, nullptr, pDepthStencilView.Get() );
}

void DepthStencil::Clear(Graphics& gfx) const
{
	pGetContext(gfx)->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DepthStencil::BindAsRT( Graphics& gfx, ID3D11DepthStencilView* pDepthStencilView ) const
{
	throw GFX_EXCEPT( ERROR_CANCELLED );
}

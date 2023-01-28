#include "Sampler.h"
#include "Macros.h"

Sampler::Sampler( Graphics& gfx )
{
	HRESULT hr;

	D3D11_SAMPLER_DESC sd = {};
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	THROW_FAILED_GFX( pGetDevice( gfx )->CreateSamplerState( &sd, &pSamplerState ) );
}

void Sampler::Bind( Graphics& gfx )
{
	pGetContext( gfx )->PSSetSamplers( 0u, 1u, pSamplerState.GetAddressOf() );
}

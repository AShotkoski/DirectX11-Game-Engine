#include "Sampler.h"
#include "Macros.h"
#include "BindableCodex.h"

namespace Binds
{

	Sampler::Sampler( Graphics& gfx )
	{
		HRESULT hr;
		D3D11_SAMPLER_DESC sd = {};
		sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.ComparisonFunc = D3D11_COMPARISON_LESS;
		sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		THROW_FAILED_GFX( pGetDevice( gfx )->CreateSamplerState( &sd, &pSamplerState ) );
	}

	void Sampler::Bind( Graphics& gfx )
	{
		pGetContext( gfx )->PSSetSamplers( 0u, 1u, pSamplerState.GetAddressOf() );
	}

	std::string Sampler::GenerateUID()
	{
		using namespace std::string_literals;
		return std::string( typeid( Sampler ).name() );
	}

	std::shared_ptr<Bindable> Sampler::Resolve( Graphics& gfx )
	{
		return Codex::Resolve<Sampler>(gfx);
	}

};
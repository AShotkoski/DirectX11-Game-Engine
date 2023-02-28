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
		sd.MipLODBias = 0.0f;
		sd.MinLOD = 0.0f;
		sd.MaxLOD = D3D11_FLOAT32_MAX;
		//sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		sd.Filter = D3D11_FILTER_ANISOTROPIC;
		sd.MaxAnisotropy = D3D11_MAX_MAXANISOTROPY;
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

	std::shared_ptr<Sampler> Sampler::Resolve( Graphics& gfx )
	{
		return Codex::Resolve<Sampler>(gfx);
	}

};
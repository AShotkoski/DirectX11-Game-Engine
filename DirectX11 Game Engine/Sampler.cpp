#include "Sampler.h"
#include "Macros.h"
#include "BindableCodex.h"

namespace Binds
{

	Sampler::Sampler( Graphics& gfx, D3D11_SAMPLER_DESC sd )
	{
		HRESULT hr;
		THROW_FAILED_GFX( pGetDevice( gfx )->CreateSamplerState( &sd, &pSamplerState ) );
	}

	void Sampler::Bind( Graphics& gfx )
	{
		pGetContext( gfx )->PSSetSamplers( 0u, 1u, pSamplerState.GetAddressOf() );
	}

	std::string Sampler::GenerateUID( D3D11_SAMPLER_DESC sd )
	{
		using namespace std::string_literals;
		return std::string(
			typeid( Sampler ).name() + "_"s + std::to_string( sd.Filter )
			+ std::to_string( sd.AddressU ) + std::to_string( sd.AddressV )
			+ std::to_string( sd.AddressW ) + std::to_string( sd.ComparisonFunc ) );
	}

	std::shared_ptr<Bindable> Sampler::Resolve( Graphics& gfx, D3D11_SAMPLER_DESC sd )
	{
		return Codex::Resolve<Sampler>(gfx, sd);
	}

};
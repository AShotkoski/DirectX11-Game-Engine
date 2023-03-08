#include "Rasterizer.h"
#include "BindableCodex.h"
#include <Macros.h>

namespace Binds
{
	Rasterizer::Rasterizer( Graphics& gfx, bool cullBackface )
	{
		HRESULT hr;
		CD3D11_RASTERIZER_DESC rdesc{ CD3D11_DEFAULT{} };
		if ( cullBackface )
		{
			
		}
		else
		{
			rdesc.CullMode = D3D11_CULL_NONE;
		}
		THROW_FAILED_GFX( pGetDevice( gfx )->CreateRasterizerState( &rdesc, &pRasterizerState ) );
	}
	void Rasterizer::Bind( Graphics& gfx )
	{
		pGetContext( gfx )->RSSetState( pRasterizerState.Get() );
	}
	std::string Rasterizer::GenerateUID( bool cullBackface )
	{
		using namespace std::string_literals;
		return std::string(typeid(Rasterizer).name() + "_"s + std::to_string((bool)cullBackface));
	}
	std::shared_ptr<Rasterizer> Rasterizer::Resolve( Graphics& gfx, bool cullBackface )
	{
		return Codex::Resolve<Rasterizer>(gfx, cullBackface);
	}
}

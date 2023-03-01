#include "NullPixelShader.h"
#include "BindableCodex.h"

namespace Binds
{
	NullPixelShader::NullPixelShader( Graphics& gfx )
	{}

	void NullPixelShader::Bind( Graphics& gfx )
	{
		pGetContext( gfx )->PSSetShader( nullptr, nullptr, 0u );
	}

	std::string NullPixelShader::GenerateUID()
	{
		return std::string( typeid( NullPixelShader ).name() );
	}

	std::shared_ptr<NullPixelShader> NullPixelShader::Resolve( Graphics& gfx )
	{
		return Codex::Resolve<NullPixelShader>( gfx );
	}

}
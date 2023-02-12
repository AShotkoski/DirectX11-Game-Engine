#include "VertexShader.h"
#include "Macros.h"
#include "BindableCodex.h"
#include <d3dcompiler.h>

#pragma comment(lib,"d3dcompiler.lib")

namespace Binds
{

	VertexShader::VertexShader( Graphics& gfx, const std::wstring path )
	{
			// error codes
		HRESULT hr;

		THROW_FAILED_GFX( D3DReadFileToBlob( path.c_str(), &pBlob ) );
		THROW_FAILED_GFX( pGetDevice( gfx )->CreateVertexShader( pBlob->GetBufferPointer(),
																 pBlob->GetBufferSize(),
																 nullptr, &pVertexShader ) );
	}

	void VertexShader::Bind( Graphics& gfx )
	{
		pGetContext( gfx )->VSSetShader( pVertexShader.Get(), nullptr, 0u );
	}

	ID3DBlob* VertexShader::pGetBytecode() const noexcept
	{
		return pBlob.Get();
	}

	std::string VertexShader::GenerateUID( const std::wstring path )
	{
		using namespace std::string_literals;
		std::string pathA( path.begin(), path.end() );
		return std::string( typeid( VertexShader ).name() + "_"s + pathA );
	}

	std::shared_ptr<Bindable> VertexShader::Resolve( Graphics& gfx, const std::wstring path )
	{
		return Codex::Resolve<VertexShader>(gfx,path);
	}

};
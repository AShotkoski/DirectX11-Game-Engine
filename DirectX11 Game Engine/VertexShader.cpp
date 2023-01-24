#include "VertexShader.h"
#include "Macros.h"
#include <d3dcompiler.h>

#pragma comment(lib,"d3dcompiler.lib")


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

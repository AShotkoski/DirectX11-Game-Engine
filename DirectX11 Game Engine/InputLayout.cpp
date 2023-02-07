#include "InputLayout.h"
#include "Macros.h"
#include <cassert>

namespace Binds
{

	InputLayout::InputLayout( Graphics& gfx,
							  const std::vector<D3D11_INPUT_ELEMENT_DESC>& descriptions,
							  ID3DBlob& VSByteCode )
	{
		// error handling
		HRESULT hr;
		assert( descriptions.size() > 0 );

		THROW_FAILED_GFX( pGetDevice( gfx )->CreateInputLayout(
			descriptions.data(),
			(UINT)descriptions.size(),
			VSByteCode.GetBufferPointer(),
			VSByteCode.GetBufferSize(),
			&pInputLayout ) );

	}

	void InputLayout::Bind( Graphics& gfx )
	{
		pGetContext( gfx )->IASetInputLayout( pInputLayout.Get() );
	}

};
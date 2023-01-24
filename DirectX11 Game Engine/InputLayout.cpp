#include "InputLayout.h"
#include "Macros.h"

InputLayout::InputLayout( Graphics& gfx,
						  const std::vector<D3D11_INPUT_ELEMENT_DESC>& descriptions,
						  ID3DBlob& VSByteCode)
	:
	Descriptions(descriptions)
{
	// error handling
	HRESULT hr;

	THROW_FAILED_GFX( pGetDevice( gfx )->CreateInputLayout(
		Descriptions.data(),
		(UINT)Descriptions.size(),
		VSByteCode.GetBufferPointer(),
		VSByteCode.GetBufferSize(),
		&pInputLayout ) );

}

void InputLayout::Bind( Graphics& gfx )
{
	pGetContext(gfx)->IASetInputLayout(pInputLayout.Get());
}

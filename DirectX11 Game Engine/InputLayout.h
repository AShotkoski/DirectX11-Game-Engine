#pragma once
#include "Bindable.h"
#include <vector>
#include <memory>
#include "VertexShader.h"

class InputLayout : public Bindable
{
public:
	InputLayout( Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& descriptions, 
				 ID3DBlob& VSByteCode );
	void Bind( Graphics& gfx ) override;
private:
	const std::vector<D3D11_INPUT_ELEMENT_DESC>& Descriptions;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
};


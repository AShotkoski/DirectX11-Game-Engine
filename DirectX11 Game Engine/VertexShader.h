#pragma once
#include "Bindable.h"
class VertexShader : public Bindable
{
public:
	VertexShader( Graphics& gfx, const std::wstring path );
	void Bind( Graphics& gfx ) override;
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
};


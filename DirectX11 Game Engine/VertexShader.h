#pragma once
#include "Bindable.h"

namespace Binds
{

	class VertexShader : public Bindable
	{
	public:
		VertexShader( Graphics& gfx, const std::wstring path );
		void Bind( Graphics& gfx ) override;
		ID3DBlob* pGetBytecode() const noexcept;
	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
		// Hold blob here for access in creating input layout
		Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	};

};
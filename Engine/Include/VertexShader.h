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
		static std::string GenerateUID( const std::wstring path );
		static std::shared_ptr<VertexShader> Resolve( Graphics& gfx, const std::wstring path );
	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
		// Hold blob here for access in creating input layout
		Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	};

};
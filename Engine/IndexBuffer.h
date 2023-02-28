#pragma once
#include "Bindable.h"
#include <vector>

namespace Binds
{

	class IndexBuffer : public Bindable
	{
	public:
		IndexBuffer( Graphics& gfx, const std::vector<unsigned short>& indices, std::string tag );
		void Bind( Graphics& gfx ) override;
		UINT GetIndicesCount() const noexcept;
		static std::string GenerateUID( const std::vector<unsigned short>& indices, std::string tag );
		static std::shared_ptr<IndexBuffer>
			Resolve( Graphics& gfx, const std::vector<unsigned short>& indices, std::string tag );
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
		UINT nIndices = 0;
	};

};
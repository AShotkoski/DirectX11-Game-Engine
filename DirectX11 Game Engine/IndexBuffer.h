#pragma once
#include "Bindable.h"
#include <vector>

class IndexBuffer : public Bindable
{
public:
	IndexBuffer( Graphics& gfx, const std::vector<unsigned short>& indices );
	void Bind( Graphics& gfx ) override;
	UINT GetIndicesCount() const noexcept;
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	UINT nIndices = 0;
};


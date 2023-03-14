#pragma once

#include "Drawable.h"

namespace RDG
{
	class RenderGraph;
}

class SphereSolidWhite : public Drawable
{
public:
	SphereSolidWhite( Graphics& gfx, float radius, RDG::RenderGraph* pGraph );
	void SetPos( DirectX::XMFLOAT3 position ) noexcept;
	DirectX::XMMATRIX GetTransformationMatrix() const noexcept override;
private:
	DirectX::XMFLOAT3 pos = { 0,0,0 };
};
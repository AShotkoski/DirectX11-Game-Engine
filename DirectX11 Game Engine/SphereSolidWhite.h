#pragma once

#include "Drawable.h"

class SphereSolidWhite : public DrawableBase<SphereSolidWhite>
{
public:
	SphereSolidWhite( Graphics& gfx, float radius );
	void SetPos( DirectX::XMFLOAT3 position ) noexcept;
	DirectX::XMMATRIX GetTransformationMatrix() const noexcept override;
private:
	DirectX::XMFLOAT3 pos = { 0,0,0 };
};
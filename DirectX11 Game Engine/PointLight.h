#pragma once
#include "ConstantBuffers.h"
#include "SphereSolidWhite.h"

class PointLight
{
public:
	PointLight( Graphics& gfx, float radius );
	void Bind( Graphics& gfx ) const;
	void Draw( Graphics& gfx ) const;
	void SpawnControlWindow();
private:
	struct PointLightCBuf
	{
		DirectX::XMFLOAT3 LightPos;
		float padding;
	};
private:
	DirectX::XMFLOAT3 LightPos;
	mutable SphereSolidWhite mesh;
	mutable PixelConstantBuffer<PointLightCBuf> cbuf;
};


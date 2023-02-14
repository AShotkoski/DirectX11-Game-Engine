#pragma once
#include "ConstantBuffers.h"
#include "SphereSolidWhite.h"

class PointLight
{
public:
	PointLight( Graphics& gfx, float radius, DirectX::XMFLOAT3 pos);
	void Bind( Graphics& gfx ) const;
	void Draw( Graphics& gfx ) const;
	void SpawnControlWindow();
private:
	struct PointLightCBuf
	{
		alignas(16)DirectX::XMFLOAT3 LightPos;
		alignas(16)DirectX::XMFLOAT3 ambient;
		alignas(16)DirectX::XMFLOAT3 diffuseColor;
		float diffuseIntensity;
		float attenConst;
		float attenLin;
		float attenQuad;
	};
private:
	mutable SphereSolidWhite mesh;
	mutable Binds::PixelConstantBuffer<PointLightCBuf> cbuf;
	// Light props
	DirectX::XMFLOAT3 LightPos;
	DirectX::XMFLOAT3 ambient = { 0.21f, 0.21f, 0.21f };
	DirectX::XMFLOAT3 diffuseColor = { 1.0f, 1.0f, 1.0f };
	float diffuseIntensity = 1.f;
	float attenConst = 1.f;
	float attenLin = 0.045f;
	float attenQuad = 0.0075f;
};


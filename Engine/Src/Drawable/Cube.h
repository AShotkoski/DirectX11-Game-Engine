#pragma once
#include "Drawable.h"
#include "Mesh.h"
#include "Graphics/Material.h"
#include <filesystem>

namespace RDG
{
	class RenderGraph;
}

class Cube : public Drawable
{
public:
	Cube(
		Graphics&         gfx,
		DirectX::XMFLOAT3 size,
		DirectX::XMFLOAT3 position,
		float pitch, float yaw, float roll, RDG::RenderGraph* pGraph);
	virtual DirectX::XMMATRIX GetTransformationMatrix() const noexcept override;
	void SpawnControlWindow( Graphics& gfx, std::string name );
private:
	DirectX::XMFLOAT3 size;
	// Position
	DirectX::XMFLOAT3 pos;
	// Rotation
	float pitch = 0;
	float yaw   = 0;
	float roll  = 0;
};

#pragma once
#include "Graphics/Graphics.h"
#include "Binds/Bindable.h"
#include "Graphics/Technique.h"
#include <vector>
#include <memory>
#include <optional>

namespace Binds
{
	class IndexBuffer;
	class VertexBuffer;
	class Topology;
}
class Material;
struct aiMesh;

class Drawable
{
public:
	Drawable( const Drawable& ) = delete;
	Drawable& operator=( const Drawable& ) = delete;
	void Submit() const;
	UINT GetIndexCount() const;
	void Bind(Graphics& gfx) const;
	void Accept( class TechniqueProbe& probe );
	virtual DirectX::XMMATRIX GetTransformationMatrix() const noexcept = 0;
	virtual ~Drawable() = default;
protected:
	Drawable() = default;
	Drawable(Graphics& gfx, const Material& material, const aiMesh& mesh );
	void AddTechnique( Technique technique);
protected:
	// These must be set by children
	std::shared_ptr<Binds::IndexBuffer> pIndexBuffer;
	std::shared_ptr<Binds::VertexBuffer> pVertexBuffer;
	std::shared_ptr<Binds::Topology> pTopology;
private:
	std::vector<Technique> techniques;
};

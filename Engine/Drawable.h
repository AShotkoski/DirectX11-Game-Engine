#pragma once
#include "Graphics.h"
#include "Bindable.h"
#include <vector>
#include <memory>
#include <optional>

namespace Binds
{
	class IndexBuffer;
	class VertexBuffer;
	class Topology;
}

class Drawable
{
public:
	Drawable() = default;
	Drawable( const Drawable& ) = delete;
	Drawable& operator=( const Drawable& ) = delete;
	virtual void Draw(Graphics& gfx) const;
	virtual DirectX::XMMATRIX GetTransformationMatrix() const noexcept = 0;
	virtual ~Drawable() = default;
protected:
	void AddBind( std::shared_ptr<Bindable> bind );
private:
	std::shared_ptr<Binds::IndexBuffer> pIndexBuffer;
	std::shared_ptr<Binds::VertexBuffer> pVertexBuffer;
	std::shared_ptr<Binds::Topology> pTopology;
};

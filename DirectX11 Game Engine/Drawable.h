#pragma once

#include "Graphics.h"
#include "Bindable.h"
#include "IndexedTriangleList.h"
#include "IndexBuffer.h"
#include <vector>
#include <memory>

class Drawable
{
public:
	Drawable() = default;
	virtual ~Drawable() = default;
	Drawable( const Drawable& ) = delete;
	Drawable& operator=( const Drawable& ) = delete;
	void Draw(Graphics& gfx) const;
	virtual void Update( float dt ) = 0;
	virtual DirectX::XMMATRIX GetTransformationMatrix() const noexcept = 0;
protected:
	void AddBind( std::unique_ptr<Bindable> bind );
private:
	std::vector<std::unique_ptr<Bindable>> Binds;
	// Hold a const ptr to the index buffer to access it when drawing
	const IndexBuffer* pIndexBuffer = nullptr;
};


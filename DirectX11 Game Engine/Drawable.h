#pragma once

#include "Graphics.h"
#include "Bindable.h"
#include "IndexedTriangleList.h"
#include "IndexBuffer.h"
#include <vector>
#include <memory>

class Drawable
{
	template <class ID>
	friend class DrawableBase;
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
	// Only implemented in drawablebase as a way to access static binds
	virtual const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept = 0;
	virtual const IndexBuffer* pGetStaticIndexBuffer() const noexcept = 0;
private:
	std::vector<std::unique_ptr<Bindable>> Binds;
	// Hold a const ptr to the index buffer to access it when drawing
	mutable const IndexBuffer* pIndexBuffer = nullptr;
};


// Static binds are held between instances of the same type. The template must be the classname
// wherein it is instantiated
template <class ID>
class DrawableBase : public Drawable
{
protected:
	void AddStaticBind( std::unique_ptr<Bindable> bind )
	{
		// Cache pointer to index buffer if static bindable is index buffer
		if ( typeid( *bind ) == typeid( IndexBuffer ) )
		{
			assert( pIndexBuffer == nullptr );
			staticpIndexBuffer = static_cast<IndexBuffer*>(bind.get());
		}

		StaticBinds.push_back( std::move( bind ) );
	}
	bool isStaticInitialized() const
	{
		return StaticBinds.size() > 0;
	}
	const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept override
	{
		return StaticBinds;
	}
	const IndexBuffer* pGetStaticIndexBuffer() const noexcept override
	{
		return staticpIndexBuffer;
	}
private:
	static std::vector<std::unique_ptr<Bindable>> StaticBinds;
	static const IndexBuffer* staticpIndexBuffer;
};

template <class ID>
std::vector<std::unique_ptr<Bindable>> DrawableBase<ID>::StaticBinds;
template <class ID>
const IndexBuffer* DrawableBase<ID>::staticpIndexBuffer;
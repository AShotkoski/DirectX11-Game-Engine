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
	virtual void Draw(Graphics& gfx) const;
	virtual DirectX::XMMATRIX GetTransformationMatrix() const noexcept = 0;
protected:
	void DrawNoIndex(Graphics& gfx, UINT vertCount) const;
	void AddBind( std::unique_ptr<Bindable> bind );
	// Only implemented in drawablebase as a way to access static binds
	virtual const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept = 0;
	virtual const Binds::IndexBuffer* pGetStaticIndexBuffer() const noexcept = 0;
private:
	std::vector<std::unique_ptr<Bindable>> Binds;
	// Hold a const ptr to the index buffer to access it when drawing
	mutable const Binds::IndexBuffer* pIndexBuffer = nullptr;
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
		if ( typeid( *bind ) == typeid( Binds::IndexBuffer ) )
		{
			assert( pIndexBuffer == nullptr );
			staticpIndexBuffer = static_cast<Binds::IndexBuffer*>(bind.get());
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
	const Binds::IndexBuffer* pGetStaticIndexBuffer() const noexcept override
	{
		return staticpIndexBuffer;
	}
private:
	static std::vector<std::unique_ptr<Bindable>> StaticBinds;
	static const Binds::IndexBuffer* staticpIndexBuffer;
};

template <class ID>
std::vector<std::unique_ptr<Bindable>> DrawableBase<ID>::StaticBinds;
template <class ID>
const Binds::IndexBuffer* DrawableBase<ID>::staticpIndexBuffer;
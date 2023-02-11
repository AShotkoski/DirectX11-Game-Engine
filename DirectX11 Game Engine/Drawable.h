#pragma once

#include "Graphics.h"
#include "Bindable.h"
#include "IndexBuffer.h"
#include <vector>
#include <memory>
#include <optional>

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
	void AddBind( std::shared_ptr<Bindable> bind );
	template<class T>
	std::optional<std::shared_ptr<T>> QueryBindable()
	{
		for ( auto& b : Binds )
		{
			if ( typeid<*b> == typid( T ) )
				return dynamic_cast<T>( *b );
		}
		return std::nullopt;
	}
private:
	const Binds::IndexBuffer* pIndexBuffer = nullptr;
	std::vector<std::shared_ptr<Bindable>> Binds;
};

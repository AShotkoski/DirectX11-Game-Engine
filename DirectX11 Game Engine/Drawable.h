#pragma once

#include "Graphics.h"
#include "Bindable.h"
#include "IndexedTriangleList.h"
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
	void AddBind( std::unique_ptr<Bindable> bind );
	virtual void Update( float dt ) = 0;
private:
	std::vector<std::unique_ptr<Bindable>> Binds;
};


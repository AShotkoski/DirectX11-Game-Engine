#pragma once

#include "Graphics.h"
#include "Bindable.h"
#include "IndexedTriangleList.h"
#include <vector>
#include <memory>

class Drawable
{
public:
	Drawable();
	virtual ~Drawable() = default;
	void Draw() const;

private:
	std::vector<std::unique_ptr<Bindable>> Binds;
};


#include "Drawable.h"

void Drawable::Draw( Graphics& gfx ) const
{
	for ( auto& b : Binds )
	{
		b->Bind( gfx );
	}
}

void Drawable::AddBind( std::unique_ptr<Bindable> bind )
{
	Binds.push_back( std::move( bind ) );
}

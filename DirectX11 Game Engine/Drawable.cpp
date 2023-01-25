#include "Drawable.h"
#include <cassert>

void Drawable::Draw( Graphics& gfx ) const
{
	for ( auto& b : Binds )
	{
		b->Bind( gfx );
	}
	assert("No index buffer bound." && pIndexBuffer != nullptr);
	gfx.DrawIndexed( pIndexBuffer->GetIndicesCount() );
}

void Drawable::AddBind( std::unique_ptr<Bindable> bind )
{
	// Cache ptr to index buffer when bound
	if ( typeid( *bind ) == typeid( IndexBuffer ) )
	{
		pIndexBuffer = static_cast<IndexBuffer*>(bind.get());
	}
	Binds.push_back( std::move( bind ) );
}

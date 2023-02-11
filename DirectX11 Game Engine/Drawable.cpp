#include "Drawable.h"
#include <cassert>
#include "IndexBuffer.h"

void Drawable::Draw( Graphics& gfx ) const
{	
	for ( auto& b : Binds )
	{
		b->Bind( gfx );
	}

	// Ensure index buffer exists
	if ( pIndexBuffer == nullptr )
	{
		assert("NO INDEX BUFFER FOUND\n" && false);
	}

	gfx.DrawIndexed( pIndexBuffer->GetIndicesCount() );
}
void Drawable::DrawNoIndex( Graphics& gfx, UINT vertCount ) const
{	
	for ( auto& b : Binds )
	{
		b->Bind( gfx );
	}

	gfx.Draw( vertCount, 0u );
}

void Drawable::AddBind( std::shared_ptr<Bindable> bind )
{
	if ( typeid( bind ) == typeid( Binds::IndexBuffer ) )
	{
		assert( "Attempted to bind more than 1 index buffer" && false );
		pIndexBuffer = static_cast<Binds::IndexBuffer*>(bind.get());
	}
	Binds.push_back( std::move( bind ) );
}

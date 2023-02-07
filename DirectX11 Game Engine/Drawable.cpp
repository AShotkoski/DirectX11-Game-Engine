#include "Drawable.h"
#include <cassert>
#include "IndexBuffer.h"

void Drawable::Draw( Graphics& gfx ) const
{	
	for ( auto& b : Binds )
	{
		b->Bind( gfx );
	}
	for ( auto& b : GetStaticBinds() )
	{
		b->Bind( gfx );
	}
	
	if ( pIndexBuffer == nullptr )
	{
		assert("NO INDEX BUFFER FOUND\n" && pGetStaticIndexBuffer() != nullptr);
		pIndexBuffer = pGetStaticIndexBuffer();
	}

	gfx.DrawIndexed( pIndexBuffer->GetIndicesCount() );

}

void Drawable::AddBind( std::unique_ptr<Bindable> bind )
{
	// Cache ptr to index buffer when bound
	if ( typeid( *bind ) == typeid( Binds::IndexBuffer ) )
	{
		assert( pIndexBuffer == nullptr );
		pIndexBuffer = static_cast<Binds::IndexBuffer*>(bind.get());
	}
	Binds.push_back( std::move( bind ) );
}

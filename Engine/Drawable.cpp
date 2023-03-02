#include "Drawable.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Topology.h"
#include "Technique.h"

Drawable::Drawable( Material& material )
{
	// this is where we will in future set all binds for drawable based on the vertlayout in material and stuff
	// right now I just wanna see what material will do lol
}

void Drawable::Submit( FrameCommander& frame ) const
{
	for ( auto& t : techniques )
	{
		t.Submit( frame, *this );
	}
}

UINT Drawable::GetIndexCount() const
{
	return pIndexBuffer->GetIndicesCount();
}

void Drawable::Bind(Graphics& gfx) const
{
	pIndexBuffer->Bind( gfx );
	pVertexBuffer->Bind( gfx );
	pTopology->Bind( gfx );
}

void Drawable::Accept( TechniqueProbe& probe )
{
	for ( auto& t : techniques )
	{
		t.Accept( probe );
	}
}

void Drawable::AddTechnique( Technique technique )
{
	technique.InitParentRef( *this );
	techniques.push_back( std::move(technique) );
}

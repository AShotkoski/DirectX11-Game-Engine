#include "Drawable.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Topology.h"
#include "Technique.h"

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

void Drawable::AddTechnique( Technique technique )
{
	techniques.push_back( std::move(technique) );
}

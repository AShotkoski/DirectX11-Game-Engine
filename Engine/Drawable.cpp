#include "Drawable.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Topology.h"
#include "Technique.h"
#include "Material.h"
#include "assimp/mesh.h"
#include "Vertex.h"

Drawable::Drawable( Material& material, const aiMesh& mesh )
{
	// Add techniques
	const auto& matTechs = material.GetTechniques();
	for ( auto& t : matTechs )
	{
		AddTechnique( t );
	}

	// Set our bindables
	
	// load vertices from aimesh
	{
		Vert::VertexBuffer vb( material.GetVertexLayout(), mesh );
		// for each elem in vb layout, set from aimesh for each vertex
		auto l = vb.Size();
	}
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

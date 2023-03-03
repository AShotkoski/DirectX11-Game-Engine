#include "Drawable.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Topology.h"
#include "Technique.h"
#include "Material.h"
#include "assimp/mesh.h"
#include "Vertex.h"

Drawable::Drawable(Graphics& gfx, const Material& material, const aiMesh& mesh )
{
	// Add techniques
	const auto& matTechs = material.GetTechniques();
	for ( auto& t : matTechs )
	{
		AddTechnique( t );
	}

	// Set our bindables
	const auto Tag = material.GetTag() + mesh.mName.C_Str();

	pTopology = Binds::Topology::Resolve( gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	// load vertices from aimesh
	{
		Vert::VertexBuffer vb( material.GetVertexLayout(), mesh );
		pVertexBuffer = Binds::DynamicVertexBuffer::Resolve( gfx, std::move( vb ), Tag );
	}
	// idx buffer
	{
		std::vector<unsigned short> Indices;
		Indices.reserve( ( size_t( mesh.mNumFaces ) * 3 ) );
		for ( size_t i = 0; i < mesh.mNumFaces; i++ )
		{
			const auto& face = mesh.mFaces[i];
			DCHECK_F( face.mNumIndices == 3, "Wrong topology loaded into model.");
			Indices.push_back( face.mIndices[0] );
			Indices.push_back( face.mIndices[1] );
			Indices.push_back( face.mIndices[2] );
		}
		pIndexBuffer = Binds::IndexBuffer::Resolve( gfx, Indices, Tag );
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

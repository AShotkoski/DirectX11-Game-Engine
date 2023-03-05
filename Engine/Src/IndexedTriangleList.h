#pragma once
#include "Graphics/Vertex.h"
#include <vector>
#include <cassert>
#include <DirectXMath.h>

//todo change and improve lol

//vertexBuffer, indices
class IndexedTriangleList
{
public:
	IndexedTriangleList( Vert::VertexBuffer& vb, std::vector<unsigned short> inds = {} )
		:
		vb( vb ),
		indices( std::move( inds ) )
	{
	}
	// All geometric primitives will use unit size and this is how they will be transformed.
	void Transform( DirectX::FXMMATRIX transformation )
	{
		assert( vb.Size() > 2 );
		assert( indices.size() % 3 == 0 );

		using namespace DirectX;
		for ( size_t i = 0; i < vb.Size(); i++ )
		{
			// Load ref to position
			auto& flPos = vb[i].Attribute<Vert::VertexLayout::Position_3D>();

			// Convert to xmvec
			XMVECTOR vecPos = XMLoadFloat3(&flPos);

			// Store transformed pos back into original memory, no unnecessary copying :) 
			XMStoreFloat3( &flPos, XMVector3Transform( vecPos, transformation ) );
		}
		
	}
	void SetNormalsIndependentFlat()
	{
		assert( vb.Size() > 2 );
		assert("Wrong number of indices in triangle list" &&
				indices.size() > 0 && indices.size() % 3 == 0);
		using namespace DirectX;

		// Loop over each triangle
		for (size_t i = 0; i < indices.size(); i += 3 )
		{
			// Get vertex views
			auto v0 = vb[indices[i]];
			auto v1 = vb[indices[i + 1]];
			auto v2 = vb[indices[i + 2]];
			// Get positions as xmvector
			const auto p0 = XMLoadFloat3(&v0.Attribute<Vert::VertexLayout::Position_3D>());
			const auto p1 = XMLoadFloat3(&v1.Attribute<Vert::VertexLayout::Position_3D>() );
			const auto p2 = XMLoadFloat3(&v2.Attribute<Vert::VertexLayout::Position_3D>() );

			// Calculate normal using cross product
			const auto n = XMVector3Normalize( XMVector3Cross( ( p1 - p0 ), ( p2 - p0 ) ) );

			// Store normalized normal into vb
			XMStoreFloat3( &v0.Attribute<Vert::VertexLayout::Normal>(), n );
			XMStoreFloat3( &v1.Attribute<Vert::VertexLayout::Normal>(), n );
			XMStoreFloat3( &v2.Attribute<Vert::VertexLayout::Normal>(), n );
		}
	}

	//core data
	Vert::VertexBuffer& vb;
	std::vector<unsigned short> indices;
};
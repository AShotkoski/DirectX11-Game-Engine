#pragma once
#include <vector>
#include <cassert>
#include <DirectXMath.h>

//vertices, indices
template <class V>
class IndexedTriangleList
{
public:
	IndexedTriangleList() = default;
	IndexedTriangleList( std::vector<V> verts, std::vector<unsigned short> inds )
		:
		vertices( std::move( verts ) ),
		indices( std::move( inds ) )
	{
		assert( vertices.size() > 2 );
		assert( indices.size() % 3 == 0 );
	}
	// All geometric primitives will use unit size and this is how they will be transformed.
	void Transform( DirectX::FXMMATRIX transformation )
	{
		using namespace DirectX;
		for ( auto& v : vertices )
		{
			const XMVECTOR pos = XMLoadFloat3( &v.pos );
			XMStoreFloat3( &v.pos, XMVector3Transform( pos, transformation ) );
		}
	}
	void SetNormalsIndependentFlat()
	{
		assert("Wrong number of indices in triangle list" &&
				indices.size() > 0 && indices.size() % 3 == 0);
		using namespace DirectX;

		// Loop over each triangle
		for (size_t i = 0; i < indices.size(); i += 3 )
		{
			// Get vertex positions
			const auto& v0 = XMLoadFloat3(&vertices[i].pos);
			const auto& v1 = XMLoadFloat3(&vertices[i + 1].pos);
			const auto& v2 = XMLoadFloat3(&vertices[i + 2].pos);

			// Calculate normal using cross product
			const auto n = XMVector3Normalize( XMVector3Cross( v1 - v0, v2 - v0 ) );

			// Store normalized normal into vertices
			XMStoreFloat3( &vertices[i].n, n );
			XMStoreFloat3( &vertices[i + 1].n, n );
			XMStoreFloat3( &vertices[i + 2].n, n );
		}
	}

	//core data
	std::vector<V> vertices;
	std::vector<unsigned short> indices;
};
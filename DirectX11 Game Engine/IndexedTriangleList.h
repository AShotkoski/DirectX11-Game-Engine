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
			auto& v0 = vertices[indices[i]];
			auto& v1 = vertices[indices[i + 1]];
			auto& v2 = vertices[indices[i + 2]];
			const auto p0 = XMLoadFloat3(&v0.pos);
			const auto p1 = XMLoadFloat3(&v1.pos);
			const auto p2 = XMLoadFloat3(&v2.pos);

			// Calculate normal using cross product
			const auto n = XMVector3Normalize( XMVector3Cross( ( p1 - p0 ), ( p2 - p0 ) ) );

			// Store normalized normal into vertices
			XMStoreFloat3( &v0.n, n );
			XMStoreFloat3( &v1.n, n );
			XMStoreFloat3( &v2.n, n );
		}
	}

	//core data
	std::vector<V> vertices;
	std::vector<unsigned short> indices;
};
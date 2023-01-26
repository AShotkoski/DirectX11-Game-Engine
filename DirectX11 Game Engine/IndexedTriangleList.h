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

	//core data
	std::vector<V> vertices;
	std::vector<unsigned short> indices;
};
#pragma once
#include <vector>
#include <cassert>

//vertices, indices
template <class V>
class IndexedTriangleList
{
public:
	IndexedTriangleList() = default;
	IndexedTriangleList( std::vector<V> verts, std::vector<size_t> inds )
		:
		vertices( std::move( verts ) ),
		indices( std::move( inds ) )
	{
		assert( vertices.size() > 2 );
		assert( indices.size() % 3 == 0 );
	}

	//core data
	std::vector<V> vertices;
	std::vector<size_t> indices;
};
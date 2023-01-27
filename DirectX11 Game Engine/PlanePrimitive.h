#pragma once
#include "IndexedTriangleList.h"
#include <vector>

namespace GeometricPrim
{
	class Plane
	{
	public:
		template <class V>
		static IndexedTriangleList<V> GetPlain( int nTesselations = 0 ) 
		{
			using namespace DirectX;

			const int vertsPerSide = nTesselations + 2;
			const float distBetweenVerts = 1.f / ( nTesselations + 1 );

			// Generate verts
			std::vector<XMFLOAT3> vertices;
			for ( int y = vertsPerSide - 1; y >= 0; y-- )
			{
				for ( int x = 0; x < vertsPerSide; x++ )
				{					
					vertices.emplace_back( x * distBetweenVerts, y * distBetweenVerts, 0.0f );
				}
			}

			// Generate Indices
			std::vector<unsigned short> indices;

			const auto cti = [vertsPerSide]
			( unsigned short i, unsigned short j )
			{
				return j * vertsPerSide + i;
			};

			for ( unsigned short i = 0; i < vertsPerSide - 1; i++ )
			{
				for ( unsigned short j = 0; j < vertsPerSide - 1; j++ )
				{
					indices.push_back( cti( i, j ) );
					indices.push_back( cti( i + 1, j + 1 ) );
					indices.push_back( cti( i, j + 1 ) );
					indices.push_back( cti( i, j ) );
					indices.push_back( cti( i + 1, j ) );
					indices.push_back( cti( i + 1, j + 1 ) );
				}
			}

			// Convert verts to V type
			std::vector<V> formattedV( vertices.size() );
			for ( size_t i = 0; i < vertices.size(); i++ )
			{
				formattedV[i].pos = vertices[i];
			}
			return IndexedTriangleList<V>
			(
				std::move( formattedV ), std::move( indices )
			);
		}
	};
};
#pragma once
#include "IndexedTriangleList.h"
#include "Graphics/Vertex.h"
#include <vector>

namespace GeometricPrim
{
	class Plane
	{
	public:
		static IndexedTriangleList GetPlain( Vert::VertexBuffer& vb, int nTesselations = 0 ) 
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

			// Add Verts to VertexBuffer
			vb.Reserve( vertices.size() );
			for ( size_t i = 0; i < vertices.size(); i++ )
			{
				vb[i].Attribute<Vert::VertexLayout::Position_3D>() = vertices[i];
			}
			return IndexedTriangleList
			(
				vb, std::move( indices )
			);
		}
	};
};
#pragma once
#include "IndexedTriangleList.h"
#include "Vertex.h"
#include <DirectXMath.h>

namespace GeometricPrim
{
	//
	//   COPIED MAIN MATH FROM CHILI FRAMEWORK 
	// 

	class Sphere
	{
	public:
		static IndexedTriangleList MakeTesselated( int latDiv, int longDiv, Vert::VertexBuffer& vb )
		{
			namespace dx = DirectX;
			assert( latDiv >= 3 );
			assert( longDiv >= 3 );

			constexpr float radius = 1.0f;
			const auto base = dx::XMVectorSet( 0.0f, 0.0f, radius, 0.0f );
			const float lattitudeAngle = dx::XM_PI / latDiv;
			const float longitudeAngle = 2.0f * dx::XM_PI / longDiv;
			std::vector<dx::XMFLOAT3> vertices;
			for ( int iLat = 1; iLat < latDiv; iLat++ )
			{
				const auto latBase = dx::XMVector3Transform(
					base,
					dx::XMMatrixRotationX( lattitudeAngle * iLat )
				);
				for ( int iLong = 0; iLong < longDiv; iLong++ )
				{
					vertices.emplace_back();
					auto v = dx::XMVector3Transform(
						latBase,
						dx::XMMatrixRotationZ( longitudeAngle * iLong )
					);
					dx::XMStoreFloat3( &vertices.back(), v );
				}
			}

			// add the cap vertices
			const auto iNorthPole = (unsigned short)vertices.size();
			vertices.emplace_back();
			dx::XMStoreFloat3( &vertices.back(), base );
			const auto iSouthPole = (unsigned short)vertices.size();
			vertices.emplace_back();
			dx::XMStoreFloat3( &vertices.back(), dx::XMVectorNegate( base ) );

			const auto calcIdx = [latDiv, longDiv]( unsigned short iLat, unsigned short iLong )
			{ return iLat * longDiv + iLong; };
			std::vector<unsigned short> indices;
			for ( unsigned short iLat = 0; iLat < latDiv - 2; iLat++ )
			{
				for ( unsigned short iLong = 0; iLong < longDiv - 1; iLong++ )
				{
					indices.push_back( calcIdx( iLat, iLong ) );
					indices.push_back( calcIdx( iLat + 1, iLong ) );
					indices.push_back( calcIdx( iLat, iLong + 1 ) );
					indices.push_back( calcIdx( iLat, iLong + 1 ) );
					indices.push_back( calcIdx( iLat + 1, iLong ) );
					indices.push_back( calcIdx( iLat + 1, iLong + 1 ) );
				}
				// wrap band
				indices.push_back( calcIdx( iLat, longDiv - 1 ) );
				indices.push_back( calcIdx( iLat + 1, longDiv - 1 ) );
				indices.push_back( calcIdx( iLat, 0 ) );
				indices.push_back( calcIdx( iLat, 0 ) );
				indices.push_back( calcIdx( iLat + 1, longDiv - 1 ) );
				indices.push_back( calcIdx( iLat + 1, 0 ) );
			}

			// cap fans
			for ( unsigned short iLong = 0; iLong < longDiv - 1; iLong++ )
			{
				// north
				indices.push_back( iNorthPole );
				indices.push_back( calcIdx( 0, iLong ) );
				indices.push_back( calcIdx( 0, iLong + 1 ) );
				// south
				indices.push_back( calcIdx( latDiv - 2, iLong + 1 ) );
				indices.push_back( calcIdx( latDiv - 2, iLong ) );
				indices.push_back( iSouthPole );
			}
			// wrap triangles
			// north
			indices.push_back( iNorthPole );
			indices.push_back( calcIdx( 0, longDiv - 1 ) );
			indices.push_back( calcIdx( 0, 0 ) );
			// south
			indices.push_back( calcIdx( latDiv - 2, 0 ) );
			indices.push_back( calcIdx( latDiv - 2, longDiv - 1 ) );
			indices.push_back( iSouthPole );

			IndexedTriangleList itl( vb, indices );

			itl.vb.Reserve( vertices.size() );
			for ( size_t i = 0u; i < vertices.size(); i++ )
			{
				itl.vb[i].Attribute<Vert::VertexLayout::Position_3D>() = vertices[i];
			}

			return std::move( itl );
		}
	};
};
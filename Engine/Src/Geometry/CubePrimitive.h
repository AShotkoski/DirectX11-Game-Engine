#pragma once
#include "IndexedTriangleList.h"
#include <Geometry/Vertex.h>
#include <vector>

namespace GeometricPrim
{
	class Cube
	{		
	public:
		static IndexedTriangleList GetPlain(Vert::VertexBuffer& vb)
		{
			using namespace DirectX;
			const float side = 1.0f / 2.0f;

			std::vector<XMFLOAT3> vertices;

			vertices.emplace_back( -side, -side, -side ); // 0
			vertices.emplace_back( side, -side, -side );  // 1
			vertices.emplace_back( -side, side, -side );  // 2
			vertices.emplace_back( side, side, -side );   // 3
			vertices.emplace_back( -side, -side, side );  // 4
			vertices.emplace_back( side, -side, side );   // 5
			vertices.emplace_back( -side, side, side );   // 6
			vertices.emplace_back( side, side, side );    // 7

			vb.Reserve( vertices.size() );
			for ( size_t i = 0; i < vertices.size(); i++ )
			{
				vb[i].Attribute<Vert::VertexLayout::Position_3D>() = vertices[i];
			}
			
			std::vector<unsigned short> inds =
			{
				0,2,1,   2,3,1,
				1,3,5,	 3,7,5,
				5,7,4,	 7,6,4,
				4,6,2,	 2,0,4,
				2,6,3,   6,7,3,
				0,1,4,	 1,5,4
			};
			
			return { vb, std::move( inds ) };
		}

		static IndexedTriangleList GetIndependentFaces( Vert::VertexBuffer& vb )
		{
			using namespace DirectX;
			const float side = 1.0f / 2.0f;

			std::vector<XMFLOAT3> vertices;

			vertices.push_back({ -side, -side, -side });// 0 near side
			vertices.push_back({ side, -side, -side });// 1
			vertices.push_back({ -side, side, -side });// 2
			vertices.push_back({ side, side, -side });// 3
			vertices.push_back({ -side, -side, side });// 4 far side
			vertices.push_back({ side, -side, side });// 5
			vertices.push_back({ -side, side, side });// 6
			vertices.push_back({ side, side, side });// 7
			vertices.push_back({ -side, -side, -side });// 8 left side
			vertices.push_back({ -side, side, -side });// 9
			vertices.push_back({ -side, -side, side });// 10
			vertices.push_back({ -side, side, side });// 11
			vertices.push_back({ side, -side, -side });// 12 right side
			vertices.push_back({ side, side, -side });// 13
			vertices.push_back({ side, -side, side });// 14
			vertices.push_back({ side, side, side });// 15
			vertices.push_back({ -side, -side, -side });// 16 bottom side
			vertices.push_back({ side, -side, -side });// 17
			vertices.push_back({ -side, -side, side });// 18
			vertices.push_back({ side, -side, side });// 19
			vertices.push_back({ -side, side, -side });// 20 top side
			vertices.push_back({ side, side, -side });// 21
			vertices.push_back({ -side, side, side });// 22
			vertices.push_back({ side, side, side });// 23

			vb.Reserve( vertices.size() );
			for ( size_t i = 0; i < vertices.size(); i++ )
			{
				vb[i].Attribute<Vert::VertexLayout::Position_3D>() = vertices[i];
			}
			
			std::vector<unsigned short> inds = 
				{
				0,2, 1,    2,3,1,
				4,5, 7,    4,7,6,
				8,10, 9,  10,11,9,
				12,13,15, 12,15,14,
				16,17,18, 18,17,19,
				20,23,21, 20,22,23
				};

			return { vb, std::move( inds ) };
		}

	};
};
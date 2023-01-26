#pragma once
#include "IndexedTriangleList.h"
#include <vector>

namespace GeometricPrim
{
	class Cube
	{		
	public:
		template <class V>
		static IndexedTriangleList<V> GetPlain()
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

			std::vector<V> formattedV(vertices.size());
			for ( size_t i = 0; i < vertices.size(); i++ )
			{
				formattedV[i].pos = vertices[i];
			}
			
			return IndexedTriangleList<V>{
				std::move( formattedV ), 
				{
					0,2,1,   2,3,1,
					1,3,5,	 3,7,5,
					5,7,4,	 7,6,4,
					4,6,2,	 2,0,4,
					2,6,3,   6,7,3,
					0,1,4,	 1,5,4
				}
			};
		}
	};
};
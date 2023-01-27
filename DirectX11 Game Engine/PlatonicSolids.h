#pragma once
#include "IndexedTriangleList.h"

namespace GeometricPrim
{
	class PlatonicSolids
	{
	public:
		template <class V>
		static IndexedTriangleList<V> Tetrahedron()
		{
			using namespace DirectX;

			const float a = 1.0f / 3.0f;
			const float b = (float)sqrt( 8.0f / 9.0f );
			const float c = (float)sqrt( 2.0f / 9.0f );
			const float d = (float)sqrt( 2.0f / 3.0f );

			// Set verts
			std::vector<XMFLOAT3> verts;

			verts.emplace_back( -c, d, -a );
			verts.emplace_back( b, 0.0f, -a );
			verts.emplace_back( -c, -d, -a );
			verts.emplace_back( 0.0f, 0.0f, 1.f );

			std::vector<V> formattedV(verts.size());
			for ( size_t i = 0; i < verts.size(); i++ )
			{
				formattedV[i].pos = verts[i];
			}

			return IndexedTriangleList<V>
			{ std::move( formattedV ),
			  {
				0, 3, 1,
				0, 1, 2,
				0, 2, 3,
				2, 1, 3
			}
			};
		}
	};
};
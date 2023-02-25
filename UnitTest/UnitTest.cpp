#include "../Engine/Win.h"
#include "CppUnitTest.h"
//#include "../Engine/DynamicCBuf.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(UnitTest)
	{
	public:
		
		TEST_METHOD(CBLayout)
		{
			//CB::Layout layout;
			///layout.append( CB::Float4 ); // 4 * 4  = 16 bytes
			//layout.append( CB::Matrix ); // 4 * 16 = 64 bytes
			//layout.append( CB::Float ); // 4 * 1 =   4 bytes
			////										 84 bytes
			//layout.add( CB::Float4, "pos" );
			//layout.add( CB::Matrix, "model" );
			///layout.add( CB::Float, "amount" );
			//CB::Buffer buf( std::move( layout ) );
			//buf["pos"] = DirectX::XMFLOAT4( 0, 1, 0, 1 );

			//DirectX::XMFLOAT4 roundtrip = buf["pos"];

		}
		TEST_METHOD( CBLayoutDumb )
		{
			//Assert::AreEqual( CB::Layout::GetTypeSysSize(CB::Float), size_t(4), L"float is 4 indeed");
		}
	};
}

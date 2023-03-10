#include "../Engine/Win.h"
#include "CppUnitTest.h"
#include "../Engine/DynamicCB.h"
#include "../Engine/MathUtil.h" // dx overloads
#include "../Engine/LoguruManager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(UnitTest)
	{
	public:		
		TEST_METHOD(CBRoundTrip)
		{
			CB::Layout layout;
			layout.add( CB::Float3, "pos"); 
			layout.add( CB::Matrix, "world"); 
			layout.add( CB::Float, "intensity"); 
			layout.add( CB::Float4, "four"); 
			// Expect 112 bytes, with padding accounted for
			CB::Buffer buf( std::move( layout ) );
			Assert::AreEqual( buf.sizeBytes(), (size_t)112, L"incorrect size of aligned buffer" );

			DirectX::XMFLOAT3 Position = { 13.5f, 133.999f, 0 };
			buf["pos"] = Position;

			DirectX::XMFLOAT3 roundTrip = buf["pos"];

			Assert::IsTrue( roundTrip == Position, L"data not round tripped success");
		}	

		TEST_METHOD(cbniche)
		{
			CB::Layout layout;
			layout.add( CB::Float3, "pos"); 
			layout.add( CB::Matrix, "world"); 
			layout.add( CB::Float, "intensity");  

			CB::Buffer buf( std::move( layout ) );

			buf["pos"] = DirectX::XMFLOAT3{ 13.5f, 133.999f, 0 };
			DirectX::XMFLOAT3 roundTrip = buf["pos"];

			if ( auto e = buf["notreal"]; e.Exists() )
				Assert::Fail();

			Assert::IsFalse( buf["worlds"].try_set( DirectX::XMMatrixScaling( 5, 2, -1 ) ) );
			Assert::IsTrue( buf["world"].try_set( DirectX::XMMatrixScaling( 55, 25, -15 ) ) );
			Assert::IsFalse( buf["nottrue"] );
			Assert::IsTrue( buf["intensity"] );
		}
		
	private:
		LoguruManager loguruman; // fatal handler
	};
}

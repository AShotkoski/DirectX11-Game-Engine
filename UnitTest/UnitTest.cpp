#include <Engine/src/Includers/Win.h>
#include "CppUnitTest.h"
#include <Engine/src/Util/MathUtil.h>
#include <Engine/src/Includers/LoguruManager.h>
#include <Engine/src/Graphics/DynamicCB.h>
#include <Engine/Src/Util/RyeFile.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(UnitTest)
	{
	public:		
		TEST_METHOD( RyeFiling )
		{
			RyeFile fil( "C:\\Users\\Shotk\\source\\repos\\DirectX11 Game Engine\\Engine\\logs\\test.txt" );
			fil.AppendParam( "money", 69.420 );
			fil.AppendParam( "cash", true );
			fil.AppendParam( "bitcoin", 32 );

			int in;
			fil.Resolve( "bitcoin", in );
			auto resopt = fil.Resolve<double>("money");
			float resref;
			fil.Resolve( "money", resref );
			bool b = fil.ForceResolve( "cash", false );
			int bad;
			fil.Resolve( "holy shit", bad );

		}

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

			DirectX::XMFLOAT4* pFour = &buf["four"];
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

			//Assert::ExpectException<std::runtime_error>( [&]() { buf["notreal"] = 5; } );
		}
		TEST_METHOD(cbptr)
		{
			CB::Layout layout;
			layout.add( CB::Float, "pos"); 
			layout.add( CB::Float, "intensity"); 
			layout.add( CB::Float, "power");  

			CB::Buffer buf(  layout  );
			CB::Buffer buf1(  layout  );

			buf["pos"] = 1.5f;
			buf["intensity"] = 0.5f;
			buf["power"] = 6.8f;
			buf1["power"] = 50.0f;

			float norm = buf["power"];
			float one = buf1["power"];
			
			float* pPow = &buf["power"];
			float* pPow1 = &buf1["power"];

			*pPow1 = 5.f;

			norm = buf["power"];
			one = buf1["power"];
		}
		
	private:
		LoguruManager loguruman; // fatal handler
	};
}

#include "../Engine/Win.h"
#include "CppUnitTest.h"
#include "../Engine/Loguru/loguru.hpp"
// I'm an evil, horrible person, who just want's to test private members of dynamic cbuffer
#define private public
#include "../Engine/DynamicCBuf.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(UnitTest)
	{
	public:
		
		TEST_METHOD(CBLayout)
		{
			CB::Layout layout;
			layout.append( CB::Float4 ); // 4 * 4  = 16 bytes
			layout.append( CB::Matrix ); // 4 * 16 = 64 bytes
			layout.append( CB::Float ); // 4 * 1 =   4 bytes
			//										 84 bytes
			Assert::AreEqual( layout.elements_.back().nextSlot(), size_t(84) );
		}
		TEST_METHOD( CBLayoutDumb )
		{
			Assert::AreEqual( CB::Layout::GetTypeSysSize(CB::Float), size_t(4), L"float is 4 indeed");
		}
	};
}

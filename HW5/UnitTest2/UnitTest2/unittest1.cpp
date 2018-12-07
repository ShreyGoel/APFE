#include "stdafx.h"
#include "CppUnitTest.h"
#include <stdio.h>
#include <stdlib.h>
#include "impHeaders.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTest2
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			string file_name = "arb.dat";
			vars var = read_file(file_name);
			Assert::AreEqual(arbitrage(var.obj, var.n, var.K, var.r, var.N, var.x), 0);

		BACK:

			return;
		}

	};
}
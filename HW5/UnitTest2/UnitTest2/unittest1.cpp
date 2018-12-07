#include "stdafx.h"
#include "CppUnitTest.h"
#include "impHeaders.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest2
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			double *obj = NULL;
			double *x;
			int n = 4, K = 3, N = 2;
			double r = 0.05;
			obj = (double *)calloc(n*(K + 1), sizeof(double));
			x = (double *)calloc(n, sizeof(double));


			obj[0] = 2.0; obj[1] = 5.0; obj[2] = 7; obj[3] = 3.5;
			obj[4] = 1.0; obj[5] = -4; obj[6] = 1.0; obj[7] = 0.0;
			obj[8] = 2.0; obj[9] = 7.0; obj[10] = -5.0; obj[11] = 0.0;
			obj[12] = 2.0;  obj[13] = 9.0; obj[14] = -9.0; obj[15] = 5.0;

			Assert::AreEqual(arbitrage(obj, n, K, r, N, x), 0);
		}

	};
}
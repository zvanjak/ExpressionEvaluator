#include "stdafx.h"
#include "CppUnitTest.h"

#include "../ExpressionEvaluatorLib/ExpressionEvaluator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft {
	namespace VisualStudio {
		namespace CppUnitTestFramework
		{
			template<> static std::wstring ToString<CalculatorStatus>(const CalculatorStatus& t)
			{
				std::wstring retVal;

				switch (t)
				{
				case CalculatorStatus::STATUS_OK:
					retVal = std::wstring(L"STATUS_OK"); break;
				case CalculatorStatus::SYNTAX_ERROR:
					retVal = std::wstring(L"SYNTAX_ERROR"); break;
				case CalculatorStatus::MISMATCHED_PARENTHESIS:
					retVal = std::wstring(L"MISMATCHED_PARENTHESIS"); break;
				}
				return retVal;
			}
		}
	}
}

namespace ScientificCalculatorTests
{
	TEST_CLASS(MultiParameterFunctionsTests)
	{
	public:

		TEST_METHOD(Test_SimpleTwoParamFunction) {
			Assert::AreEqual(4.0, _calculator.evaluate("4", &outStatus));
			Assert::AreEqual(4.0, _calculator.evaluate("2+2", &outStatus));
			Assert::AreEqual(52.3456, _calculator.evaluate("50+2.3456", &outStatus));
			Assert::AreEqual(3.1, _calculator.evaluate("4.5 - 1.4", &outStatus));
			Assert::AreEqual(6.0, _calculator.evaluate("2 * 3", &outStatus));
			Assert::AreEqual(5.0, _calculator.evaluate("20 /4", &outStatus));
		}

		

	private:
		ExpressionEvaluator _calculator;
		CalculatorStatus outStatus;

	};
}
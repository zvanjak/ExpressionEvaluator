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

double simple_two_param_func(double x, double y)
{
	return x + y;
}

namespace ScientificCalculatorTests
{
	TEST_CLASS(MultiParameterFunctionsTests)
	{
	public:

		TEST_METHOD(Test_SimpleTwoParamFunction1) {
			_calculator.addUserDefinedFunction("simpleFunc", new DefinedFunctionTwoParam(simple_two_param_func));

			Assert::AreEqual(4.0, _calculator.evaluate("simpleFunc(2,2)", &outStatus));
		}

		TEST_METHOD(Test_SimpleTwoParamFunction2) {
			_calculator.addUserDefinedFunction("simpleFunc", new DefinedFunctionTwoParam(simple_two_param_func));

			Assert::AreEqual(10.0, _calculator.evaluate("simpleFunc(2+2,2*3)", &outStatus));
		}

		TEST_METHOD(Test_ComplexTwoParamFunction1) {
			_calculator.addUserDefinedFunction("simpleFunc", new DefinedFunctionTwoParam(simple_two_param_func));

			Assert::AreEqual(6.0, _calculator.evaluate("simpleFunc(simpleFunc(1,1),simpleFunc(2,2))", &outStatus));
		}



	private:
		ExpressionEvaluator _calculator;
		CalculatorStatus outStatus;

	};
}
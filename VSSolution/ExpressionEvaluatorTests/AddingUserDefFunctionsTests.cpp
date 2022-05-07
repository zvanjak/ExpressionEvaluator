#include "stdafx.h"
#include "CppUnitTest.h"

#include "..\ExpressionEvaluatorLib/ExpressionEvaluator.h"

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

double simple_func(double a)
{
	return a * 2;
}
namespace ScientificCalculatorTests
{
	TEST_CLASS(AddingUserDefFunctionsTests)
	{
	public:

		TEST_METHOD(Test_AddSimpleFunction) {
			_calculator.addUserDefinedFunction("simpleFunc", new DefinedFunctionOneParam(simple_func));

			Assert::AreEqual(4.0, _calculator.evaluate("simpleFunc(2)", &outStatus));
		}

		TEST_METHOD(Test_AddSimpleFunction2) {
			_calculator.addUserDefinedFunction("simple_func", new DefinedFunctionOneParam(simple_func));

			Assert::AreEqual(4.0, _calculator.evaluate("simple_func(2)", &outStatus));
		}

	private:
		ExpressionEvaluator _calculator;
		CalculatorStatus outStatus;

	};
}
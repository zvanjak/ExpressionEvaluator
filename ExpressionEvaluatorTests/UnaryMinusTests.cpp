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



namespace ScientificCalculatorTests
{
	TEST_CLASS(UnaryMinusTests)
	{
	public:

		TEST_METHOD(Test_UnaryMinus_SimpleExpression) {
			Assert::AreEqual(-4.0, _calculator.evaluate("-4", &outStatus));
			Assert::AreEqual(0.0, _calculator.evaluate("-2+2", &outStatus));
			Assert::AreEqual(-48.3456, _calculator.evaluate("-50+2.3456", &outStatus));
			Assert::AreEqual(-3.1, _calculator.evaluate("- 4.5 + 1.4", &outStatus));
			Assert::AreEqual(-6.0, _calculator.evaluate("-2 * 3", &outStatus));
			Assert::AreEqual(-5.0, _calculator.evaluate("-20 /4", &outStatus));
		}

		TEST_METHOD(Test_UnaryMinusWithParenthesis) {
			Assert::AreEqual(1.0, _calculator.evaluate("(-2 + 3)", &outStatus));
			Assert::AreEqual(4.0, _calculator.evaluate("2 * (-2 + 3) *2", &outStatus));
		}

		TEST_METHOD(Test_UnaryMinusSimpleFunction) {
			Assert::AreEqual(-log(10), _calculator.evaluate("-log(10)", &outStatus));
			Assert::AreEqual(-sin(2), _calculator.evaluate("-sin(2)", &outStatus));
			Assert::AreEqual(-cos(2), _calculator.evaluate("-cos(2)", &outStatus));
		}

		TEST_METHOD(Test_UnaryMinusSyntaxError1) {
			_calculator.evaluate("2 + (-2 *2", &outStatus);

			Assert::AreEqual(CalculatorStatus::MISMATCHED_PARENTHESIS, outStatus);
		}
		TEST_METHOD(Test_UnaryMinusSyntaxError2) {
			double res = _calculator.evaluate("-2(2+2)", &outStatus);
			Assert::AreEqual(CalculatorStatus::SYNTAX_ERROR, outStatus);
		}

		TEST_METHOD(Test_UnaryMinusSyntaxError3) {
			double res = _calculator.evaluate("-2 3", &outStatus);
			Assert::AreEqual(CalculatorStatus::SYNTAX_ERROR, outStatus);
		}
		TEST_METHOD(Test_UnaryMinusSyntaxError4) {
			double res = _calculator.evaluate("-() + 2", &outStatus);
			Assert::AreEqual(CalculatorStatus::SYNTAX_ERROR, outStatus);
		}

	private:
		ExpressionEvaluator _calculator;
		CalculatorStatus outStatus;

	};
}
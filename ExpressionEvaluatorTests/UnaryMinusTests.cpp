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
				case CalculatorStatus::ERROR_IN_CALCULATION:
					retVal = std::wstring(L"ERROR_IN_CALCULATION"); break;
				case CalculatorStatus::NO_EQUAL_SIGN_IN_EQUATION:
					retVal = std::wstring(L"NO_EQUAL_SIGN_IN_EQUATION"); break;
				case CalculatorStatus::NO_VAR_IN_EQUATION: 
					retVal = std::wstring(L"NO_VAR_IN_EQUATION"); break;
				case CalculatorStatus::INIFINITY_VARIABLE_VALUE:
					retVal = std::wstring(L"INIFINITY_VARIABLE_VALUE"); break;
				default:
					retVal = std::wstring(L"UNKNOWN ERROR STATUS CODE"); break;
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
			Assert::AreEqual(-47.6544, _calculator.evaluate("-50+2.3456", &outStatus));
			Assert::AreEqual(-3.1, _calculator.evaluate("- 4.5 + 1.4", &outStatus));
			Assert::AreEqual(-6.0, _calculator.evaluate("-2 * 3", &outStatus));
			Assert::AreEqual(-5.0, _calculator.evaluate("-20 /4", &outStatus));
		}

		TEST_METHOD(Test_UnaryMinus_ComplexExpression) {
			Assert::AreEqual(-24.0, _calculator.evaluate("4 * (-2 * 3)", &outStatus));
			Assert::AreEqual(-1.0, _calculator.evaluate("-(-2 +3)", &outStatus));
			Assert::AreEqual(-1.0, _calculator.evaluate("(-(-2 +3))", &outStatus));
			Assert::AreEqual(-6.0, _calculator.evaluate("-3 +(-3)", &outStatus));
			Assert::AreEqual(-3.1, _calculator.evaluate("- 4.5 + 1.4", &outStatus));
			Assert::AreEqual(-6.0, _calculator.evaluate("-2 * 3", &outStatus));
			Assert::AreEqual(-5.0, _calculator.evaluate("-20 /4", &outStatus));
		}

		TEST_METHOD(Test_UnaryMinus_ComplexExpression2) {
			Assert::AreEqual(-2.0*(-1 + (2 * 4 + (-5.25)*2.5 + 3 - 5 * 6 / (-2) + 10)), _calculator.evaluate("-2.0*(-1+(2*4+(-5.25)*2.5+3-5*6/(-2)+10))", &outStatus));
			Assert::AreEqual(1000.0/10/10/10, _calculator.evaluate("1000.0/10/10/10", &outStatus));
		}

		TEST_METHOD(Test_UnaryMinusWithParenthesis) {
			Assert::AreEqual(1.0, _calculator.evaluate("(-2 + 3)", &outStatus));
			Assert::AreEqual(1.0, _calculator.evaluate("( -2 + 3)", &outStatus));
			Assert::AreEqual(4.0, _calculator.evaluate("2 * (-2 + 3) *2", &outStatus));
		}

		TEST_METHOD(Test_UnaryMinusSimpleFunction) {
			Assert::AreEqual(-log(10), _calculator.evaluate("-log(10)", &outStatus));
			Assert::AreEqual(-sin(2), _calculator.evaluate("-sin(2)", &outStatus));
			Assert::AreEqual(sin(-2), _calculator.evaluate("sin(-2)", &outStatus));
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
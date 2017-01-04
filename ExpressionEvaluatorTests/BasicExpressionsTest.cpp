#include "stdafx.h"
#include "CppUnitTest.h"

#include "..\ExpressionEvaluatorLib/ExpressionEvaluator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft {	namespace VisualStudio {	namespace CppUnitTestFramework
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
	TEST_CLASS(BasicExpressionsTest)
	{
	public:
		
		TEST_METHOD(Test_SimpleExpression) {
			Assert::AreEqual(4.0, _calculator.evaluate("4", &outStatus));
			Assert::AreEqual(4.0, _calculator.evaluate("2+2", &outStatus));
			Assert::AreEqual(52.3456, _calculator.evaluate("50+2.3456", &outStatus));
			Assert::AreEqual(3.1, _calculator.evaluate("4.5 - 1.4", &outStatus));
			Assert::AreEqual(6.0, _calculator.evaluate("2 * 3", &outStatus));
			Assert::AreEqual(5.0, _calculator.evaluate("20 /4", &outStatus));
		}

		TEST_METHOD(Test_ComplexExpression) {
			Assert::AreEqual(-3.0, _calculator.evaluate("4*2-4/2*3-5", &outStatus));
			Assert::AreEqual(15.0, _calculator.evaluate("1+2+3+4+5", &outStatus));
			Assert::AreEqual(10.0, _calculator.evaluate("100/5 / 2", &outStatus));
			Assert::AreEqual(105.0, _calculator.evaluate("3+ 2 *50 +2", &outStatus));
			Assert::AreEqual(27.0, _calculator.evaluate("2 + 3*8 -20 /4+6", &outStatus));
		}

		TEST_METHOD(Test_ExpressionWithParenthesis) {
			Assert::AreEqual(20.0, _calculator.evaluate("2 * (2+ 3)*2", &outStatus));
		}

		TEST_METHOD(Test_SimpleFunction) {
			Assert::AreEqual(log(10), _calculator.evaluate("log(10)", &outStatus));
			Assert::AreEqual(sin(2), _calculator.evaluate("sin(2)", &outStatus));
			Assert::AreEqual(cos(2), _calculator.evaluate("cos(2)", &outStatus));
		}

		TEST_METHOD(Test_ExpressionWithFunction) {
			Assert::AreEqual(2 * log(10) + 3, _calculator.evaluate("2 *log(10) + 3", &outStatus));
			Assert::AreEqual((1 + sin(2)) / 2, _calculator.evaluate("(1 + sin(2)) / 2", &outStatus));
		}

		TEST_METHOD(Test_CompleteComplexExpression1) {
			Assert::AreEqual(2 * log(10) + 3 / sin(2.5) + 2 * (2 + cos(sin(23 - log(5.345)))), _calculator.evaluate("2 *log(10) + 3/sin(  2.5)+2*(2+cos(sin(23 - log(5.345))))", &outStatus));
		}

		TEST_METHOD(Test_SyntaxError1) {
			_calculator.evaluate("2 + (2 *2", &outStatus);

			Assert::AreEqual(CalculatorStatus::MISMATCHED_PARENTHESIS, outStatus);
		}
		TEST_METHOD(Test_SyntaxError2) {
			double res = _calculator.evaluate("2(2+2)", &outStatus);
			Assert::AreEqual(CalculatorStatus::SYNTAX_ERROR, outStatus);
		}

		TEST_METHOD(Test_SyntaxError3) {
			double res = _calculator.evaluate("2 3", &outStatus);
			Assert::AreEqual(CalculatorStatus::SYNTAX_ERROR, outStatus);
		}
		TEST_METHOD(Test_SyntaxError4) {
			double res = _calculator.evaluate("()", &outStatus);
			Assert::AreEqual(CalculatorStatus::SYNTAX_ERROR, outStatus);
		}
		TEST_METHOD(Test_SyntaxError5) {
			double res = _calculator.evaluate("2 sin(10)", &outStatus);
			Assert::AreEqual(CalculatorStatus::SYNTAX_ERROR, outStatus);
		}
		TEST_METHOD(Test_SyntaxError6) {
			double res = _calculator.evaluate("2+(2+2) *3 / 5 +456 * 26 ( 4 + 5)", &outStatus);
			Assert::AreEqual(CalculatorStatus::SYNTAX_ERROR, outStatus);
		}


	private:
		ExpressionEvaluator _calculator;
		CalculatorStatus outStatus;

	};
}
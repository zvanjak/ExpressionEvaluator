#include "stdafx.h"
#include "CppUnitTest.h"

#include "..\ExpressionEvaluatorLib/ExpressionEvaluator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ScientificCalculatorTests
{
	TEST_CLASS(ErrorTests)
	{
	public:
		
		TEST_METHOD(Test_MismatchedParenthesis)
		{
			string res = _calculator.driver("2 + (3*5");

			Assert::AreEqual(_calculator.getErrorMessage(CalculatorStatus::MISMATCHED_PARENTHESIS), res);
		}

		TEST_METHOD(Test_SyntaxError)
		{
			string res = _calculator.driver("2 + + (3*5");

			Assert::AreEqual(_calculator.getErrorMessage(CalculatorStatus::SYNTAX_ERROR), res);
		}

		TEST_METHOD(Test_NoVarInEquation)
		{
			string res = _calculator.driver("2 +3 = 3*5");

			Assert::AreEqual(_calculator.getErrorMessage(CalculatorStatus::NO_VAR_IN_EQUATION), res);
		}

		TEST_METHOD(Test_NoEqualSignInEquation)
		{
			string res = _calculator.driver("2 + 3*x + 3*5");

			Assert::AreEqual(_calculator.getErrorMessage(CalculatorStatus::NO_EQUAL_SIGN_IN_EQUATION), res);
		}

	private:
		ExpressionEvaluator _calculator;
		CalculatorStatus outStatus;
	};
}
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

	private:
		ExpressionEvaluator _calculator;
		CalculatorStatus outStatus;
	};
}
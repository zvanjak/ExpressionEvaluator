#include "catch.hpp"
#include "ExpressionEvaluator.h"


TEST_CASE("Test_MismatchedParenthesis", "[errors]")
{
	ExpressionEvaluator _calculator;

	string res = _calculator.driver("2 + (3*5");

	REQUIRE(_calculator.getErrorMessage(CalculatorStatus::MISMATCHED_PARENTHESIS) == res);
}

TEST_CASE("Test_SyntaxError", "[errors]")
{
	ExpressionEvaluator _calculator;  

	string res = _calculator.driver("2 + + (3*5");

	REQUIRE(_calculator.getErrorMessage(CalculatorStatus::SYNTAX_ERROR) == res);
}

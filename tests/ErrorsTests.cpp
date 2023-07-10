#include "catch.hpp"
#include "ExpressionEvaluator.h"


TEST_CASE("Test_MismatchedParenthesis", "[errors]")
{
	ExpressionEvaluator _calculator;

	REQUIRE(_calculator.driver("2 + (3*5") == _calculator.getErrorMessage(CalculatorStatus::MISMATCHED_PARENTHESIS));
	REQUIRE(_calculator.driver("2) + (3*5") == _calculator.getErrorMessage(CalculatorStatus::MISMATCHED_PARENTHESIS));
	REQUIRE(_calculator.driver("2 + 3+5)") == _calculator.getErrorMessage(CalculatorStatus::MISMATCHED_PARENTHESIS));
	REQUIRE(_calculator.driver("(2 + sin(3)*4.5) + (3*5") == _calculator.getErrorMessage(CalculatorStatus::MISMATCHED_PARENTHESIS));
	REQUIRE(_calculator.driver(")(") == _calculator.getErrorMessage(CalculatorStatus::MISMATCHED_PARENTHESIS));
	REQUIRE(_calculator.driver("(") == _calculator.getErrorMessage(CalculatorStatus::MISMATCHED_PARENTHESIS));
	REQUIRE(_calculator.driver(")") == _calculator.getErrorMessage(CalculatorStatus::MISMATCHED_PARENTHESIS));
	REQUIRE(_calculator.driver("sin(3") == _calculator.getErrorMessage(CalculatorStatus::MISMATCHED_PARENTHESIS));
}

TEST_CASE("Test_SyntaxError", "[errors]")
{
	ExpressionEvaluator _calculator;  

	string res = _calculator.driver("2 + + (3*5");

	REQUIRE(_calculator.getErrorMessage(CalculatorStatus::SYNTAX_ERROR) == res);
	REQUIRE(_calculator.driver("sin 3") == _calculator.getErrorMessage(CalculatorStatus::SYNTAX_ERROR));
}

// runtime errors - sqrt(-1), asin(5), ...
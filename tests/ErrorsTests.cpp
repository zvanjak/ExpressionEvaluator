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

TEST_CASE("Test_InsufficientOperand", "[errors]")
{
	ExpressionEvaluator _calculator;
	CalculatorStatus outStatus;

	_calculator.evaluate("2+", &outStatus);
	REQUIRE(CalculatorStatus::INSUFFICIENT_OPERAND == outStatus);

	_calculator.evaluate("3/", &outStatus);
	REQUIRE(CalculatorStatus::INSUFFICIENT_OPERAND == outStatus);

	_calculator.evaluate("-", &outStatus);
	REQUIRE(CalculatorStatus::INSUFFICIENT_OPERAND == outStatus);

	_calculator.evaluate("sin()", &outStatus);
	REQUIRE(CalculatorStatus::SYNTAX_ERROR == outStatus);

	// missing second argument for a two-parameter function
	_calculator.addUserDefinedFunction("simpleTwoParamFunc", new DefinedFunctionTwoParam([](double x, double y) { return x + y; }));
	_calculator.evaluate("simpleTwoParamFunc(1)", &outStatus);
	REQUIRE(CalculatorStatus::INSUFFICIENT_OPERAND == outStatus);
}

TEST_CASE("Test_RuntimeMathFaults", "[errors]")
{
	ExpressionEvaluator _calculator;
	CalculatorStatus outStatus;

	_calculator.evaluate("1/0", &outStatus);
	REQUIRE(CalculatorStatus::DIVISION_BY_ZERO == outStatus);

	_calculator.evaluate("2/(1-1)", &outStatus);
	REQUIRE(CalculatorStatus::DIVISION_BY_ZERO == outStatus);

	_calculator.evaluate("sqrt(-1)", &outStatus);
	REQUIRE(CalculatorStatus::DOMAIN_ERROR == outStatus);

	_calculator.evaluate("log(-1)", &outStatus);
	REQUIRE(CalculatorStatus::DOMAIN_ERROR == outStatus);

	_calculator.evaluate("(-2)^(0.5)", &outStatus);
	REQUIRE(CalculatorStatus::DOMAIN_ERROR == outStatus);
}

// runtime errors - sqrt(-1), asin(5), ...
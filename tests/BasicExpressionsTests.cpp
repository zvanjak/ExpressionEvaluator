//#define CATCH_CONFIG_MAIN

#include "../catch/catch.hpp"

#include "ExpressionEvaluator.h"

TEST_CASE("Test_SimpleExpression", "[simple]") {
	ExpressionEvaluator _calculator;
	CalculatorStatus outStatus;
	
	REQUIRE(4.0 ==  _calculator.evaluate("4", &outStatus));
	REQUIRE(-2.0 ==  _calculator.evaluate("-2", &outStatus));

	REQUIRE(4.0 == _calculator.evaluate("2+2", &outStatus));
	REQUIRE(52.3456 == _calculator.evaluate("50+2.3456", &outStatus));
	REQUIRE(3.1 == _calculator.evaluate("4.5 - 1.4", &outStatus));
	REQUIRE(6.0 == _calculator.evaluate("2 * 3", &outStatus));
	
    REQUIRE(5.0 == _calculator.evaluate("20/4", &outStatus));
	REQUIRE(5.0 == _calculator.evaluate("20 /4", &outStatus));
	REQUIRE(5.0 == _calculator.evaluate("20/ 4", &outStatus));
	REQUIRE(5.0 == _calculator.evaluate("20 / 4", &outStatus));
}

TEST_CASE("Test_ComplexExpression", "[simple]") {
	ExpressionEvaluator _calculator;
	CalculatorStatus outStatus;
	
    REQUIRE(-3.0 == _calculator.evaluate("4*2-4/2*3-5", &outStatus));
    REQUIRE(15.0 == _calculator.evaluate("1+2+3+4+5", &outStatus));
    REQUIRE(10.0 == _calculator.evaluate("100/5 / 2", &outStatus));
    REQUIRE(105.0 == _calculator.evaluate("3+ 2 *50 +2", &outStatus));
    REQUIRE(27.0 == _calculator.evaluate("2 + 3*8 -20 /4+6", &outStatus));
}

TEST_CASE("Test_ExpressionWithParenthesis", "[simple]") {
	ExpressionEvaluator _calculator;
	CalculatorStatus outStatus;
	
    REQUIRE(20.0 == _calculator.evaluate("2 * (2+ 3)*2", &outStatus));
}

TEST_CASE("Test_ExpressionWithPowerOperation", "[simple]") {
	ExpressionEvaluator _calculator;
	CalculatorStatus outStatus;
	
    REQUIRE(pow(2, 3) == _calculator.evaluate("2^3", &outStatus));
    REQUIRE(-pow(2, 3) == _calculator.evaluate("-2^3", &outStatus));
    REQUIRE(pow(-2, 3) == _calculator.evaluate("(-2)^3", &outStatus));
    REQUIRE(pow(2, -3) == _calculator.evaluate("2^(-3)", &outStatus));
}

TEST_CASE("Test_ExpressionWithPowerOperationComplex", "[simple]") {
	ExpressionEvaluator _calculator;
	CalculatorStatus outStatus;
	
    REQUIRE(pow(2,(3 - 10 / 5.25)) / (pow(-2, 3) * 2 + 1) == _calculator.evaluate("2^(3 - 10/5.25)/((-2)^3*2+1)", &outStatus));
}

TEST_CASE("Test_ScientificNotationExpression", "[simple]") {
    ExpressionEvaluator _calculator;
    CalculatorStatus outStatus;

    REQUIRE(1000.0 + 0.02 == _calculator.evaluate("1e3 + 2E-2", &outStatus));
    REQUIRE(1.5e3 - 3.25E2 == _calculator.evaluate("1.5e3 - 3.25E2", &outStatus));
}

TEST_CASE("Test_SimpleFunction", "[simple]") {
	ExpressionEvaluator _calculator;
	CalculatorStatus outStatus;
	
    REQUIRE(log(10) == _calculator.evaluate("log(10)", &outStatus));
    REQUIRE(sin(2) == _calculator.evaluate("sin(2)", &outStatus));
    REQUIRE(cos(2) == _calculator.evaluate("cos(2)", &outStatus));
    REQUIRE(2*sin(1.5) + cos(2)*3 == _calculator.evaluate("2*sin(1.5) + cos(2)*3", &outStatus));
}

TEST_CASE("Test_ComplexFunction", "[simple]") {
	ExpressionEvaluator _calculator;
	CalculatorStatus outStatus;
	
    REQUIRE(log(10)*(2+sin(1.5)/cos(-1.58)) == _calculator.evaluate("log(10)*(2+sin(1.5)/cos(-1.58))", &outStatus));
    REQUIRE(10.5/(1+sin(2/(-4.5*0.78))) == _calculator.evaluate("10.5/(1+sin(2/(-4.5*0.78)))", &outStatus));
    REQUIRE(cos(2) == _calculator.evaluate("cos(2)", &outStatus));
}

TEST_CASE("Test_ExpressionWithFunction", "[simple]") {
	ExpressionEvaluator _calculator;
	CalculatorStatus outStatus;
	
    REQUIRE(2 * log(10) + 3 == _calculator.evaluate("2 *log(10) + 3", &outStatus));
    REQUIRE((1 + sin(2)) / 2 == _calculator.evaluate("(1 + sin(2)) / 2", &outStatus));
}

TEST_CASE("Test_CompleteComplexExpression1", "[simple]") {
	ExpressionEvaluator _calculator;
	CalculatorStatus outStatus;
	
    REQUIRE(2 * log(10) + 3 / sin(2.5) + 2 * (2 + cos(sin(23 - log(5.345)))) == _calculator.evaluate("2 *log(10) + 3/sin(  2.5)+2*(2+cos(sin(23 - log(5.345))))", &outStatus));
}

TEST_CASE("Test_SyntaxErrors", "[errors]") {
	ExpressionEvaluator _calculator;
	CalculatorStatus outStatus;
	
    _calculator.evaluate("2 + (2 *2", &outStatus);
    REQUIRE(CalculatorStatus::MISMATCHED_PARENTHESIS == outStatus);
	
    double res = _calculator.evaluate("2(2+2)", &outStatus);
    REQUIRE(CalculatorStatus::SYNTAX_ERROR == outStatus);
	
    res = _calculator.evaluate("2 3", &outStatus);
    REQUIRE(CalculatorStatus::SYNTAX_ERROR == outStatus);
	
    res = _calculator.evaluate("()", &outStatus);
    REQUIRE(CalculatorStatus::SYNTAX_ERROR == outStatus);
	
    res = _calculator.evaluate("2 sin(10)", &outStatus);
    REQUIRE(CalculatorStatus::SYNTAX_ERROR == outStatus);
	
    res = _calculator.evaluate("2+(2+2) *3 / 5 +456 * 26 ( 4 + 5)", &outStatus);
    REQUIRE(CalculatorStatus::SYNTAX_ERROR == outStatus);
	
    res = _calculator.evaluate("2^-3", &outStatus);
    REQUIRE(CalculatorStatus::SYNTAX_ERROR == outStatus);
	
    res = _calculator.evaluate("2+5-", &outStatus);
    REQUIRE(CalculatorStatus::INSUFFICIENT_OPERAND == outStatus);
}

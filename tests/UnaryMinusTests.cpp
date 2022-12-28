#include "catch.hpp"
#include "ExpressionEvaluator.h"

TEST_CASE("Test_UnaryMinus_SimpleExpression", "[errors]")
{
	ExpressionEvaluator _calculator;
    CalculatorStatus outStatus;

    REQUIRE(-4.0 == _calculator.evaluate("-4", &outStatus));
    REQUIRE(0.0 == _calculator.evaluate("-2+2", &outStatus));
    REQUIRE(-50 + 2.3456 == _calculator.evaluate("-50+2.3456", &outStatus));
    REQUIRE(-3.1 == _calculator.evaluate("- 4.5 + 1.4", &outStatus));
    REQUIRE(-6.0 == _calculator.evaluate("-2 * 3", &outStatus));
    REQUIRE(-5.0 == _calculator.evaluate("-20 /4", &outStatus));
}

TEST_CASE("Test_UnaryMinus_ComplexExpression", "[errors]") {
	ExpressionEvaluator _calculator;
    CalculatorStatus outStatus;

    REQUIRE(-24.0 == _calculator.evaluate("4 * (-2 * 3)", &outStatus));
    REQUIRE(-1.0 == _calculator.evaluate("-(-2 +3)", &outStatus));
    REQUIRE(-1.0 == _calculator.evaluate("(-(-2 +3))", &outStatus));
    REQUIRE(-6.0 == _calculator.evaluate("-3 +(-3)", &outStatus));
    REQUIRE(-3.1 == _calculator.evaluate("- 4.5 + 1.4", &outStatus));
    REQUIRE(-6.0 == _calculator.evaluate("-2 * 3", &outStatus));
    REQUIRE(-5.0 == _calculator.evaluate("-20 /4", &outStatus));
}

TEST_CASE("Test_UnaryMinus_ComplexExpression2", "[errors]") {
	ExpressionEvaluator _calculator;
    CalculatorStatus outStatus;

    REQUIRE(-2.0*(-1 + (2 * 4 + (-5.25)*2.5 + 3 - 5 * 6 / (-2) + 10)) == _calculator.evaluate("-2.0*(-1+(2*4+(-5.25)*2.5+3-5*6/(-2)+10))", &outStatus));
    REQUIRE(1000.0/10/10/10 == _calculator.evaluate("1000.0/10/10/10", &outStatus));
}

TEST_CASE("Test_UnaryMinusWithParenthesis", "[errors]") {
	ExpressionEvaluator _calculator;
    CalculatorStatus outStatus;

    REQUIRE(1.0 == _calculator.evaluate("(-2 + 3)", &outStatus));
    REQUIRE(1.0 == _calculator.evaluate("( -2 + 3)", &outStatus));
    REQUIRE(4.0 == _calculator.evaluate("2 * (-2 + 3) *2", &outStatus));
}

TEST_CASE("Test_UnaryMinusSimpleFunction", "[errors]") {
	ExpressionEvaluator _calculator;
    CalculatorStatus outStatus;

    REQUIRE(-log(10) == _calculator.evaluate("-log(10)", &outStatus));
    REQUIRE(-sin(2) == _calculator.evaluate("-sin(2)", &outStatus));
    REQUIRE(sin(-2) == _calculator.evaluate("sin(-2)", &outStatus));
    REQUIRE(-cos(2) == _calculator.evaluate("-cos(2)", &outStatus));
}

TEST_CASE("Test_UnaryMinusSyntaxError1", "[errors]") {
	ExpressionEvaluator _calculator;
    CalculatorStatus outStatus;

    _calculator.evaluate("2 + (-2 *2", &outStatus);

    REQUIRE(CalculatorStatus::MISMATCHED_PARENTHESIS == outStatus);
}
TEST_CASE("Test_UnaryMinusSyntaxError2", "[errors]") {
	ExpressionEvaluator _calculator;
    CalculatorStatus outStatus;

    double res = _calculator.evaluate("-2(2+2)", &outStatus);
    REQUIRE(CalculatorStatus::SYNTAX_ERROR == outStatus);
}
TEST_CASE("Test_UnaryMinusSyntaxError3", "[errors]") {
	ExpressionEvaluator _calculator;
    CalculatorStatus outStatus;

    double res = _calculator.evaluate("-2 3", &outStatus);
    REQUIRE(CalculatorStatus::SYNTAX_ERROR == outStatus);
}
TEST_CASE("Test_UnaryMinusSyntaxError4", "[errors]") {
	ExpressionEvaluator _calculator;
    CalculatorStatus outStatus;

    double res = _calculator.evaluate("-() + 2", &outStatus);
    REQUIRE(CalculatorStatus::SYNTAX_ERROR == outStatus);
}

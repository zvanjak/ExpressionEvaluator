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

// TEST_METHOD(Test_UnaryMinus_ComplexExpression) {
//     Assert::AreEqual(-24.0, _calculator.evaluate("4 * (-2 * 3)", &outStatus));
//     Assert::AreEqual(-1.0, _calculator.evaluate("-(-2 +3)", &outStatus));
//     Assert::AreEqual(-1.0, _calculator.evaluate("(-(-2 +3))", &outStatus));
//     Assert::AreEqual(-6.0, _calculator.evaluate("-3 +(-3)", &outStatus));
//     Assert::AreEqual(-3.1, _calculator.evaluate("- 4.5 + 1.4", &outStatus));
//     Assert::AreEqual(-6.0, _calculator.evaluate("-2 * 3", &outStatus));
//     Assert::AreEqual(-5.0, _calculator.evaluate("-20 /4", &outStatus));
// }

// TEST_METHOD(Test_UnaryMinus_ComplexExpression2) {
//     Assert::AreEqual(-2.0*(-1 + (2 * 4 + (-5.25)*2.5 + 3 - 5 * 6 / (-2) + 10)), _calculator.evaluate("-2.0*(-1+(2*4+(-5.25)*2.5+3-5*6/(-2)+10))", &outStatus));
//     Assert::AreEqual(1000.0/10/10/10, _calculator.evaluate("1000.0/10/10/10", &outStatus));
// }

// TEST_METHOD(Test_UnaryMinusWithParenthesis) {
//     Assert::AreEqual(1.0, _calculator.evaluate("(-2 + 3)", &outStatus));
//     Assert::AreEqual(1.0, _calculator.evaluate("( -2 + 3)", &outStatus));
//     Assert::AreEqual(4.0, _calculator.evaluate("2 * (-2 + 3) *2", &outStatus));
// }

// TEST_METHOD(Test_UnaryMinusSimpleFunction) {
//     Assert::AreEqual(-log(10), _calculator.evaluate("-log(10)", &outStatus));
//     Assert::AreEqual(-sin(2), _calculator.evaluate("-sin(2)", &outStatus));
//     Assert::AreEqual(sin(-2), _calculator.evaluate("sin(-2)", &outStatus));
//     Assert::AreEqual(-cos(2), _calculator.evaluate("-cos(2)", &outStatus));
// }

// TEST_METHOD(Test_UnaryMinusSyntaxError1) {
//     _calculator.evaluate("2 + (-2 *2", &outStatus);

//     Assert::AreEqual(CalculatorStatus::MISMATCHED_PARENTHESIS, outStatus);
// }
// TEST_METHOD(Test_UnaryMinusSyntaxError2) {
//     double res = _calculator.evaluate("-2(2+2)", &outStatus);
//     Assert::AreEqual(CalculatorStatus::SYNTAX_ERROR, outStatus);
// }

// TEST_METHOD(Test_UnaryMinusSyntaxError3) {
//     double res = _calculator.evaluate("-2 3", &outStatus);
//     Assert::AreEqual(CalculatorStatus::SYNTAX_ERROR, outStatus);
// }
// TEST_METHOD(Test_UnaryMinusSyntaxError4) {
//     double res = _calculator.evaluate("-() + 2", &outStatus);
//     Assert::AreEqual(CalculatorStatus::SYNTAX_ERROR, outStatus);
// }

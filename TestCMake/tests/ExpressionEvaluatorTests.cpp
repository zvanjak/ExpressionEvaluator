//#define CATCH_CONFIG_MAIN

#include "../catch/catch.hpp"

#include "ExpressionEvaluator.h"

TEST_CASE("Test_SimpleExpression", "[simple]") {
	ExpressionEvaluator _calculator;
	CalculatorStatus outStatus;
	
	REQUIRE(4.0 ==  _calculator.evaluate("4", &outStatus));
	//Assert::AreEqual(4.0, _calculator.evaluate("2+2", &outStatus));
	//Assert::AreEqual(52.3456, _calculator.evaluate("50+2.3456", &outStatus));
	//Assert::AreEqual(3.1, _calculator.evaluate("4.5 - 1.4", &outStatus));
	//Assert::AreEqual(6.0, _calculator.evaluate("2 * 3", &outStatus));
	//Assert::AreEqual(5.0, _calculator.evaluate("20 /4", &outStatus));
}

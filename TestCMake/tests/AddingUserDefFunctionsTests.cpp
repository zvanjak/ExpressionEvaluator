#include "catch.hpp"
#include "ExpressionEvaluator.h"

double simple_func(double a)
{
	return a * 2;
}

TEST_CASE("Test_AddSimpleFunction", "[errors]")
{
	ExpressionEvaluator _calculator;
    CalculatorStatus outStatus;

    _calculator.addUserDefinedFunction("simpleFunc", new DefinedFunctionOneParam(simple_func));

    REQUIRE(4.0 == _calculator.evaluate("simpleFunc(2)", &outStatus));    
}


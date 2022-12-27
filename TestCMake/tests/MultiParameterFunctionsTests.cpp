#include "catch.hpp"
#include "ExpressionEvaluator.h"


double simple_two_param_func(double x, double y)
{
	return x + y;
}

TEST_CASE("Test_SimpleTwoParamFunction1", "[errors]")
{
	ExpressionEvaluator _calculator;
    CalculatorStatus outStatus;

    _calculator.addUserDefinedFunction("simpleFunc", new DefinedFunctionTwoParam(simple_two_param_func));

    REQUIRE(4.0 == _calculator.evaluate("simpleFunc(2,2)", &outStatus));
}

TEST_CASE("Test_SimpleTwoParamFunction2", "[errors]")
{
	ExpressionEvaluator _calculator;
    CalculatorStatus outStatus;

    _calculator.addUserDefinedFunction("simpleFunc", new DefinedFunctionTwoParam(simple_two_param_func));

    REQUIRE(10.0 == _calculator.evaluate("simpleFunc(2+2,2*3)", &outStatus));
}

TEST_CASE("Test_SimpleTwoParamFunction3", "[errors]")
{
	ExpressionEvaluator _calculator;
    CalculatorStatus outStatus;

    _calculator.addUserDefinedFunction("simpleFunc", new DefinedFunctionTwoParam(simple_two_param_func));

    REQUIRE(6.0 == _calculator.evaluate("simpleFunc(simpleFunc(1,1),simpleFunc(2,2))", &outStatus));
}

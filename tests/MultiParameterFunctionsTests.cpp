#include "catch.hpp"
#include "ExpressionEvaluator.h"


double simpleTwoParamFunc(double x, double y)
{
	return x + y;
}

TEST_CASE("Test_SimpleTwoParamFunction1", "[errors]")
{
	ExpressionEvaluator _calculator;
    CalculatorStatus outStatus;

    _calculator.addUserDefinedFunction("simpleTwoParamFunc", new DefinedFunctionTwoParam(simpleTwoParamFunc));

    REQUIRE(simpleTwoParamFunc(2,2) == _calculator.evaluate("simpleTwoParamFunc(2,2)", &outStatus));
}

TEST_CASE("Test_SimpleTwoParamFunction2", "[errors]")
{
	ExpressionEvaluator _calculator;
    CalculatorStatus outStatus;

    _calculator.addUserDefinedFunction("simpleTwoParamFunc", new DefinedFunctionTwoParam(simpleTwoParamFunc));

    REQUIRE(simpleTwoParamFunc(2+2,2*3) == _calculator.evaluate("simpleTwoParamFunc(2+2,2*3)", &outStatus));
    REQUIRE(simpleTwoParamFunc(1+2*3,4*5-6) == _calculator.evaluate("simpleTwoParamFunc(1+2*3,4*5-6)", &outStatus));
    REQUIRE(simpleTwoParamFunc(2+2*3-4,2*3-1/8.5) == _calculator.evaluate("simpleTwoParamFunc(2+2*3-4,2*3-1/8.5)", &outStatus));
    
    REQUIRE(simpleTwoParamFunc(-2+3,2*3) == _calculator.evaluate("simpleTwoParamFunc(-2+3,2*3)", &outStatus));
    REQUIRE(simpleTwoParamFunc(2*sin(0.5),cos(2)*3) == _calculator.evaluate("simpleTwoParamFunc(2*sin(0.5),cos(2)*3)", &outStatus));
}

TEST_CASE("Test_SimpleTwoParamFunction3", "[errors]")
{
	ExpressionEvaluator _calculator;
    CalculatorStatus outStatus;

    _calculator.addUserDefinedFunction("simpleTwoParamFunc", new DefinedFunctionTwoParam(simpleTwoParamFunc));

    REQUIRE(simpleTwoParamFunc(simpleTwoParamFunc(1,1),simpleTwoParamFunc(2,2)) == _calculator.evaluate("simpleTwoParamFunc(simpleTwoParamFunc(1,1),simpleTwoParamFunc(2,2))", &outStatus));
    REQUIRE(simpleTwoParamFunc(simpleTwoParamFunc(2+2,2*3),simpleTwoParamFunc(1+2*3,4*5-6)) == _calculator.evaluate("simpleTwoParamFunc(simpleTwoParamFunc(2+2,2*3),simpleTwoParamFunc(1+2*3,4*5-6))", &outStatus));
    REQUIRE(simpleTwoParamFunc(simpleTwoParamFunc(-2+3,2*3),simpleTwoParamFunc(2*sin(0.5),cos(2)*3)) == _calculator.evaluate("simpleTwoParamFunc(simpleTwoParamFunc(-2+3,2*3),simpleTwoParamFunc(2*sin(0.5),cos(2)*3))", &outStatus));
}

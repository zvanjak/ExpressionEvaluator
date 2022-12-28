#include "catch.hpp"
#include "ExpressionEvaluator.h"

double simple_func(double a)
{
	return a * 2;
}

TEST_CASE("Test_AddSimpleFunction1", "[errors]")
{
	ExpressionEvaluator _calculator;
    CalculatorStatus outStatus;

    _calculator.addUserDefinedFunction("simpleFunc", new DefinedFunctionOneParam(simple_func));

    REQUIRE(4.0 == _calculator.evaluate("simpleFunc(2)", &outStatus));    
}

TEST_CASE("Test_AddSimpleFunction2", "[errors]")
{
	ExpressionEvaluator _calculator;
    CalculatorStatus outStatus;

    _calculator.addUserDefinedFunction("simpleFunc", new DefinedFunctionOneParam(simple_func));

    REQUIRE(8.0 == _calculator.evaluate("simpleFunc(2+2)", &outStatus));    
}

TEST_CASE("Test_AddSimpleFunction3", "[errors]")
{
	ExpressionEvaluator _calculator;
    CalculatorStatus outStatus;

    _calculator.addUserDefinedFunction("simpleFunc", new DefinedFunctionOneParam(simple_func));

    REQUIRE(simple_func(sin(2)) == _calculator.evaluate("simpleFunc(sin(2))", &outStatus));    
}
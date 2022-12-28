#include "catch.hpp"
#include "ExpressionEvaluator.h"

double simpleFunc(double a)
{
	return a * 2;
}

TEST_CASE("Test_AddSimpleFunction1", "[errors]")
{
	ExpressionEvaluator _calculator;
    CalculatorStatus outStatus;

    _calculator.addUserDefinedFunction("simpleFunc", new DefinedFunctionOneParam(simpleFunc));

    REQUIRE(simpleFunc(2) == _calculator.evaluate("simpleFunc(2)", &outStatus));    
}

TEST_CASE("Test_AddSimpleFunction2", "[errors]")
{
	ExpressionEvaluator _calculator;
    CalculatorStatus outStatus;

    _calculator.addUserDefinedFunction("simpleFunc", new DefinedFunctionOneParam(simpleFunc));

    REQUIRE(simpleFunc(2+2) == _calculator.evaluate("simpleFunc(2+2)", &outStatus));    
}

TEST_CASE("Test_AddSimpleFunction3", "[errors]")
{
	ExpressionEvaluator _calculator;
    CalculatorStatus outStatus;

    _calculator.addUserDefinedFunction("simpleFunc", new DefinedFunctionOneParam(simpleFunc));

    REQUIRE(simpleFunc(sin(2)) == _calculator.evaluate("simpleFunc(sin(2))", &outStatus));    
}
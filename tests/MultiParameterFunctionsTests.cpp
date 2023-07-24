#include "catch.hpp"
#include "ExpressionEvaluator.h"


double simpleTwoParamFunc(double x, double y) {	return x + y; }

double subTwoParamFunc(double x, double y)    {	return x - y; }

TEST_CASE("Test_TwoParamFunction_SimpleEvaluation", "[errors]")
{
	ExpressionEvaluator _calculator;
    CalculatorStatus outStatus;

    _calculator.addUserDefinedFunction("simpleTwoParamFunc", new DefinedFunctionTwoParam(simpleTwoParamFunc));
    _calculator.addUserDefinedFunction("subTwoParamFunc", new DefinedFunctionTwoParam(subTwoParamFunc));

    REQUIRE(simpleTwoParamFunc(2,4) == _calculator.evaluate("simpleTwoParamFunc(2,4)", &outStatus));
    REQUIRE(subTwoParamFunc(5,2) == _calculator.evaluate("subTwoParamFunc(5,2)", &outStatus));
}

TEST_CASE("Test_TwoParamFunction_SimpleExprAsParam", "[errors]")
{
	ExpressionEvaluator _calculator;
    CalculatorStatus outStatus;

    _calculator.addUserDefinedFunction("simpleTwoParamFunc", new DefinedFunctionTwoParam(simpleTwoParamFunc));
    _calculator.addUserDefinedFunction("subTwoParamFunc", new DefinedFunctionTwoParam(subTwoParamFunc));

    REQUIRE(simpleTwoParamFunc(2+2,2*3) == _calculator.evaluate("simpleTwoParamFunc(2+2,2*3)", &outStatus));
    REQUIRE(simpleTwoParamFunc(1+2*3,4*5-6) == _calculator.evaluate("simpleTwoParamFunc(1+2*3,4*5-6)", &outStatus));
    REQUIRE(simpleTwoParamFunc(2+2*3-4,2*3-1/8.5) == _calculator.evaluate("simpleTwoParamFunc(2+2*3-4,2*3-1/8.5)", &outStatus));
    
    REQUIRE(simpleTwoParamFunc(-2+3,2*3) == _calculator.evaluate("simpleTwoParamFunc(-2+3,2*3)", &outStatus));
    REQUIRE(simpleTwoParamFunc(2*sin(0.5),cos(2)*3) == _calculator.evaluate("simpleTwoParamFunc(2*sin(0.5),cos(2)*3)", &outStatus));

    REQUIRE(subTwoParamFunc(2+2,2*3) == _calculator.evaluate("subTwoParamFunc(2+2,2*3)", &outStatus));
    REQUIRE(subTwoParamFunc(1+2*3,4*5-6) == _calculator.evaluate("subTwoParamFunc(1+2*3,4*5-6)", &outStatus));
    REQUIRE(subTwoParamFunc(2+2*3-4,2*3-1/8.5) == _calculator.evaluate("subTwoParamFunc(2+2*3-4,2*3-1/8.5)", &outStatus));
    
    REQUIRE(subTwoParamFunc(-2+3,2*3) == _calculator.evaluate("subTwoParamFunc(-2+3,2*3)", &outStatus));
    REQUIRE(subTwoParamFunc(2*sin(0.5),cos(2)*3) == _calculator.evaluate("subTwoParamFunc(2*sin(0.5),cos(2)*3)", &outStatus));
}

TEST_CASE("Test_TwoParamFunction_ComplexExprAsParam", "[errors]")
{
	ExpressionEvaluator _calculator;
    CalculatorStatus outStatus;

    _calculator.addUserDefinedFunction("simpleTwoParamFunc", new DefinedFunctionTwoParam(simpleTwoParamFunc));

    REQUIRE(simpleTwoParamFunc(1 * (-1.5 + 5), 10.5/(1+sin(2/(-4.5*0.78)))) == _calculator.evaluate("simpleTwoParamFunc(1 * (-1.5 + 5), 10.5/(1+sin(2/(-4.5*0.78))))", &outStatus));    
}

TEST_CASE("Test_TwoParamFunction_CallToFuncsAsParam", "[errors]")
{
	ExpressionEvaluator _calculator;
    CalculatorStatus outStatus;

    _calculator.addUserDefinedFunction("simpleTwoParamFunc", new DefinedFunctionTwoParam(simpleTwoParamFunc));

    // REQUIRE(simpleTwoParamFunc(sin(2), 2) == _calculator.evaluate("simpleTwoParamFunc(sin(2), 2)", &outStatus));    
    REQUIRE(simpleTwoParamFunc(cos(2), -2) == _calculator.evaluate("simpleTwoParamFunc(cos(2), -2)", &outStatus));    
    // REQUIRE(simpleTwoParamFunc(exp(2), -tan(0.5)) == _calculator.evaluate("simpleTwoParamFunc(exp(2), -tan(0.5))", &outStatus));    
    // REQUIRE(simpleTwoParamFunc(exp(2 * sin(3.15)), -tan(0.5)) == _calculator.evaluate("simpleTwoParamFunc(exp(2 * sin(3.15)), -tan(0.5))", &outStatus));    
}

TEST_CASE("Test_TwoParamFunction_RecursiveCallAsParam", "[errors]")
{
	ExpressionEvaluator _calculator;
    CalculatorStatus outStatus;

    _calculator.addUserDefinedFunction("simpleTwoParamFunc", new DefinedFunctionTwoParam(simpleTwoParamFunc));

    REQUIRE(simpleTwoParamFunc(simpleTwoParamFunc(1,1),simpleTwoParamFunc(2,2)) == _calculator.evaluate("simpleTwoParamFunc(simpleTwoParamFunc(1,1),simpleTwoParamFunc(2,2))", &outStatus));
    REQUIRE(simpleTwoParamFunc(simpleTwoParamFunc(2+2,2*3),simpleTwoParamFunc(1+2*3,4*5-6)) == _calculator.evaluate("simpleTwoParamFunc(simpleTwoParamFunc(2+2,2*3),simpleTwoParamFunc(1+2*3,4*5-6))", &outStatus));
    REQUIRE(simpleTwoParamFunc(simpleTwoParamFunc(-2+3,2*3),simpleTwoParamFunc(2*sin(0.5),cos(2)*3)) == _calculator.evaluate("simpleTwoParamFunc(simpleTwoParamFunc(-2+3,2*3),simpleTwoParamFunc(2*sin(0.5),cos(2)*3))", &outStatus));
}

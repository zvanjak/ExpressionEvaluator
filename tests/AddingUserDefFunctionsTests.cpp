#include "catch.hpp"
#include "ExpressionEvaluator.h"

double simpleFunc(double a)
{
	return a * 2;
}

TEST_CASE("Test_UserDefinedFunction_SimpleEvaluation", "[errors]")
{
	ExpressionEvaluator _calculator;
    CalculatorStatus outStatus;

    _calculator.addUserDefinedFunction("simpleFunc", new DefinedFunctionOneParam(simpleFunc));

    REQUIRE(simpleFunc(2) == _calculator.evaluate("simpleFunc(2)", &outStatus));    
}

TEST_CASE("Test_UserDefinedFunction_SimpleExprAsParam", "[errors]")
{
	ExpressionEvaluator _calculator;
    CalculatorStatus outStatus;

    _calculator.addUserDefinedFunction("simpleFunc", new DefinedFunctionOneParam(simpleFunc));

    REQUIRE(simpleFunc(2+2) == _calculator.evaluate("simpleFunc(2+2)", &outStatus));    
    REQUIRE(simpleFunc(2-3) == _calculator.evaluate("simpleFunc(2-3)", &outStatus));    
    REQUIRE(simpleFunc(2 * (1 - 2)) == _calculator.evaluate("simpleFunc(2 * (1 - 2))", &outStatus));    
}

TEST_CASE("Test_UserDefinedFunction_ComplexExprAsParam", "[errors]")
{
	ExpressionEvaluator _calculator;
    CalculatorStatus outStatus;

    _calculator.addUserDefinedFunction("simpleFunc", new DefinedFunctionOneParam(simpleFunc));

    REQUIRE(simpleFunc(10.5/(1+sin(2/(-4.5*0.78)))) == _calculator.evaluate("simpleFunc(10.5/(1+sin(2/(-4.5*0.78))))", &outStatus));    
}

TEST_CASE("Test_UserDefinedFunction_CallToFuncsAsParam", "[errors]")
{
	ExpressionEvaluator _calculator;
    CalculatorStatus outStatus;

    _calculator.addUserDefinedFunction("simpleFunc", new DefinedFunctionOneParam(simpleFunc));

    REQUIRE(simpleFunc(sin(2)) == _calculator.evaluate("simpleFunc(sin(2))", &outStatus));    
    REQUIRE(simpleFunc(cos(2)) == _calculator.evaluate("simpleFunc(cos(2))", &outStatus));    
    REQUIRE(simpleFunc(exp(2)) == _calculator.evaluate("simpleFunc(exp(2))", &outStatus));    
}

TEST_CASE("Test_UserDefinedFunction_RecursiveCallAsParam", "[errors]")
{
	ExpressionEvaluator _calculator;
    CalculatorStatus outStatus;

    _calculator.addUserDefinedFunction("simpleFunc", new DefinedFunctionOneParam(simpleFunc));

    REQUIRE(simpleFunc(simpleFunc(2)) == _calculator.evaluate("simpleFunc(simpleFunc(2))", &outStatus));    
}
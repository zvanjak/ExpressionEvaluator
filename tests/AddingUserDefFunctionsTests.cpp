#include "catch.hpp"
#include "ExpressionEvaluator.h"
#include <memory>

double simpleFunc(double a)
{
	return a * 2;
}

double tripleFunc(double a)
{
    return a * 3;
}

struct TrackingFunctionOneParam : public DefinedFunctionOneParam
{
    static int destructCount;

    explicit TrackingFunctionOneParam(double(*fn)(double)) : DefinedFunctionOneParam(fn) {}
    ~TrackingFunctionOneParam() override { ++destructCount; }
};

int TrackingFunctionOneParam::destructCount = 0;

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
    REQUIRE(simpleFunc(-2) == _calculator.evaluate("simpleFunc(-2)", &outStatus));    
    REQUIRE(simpleFunc(-2*3) == _calculator.evaluate("simpleFunc(-2*3)", &outStatus));    
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
    REQUIRE(simpleFunc(exp(2 * sin(3.15))) == _calculator.evaluate("simpleFunc(exp(2 * sin(3.15)))", &outStatus));    
}

TEST_CASE("Test_UserDefinedFunction_RecursiveCallAsParam", "[errors]")
{
	ExpressionEvaluator _calculator;
    CalculatorStatus outStatus;

    _calculator.addUserDefinedFunction("simpleFunc", new DefinedFunctionOneParam(simpleFunc));

    REQUIRE(simpleFunc(simpleFunc(2)) == _calculator.evaluate("simpleFunc(simpleFunc(2))", &outStatus));    
    REQUIRE(simpleFunc(simpleFunc(2+2) + 2) == _calculator.evaluate("simpleFunc(simpleFunc(2+2) + 2)", &outStatus));    
}

TEST_CASE("Test_UserDefinedFunction_InComplexExpression", "[errors]")
{
	ExpressionEvaluator _calculator;
    CalculatorStatus outStatus;

    _calculator.addUserDefinedFunction("simpleFunc", new DefinedFunctionOneParam(simpleFunc));

    REQUIRE(sin(simpleFunc(-2) / 3) + simpleFunc(cos(1.5)) == _calculator.evaluate("sin(simpleFunc(-2) / 3) + simpleFunc(cos(1.5))", &outStatus));    
}

TEST_CASE("Test_UserDefinedFunction_OwnershipTransferred", "[ownership]")
{
    TrackingFunctionOneParam::destructCount = 0;

    {
        ExpressionEvaluator calc;
        CalculatorStatus status;

        auto owned = std::make_unique<TrackingFunctionOneParam>(tripleFunc);
        calc.addUserDefinedFunction("triple", owned.release());

        status = CalculatorStatus::STATUS_OK;
        REQUIRE(tripleFunc(4.0) == calc.evaluate("triple(4)", &status));
        REQUIRE(status == CalculatorStatus::STATUS_OK);
    }

    REQUIRE(TrackingFunctionOneParam::destructCount == 1);
}
#include "../catch/catch.hpp"
#include <sstream>

#include "ExpressionEvaluator.h"


TEST_CASE("Tokenizer_SimpleExpressionTest", "[errors]")
{
	std::stringstream s("2+2");
	Tokenizer tokenizer(s);
	std::unordered_map<string, DefinedFunction *> defFunc;

	Token t = tokenizer.getNext(defFunc);		REQUIRE(2.0 == t.numberValue);
	t = tokenizer.getNext(defFunc);					REQUIRE(TokenType::plus == t.tokenType);
	t = tokenizer.getNext(defFunc);					REQUIRE(2.0 == t.numberValue);
	t = tokenizer.getNext(defFunc);					REQUIRE(TokenType::end == t.tokenType);
}

TEST_CASE("Tokenizer_SimpleExpressionWithUnaryMinusTest", "[errors]")
{
	std::stringstream s("-2+2");
	Tokenizer tokenizer(s);
	std::unordered_map<string, DefinedFunction *> defFunc;

	Token t = tokenizer.getNext(defFunc);		REQUIRE(TokenType::minus == t.tokenType);
	t = tokenizer.getNext(defFunc);					REQUIRE(2.0 == t.numberValue);
	t = tokenizer.getNext(defFunc);					REQUIRE(TokenType::plus == t.tokenType);
	t = tokenizer.getNext(defFunc);					REQUIRE(2.0 == t.numberValue);
	t = tokenizer.getNext(defFunc);					REQUIRE(TokenType::end == t.tokenType);
}

TEST_CASE("Tokenizer_SimpleExpressionTest_WithSpaces", "[errors]")
{
	std::stringstream s("2   * 2");
	Tokenizer tokenizer(s);
	std::unordered_map<string, DefinedFunction *> defFunc;

	Token t = tokenizer.getNext(defFunc);	REQUIRE(2.0 == t.numberValue);
	t = tokenizer.getNext(defFunc);				REQUIRE(TokenType::mul == t.tokenType);
	t = tokenizer.getNext(defFunc);				REQUIRE(2.0 == t.numberValue);
	t = tokenizer.getNext(defFunc);				REQUIRE(TokenType::end == t.tokenType);
}

TEST_CASE("Tokenizer_SimpleVarTest", "[errors]")
{
	std::stringstream s("x=2");
	Tokenizer tokenizer(s);
	std::unordered_map<string, DefinedFunction *> defFunc;

	Token t = tokenizer.getNext(defFunc);	REQUIRE(TokenType::name == t.tokenType);
	t = tokenizer.getNext(defFunc);				REQUIRE(TokenType::assign == t.tokenType);
	t = tokenizer.getNext(defFunc);				REQUIRE(2.0 == t.numberValue);
}
TEST_CASE("Tokenizer_SimpleVarTest_WithSpaces", "[errors]")
{
	std::stringstream s("x  = 2");
	Tokenizer tokenizer(s);
	std::unordered_map<string, DefinedFunction *> defFunc;

	Token t = tokenizer.getNext(defFunc);	REQUIRE(TokenType::name == t.tokenType);
	t = tokenizer.getNext(defFunc);				REQUIRE(TokenType::assign == t.tokenType);
	t = tokenizer.getNext(defFunc);				REQUIRE(2.0 == t.numberValue);
}
TEST_CASE("Tokenizer_TestWithParenthesis", "[errors]")
{
	std::stringstream s("3*(2+3)");
	Tokenizer tokenizer(s);
	std::unordered_map<string, DefinedFunction *> defFunc;

	Token t = tokenizer.getNext(defFunc);	REQUIRE(TokenType::number == t.tokenType);
	t = tokenizer.getNext(defFunc);				REQUIRE(TokenType::mul == t.tokenType);
	t = tokenizer.getNext(defFunc);				REQUIRE(TokenType::left == t.tokenType);
	t = tokenizer.getNext(defFunc);				REQUIRE(TokenType::number == t.tokenType);
	t = tokenizer.getNext(defFunc);				REQUIRE(TokenType::plus == t.tokenType);
	t = tokenizer.getNext(defFunc);				REQUIRE(TokenType::number == t.tokenType);
	t = tokenizer.getNext(defFunc);				REQUIRE(TokenType::right == t.tokenType);
}


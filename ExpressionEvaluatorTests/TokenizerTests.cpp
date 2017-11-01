#include "stdafx.h"
#include "CppUnitTest.h"

#include <sstream>

#include "..\ExpressionEvaluatorLib/ExpressionEvaluator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft { namespace VisualStudio {	namespace CppUnitTestFramework
{
	template<> static std::wstring ToString<TokenType>(const TokenType& t) 
	{ 
		std::wstring retVal;

		switch (t)
		{
			case TokenType::assign :
				retVal = std::wstring(L"assign"); break;
			case TokenType::div :
				retVal = std::wstring(L"div"); break;
			case TokenType::plus :
				retVal = std::wstring(L"plus"); break;
			case TokenType::mul :
				retVal = std::wstring(L"mul"); break;
			case TokenType::minus :
				retVal = std::wstring(L"minus"); break;
			case TokenType::name :
				retVal = std::wstring(L"name"); break;
			case TokenType::number :
				retVal = std::wstring(L"number"); break;
			case TokenType::end :
				retVal = std::wstring(L"end"); break;
		}
		return retVal; 
	}
}}}

namespace ScientificCalculatorTests
{
	TEST_CLASS(TokenizerTests)
	{
	public:
		
		TEST_METHOD(Tokenizer_SimpleExpressionTest)
		{
			std::stringstream s("2+2");
			Tokenizer tokenizer(s);
			std::unordered_map<string, double(*)(double)> defFunc;

			Token t = tokenizer.getNext(defFunc);			Assert::AreEqual(2.0, t.numberValue);
			t = tokenizer.getNext(defFunc);					Assert::AreEqual(TokenType::plus, t.tokenType);
			t = tokenizer.getNext(defFunc);					Assert::AreEqual(2.0, t.numberValue);
			t = tokenizer.getNext(defFunc);					Assert::AreEqual(TokenType::end, t.tokenType);
		}

		TEST_METHOD(Tokenizer_SimpleExpressionWithUnaryMinusTest)
		{
			std::stringstream s("-2+2");
			Tokenizer tokenizer(s);
			std::unordered_map<string, double(*)(double)> defFunc;

			Token t = tokenizer.getNext(defFunc);			Assert::AreEqual(TokenType::minus, t.tokenType);
			t = tokenizer.getNext(defFunc);					Assert::AreEqual(2.0, t.numberValue);
			t = tokenizer.getNext(defFunc);					Assert::AreEqual(TokenType::plus, t.tokenType);
			t = tokenizer.getNext(defFunc);					Assert::AreEqual(2.0, t.numberValue);
			t = tokenizer.getNext(defFunc);					Assert::AreEqual(TokenType::end, t.tokenType);
		}

		TEST_METHOD(Tokenizer_SimpleExpressionTest_WithSpaces)
		{
			std::stringstream s("2   * 2");
			Tokenizer tokenizer(s);
			std::unordered_map<string, double(*)(double)> defFunc;

			Token t = tokenizer.getNext(defFunc);		Assert::AreEqual(2.0, t.numberValue);
			t = tokenizer.getNext(defFunc);				Assert::AreEqual(TokenType::mul, t.tokenType);
			t = tokenizer.getNext(defFunc);				Assert::AreEqual(2.0, t.numberValue);
			t = tokenizer.getNext(defFunc);				Assert::AreEqual(TokenType::end, t.tokenType);
		}

		TEST_METHOD(Tokenizer_SimpleVarTest)
		{
			std::stringstream s("x=2");
			Tokenizer tokenizer(s);
			std::unordered_map<string, double(*)(double)> defFunc;

			Token t = tokenizer.getNext(defFunc);		Assert::AreEqual(TokenType::name, t.tokenType);
			t = tokenizer.getNext(defFunc);				Assert::AreEqual(TokenType::assign, t.tokenType);
			t = tokenizer.getNext(defFunc);				Assert::AreEqual(2.0, t.numberValue);
		}
		TEST_METHOD(Tokenizer_SimpleVarTest_WithSpaces)
		{
			std::stringstream s("x  = 2");
			Tokenizer tokenizer(s);
			std::unordered_map<string, double(*)(double)> defFunc;

			Token t = tokenizer.getNext(defFunc);		Assert::AreEqual(TokenType::name, t.tokenType);
			t = tokenizer.getNext(defFunc);				Assert::AreEqual(TokenType::assign, t.tokenType);
			t = tokenizer.getNext(defFunc);				Assert::AreEqual(2.0, t.numberValue);
		}
		TEST_METHOD(Tokenizer_TestWithParenthesis)
		{
			std::stringstream s("3*(2+3)");
			Tokenizer tokenizer(s);
			std::unordered_map<string, double(*)(double)> defFunc;

			Token t = tokenizer.getNext(defFunc);		Assert::AreEqual(TokenType::number, t.tokenType);
			t = tokenizer.getNext(defFunc);				Assert::AreEqual(TokenType::mul, t.tokenType);
			t = tokenizer.getNext(defFunc);				Assert::AreEqual(TokenType::left, t.tokenType);
			t = tokenizer.getNext(defFunc);				Assert::AreEqual(TokenType::number, t.tokenType);
			t = tokenizer.getNext(defFunc);				Assert::AreEqual(TokenType::plus, t.tokenType);
			t = tokenizer.getNext(defFunc);				Assert::AreEqual(TokenType::number, t.tokenType);
			t = tokenizer.getNext(defFunc);				Assert::AreEqual(TokenType::right, t.tokenType);
		}
	};
}
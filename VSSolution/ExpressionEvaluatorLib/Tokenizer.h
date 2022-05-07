#pragma once

#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include "DefinedFunction.h"

using std::string;
using std::stringstream;
using std::vector;

enum class TokenType : char
{
	name, number, function, comma, end,
	plus = '+',
	minus = '-',
	unary_minus = '~',
	mul = '*',
	div = '/',
	pow = '^',
	left = '(',
	right = ')',
	assign = '='
};

struct Token
{
	TokenType	tokenType;
	string		stringValue;
	double		numberValue;
};

class Tokenizer
{
public:
	Tokenizer(stringstream& s) : _inputStream{ &s } { _currentToken.tokenType = TokenType::end; }
	~Tokenizer() {}

	static bool isTokenOperator(Token &t)
	{
		return  t.tokenType == TokenType::plus ||
			t.tokenType == TokenType::minus ||
			t.tokenType == TokenType::unary_minus ||
			t.tokenType == TokenType::mul ||
			t.tokenType == TokenType::div ||
			t.tokenType == TokenType::pow;
	}

	Token Tokenizer::getNext(std::unordered_map<string, DefinedFunction *> inFunctionNames)
	{
		char ch = 0;

		do
		{
			if (!_inputStream->get(ch))
				return _currentToken = { TokenType::end };
		} while (ch != '\n' && isspace(ch));

		switch (ch)
		{
		case 0:
			return _currentToken = { TokenType::end }; // assign and return
		case ',':
			return _currentToken = { TokenType::comma }; // assign and return
		case '^':
		case '*':
		case '/':
		case '+':
		case '-':
			//	case '~':		// actually, we don't need this here - we will use ~ only in the output token string
		case '(':
		case ')':
		case '=':
			return _currentToken = { static_cast<TokenType>(ch) };
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '.':
			_inputStream->putback(ch);												// put the first digit (or .) back into the input stream
			*_inputStream >> _currentToken.numberValue;				// read number into ct
			_currentToken.tokenType = TokenType::number;
			return _currentToken;
		default:											// name, name =, function name or error
			if (isalpha(ch)) {
				_currentToken.stringValue = ch;
				while (_inputStream->get(ch) && (isalnum(ch) || ch == '_') )		// we can have _ in function and variable names
					_currentToken.stringValue += ch;
				_inputStream->putback(ch);

				auto iter = inFunctionNames.find(_currentToken.stringValue);
				if (iter != end(inFunctionNames))
					_currentToken.tokenType = TokenType::function;
				else
					_currentToken.tokenType = TokenType::name;

				return _currentToken;
			}
			//error("bad token");
			return _currentToken = { TokenType::end };
		}

	}

	Token&	current()		// most recently read token
	{
		return _currentToken;
	}


private:
	stringstream *_inputStream;
	Token	_currentToken;
};
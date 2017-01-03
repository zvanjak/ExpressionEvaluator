#include "Tokenizer.h"

#include <iostream>
#include <vector>

using std::vector;
using namespace std;

bool isTokenOperator(Token &t)
{
	return  t.tokenType == TokenType::plus ||
		t.tokenType == TokenType::minus ||
		t.tokenType == TokenType::mul ||
		t.tokenType == TokenType::div;
}

Tokenizer::~Tokenizer()
{
}

Token& Tokenizer::current()
{
	return _currentToken;
}

Token Tokenizer::getNext(vector<string> inFunctionNames)
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
	case '*':
	case '/':
	case '+':
	case '-':
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
			while (_inputStream->get(ch) && isalnum(ch))
				_currentToken.stringValue += ch;
			_inputStream->putback(ch);

			auto func = find(begin(inFunctionNames), end(inFunctionNames), _currentToken.stringValue);
			if (func != end(inFunctionNames))
				_currentToken.tokenType = TokenType::function;
			else
				_currentToken.tokenType = TokenType::name;

			return _currentToken;
		}
		//error("bad token");
		return _currentToken = { TokenType::end };
	}

}

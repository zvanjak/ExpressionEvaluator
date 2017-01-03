#pragma once

#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::stringstream;
using std::vector;

enum class TokenType : char
{
	name, number, function, end,
	plus = '+',
	minus = '-',
	mul = '*',
	div = '/',
	left = '(',
	right = ')',
	assign = '='
};

struct Token
{
	TokenType tokenType;
	string		stringValue;
	double		numberValue;
};

bool isTokenOperator(Token &t);

class Tokenizer
{
public:
	Tokenizer(stringstream& s) : _inputStream{ &s } { _currentToken.tokenType = TokenType::end; }
	~Tokenizer();

	Token		getNext(vector<string> inFunctionNames);		// read and return next token
	Token&	current();		// most recently read token

private:
	stringstream *_inputStream;
	Token		_currentToken;
};
#pragma once
#include <string>
#include <vector>
#include <list>
#include <map>

#include "Tokenizer.h"

using std::string;
using std::vector;
using std::map;

struct Operator
{
	char	_name;
	int		_priority;
	bool	_isRightAssociative;
};

enum class CalculatorStatus
{
	STATUS_OK,
	SYNTAX_ERROR,
	MISMATCHED_PARENTHESIS,
	ERROR_IN_CALCULATION,
	NO_EQUAL_SIGN_IN_EQUATION,
	NO_VAR_IN_EQUATION,
	INIFINITY_VARIABLE_VALUE
};

class ExpressionEvaluator
{
public:
	ExpressionEvaluator();
	~ExpressionEvaluator();

	string			driver(string inputExpression);

	vector<Token>	tokenize(string inExpr);
	double			evaluateExpression(vector<Token> &vecTokens, CalculatorStatus *outStatus);
	vector<Token>	transformToRPN(vector<Token> &vecTokens, CalculatorStatus *outStatus);
	double			evaluateRPN(vector<Token> output, CalculatorStatus *outStatus);

	string			getErrorMessage(CalculatorStatus inStatus);

	double			evaluate(string inputExpression, CalculatorStatus *outStatus);

private:
	vector<string>		_definedFunctions;		// { "log", "sin", "cos" };
	vector<Operator>	_definedOperators;
	map<CalculatorStatus, string>		_errorMessages;

	void		initializeCalculator();

	void		checkTransition(TokenType from, TokenType to, CalculatorStatus *outStatus);

	bool		isFunctionName(string s);
	char		getOperatorChar(Token t);
	Operator&	getOperator(Token t);
};


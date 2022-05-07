#pragma once
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

#include "Tokenizer.h"
#include <stack>

using std::string;
using std::vector;
using std::map;
using std::stack;

struct Operator
{
	char	_name;
	int		_priority;
	bool	_isUnary;
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
	ExpressionEvaluator()
	{
		initializeCalculator();
	}

	bool addUserDefinedFunction(string inName, DefinedFunction* inFunc)
	{
		// TODO - check for function with that name already present
		_defFunc[inName] = inFunc;

		return true;
	}

	string	driver(string inputExpr)
	{
		string		result;
		vector<Token> tokens;
		CalculatorStatus	outStatus = CalculatorStatus::STATUS_OK;

		tokens = tokenize(inputExpr);

		double res = evaluateExpression(tokens, &outStatus);

		if (outStatus == CalculatorStatus::STATUS_OK)
			result = std::to_string(res);
		else
			result = _errorMessages[outStatus];

		return result;
	}

	vector<Token> tokenize(string inExpr)
	{
		vector<Token> vecTokens;
		stringstream s(inExpr);
		Tokenizer tokenizer(s);

		Token t;
		do
		{
			t = tokenizer.getNext(_defFunc);
			vecTokens.push_back(t);
		} while (t.tokenType != TokenType::end);

		return vecTokens;
	}

	double	evaluateExpression(vector<Token>& vecTokens, CalculatorStatus* outStatus)
	{
		vector<Token> output = transformToRPN(vecTokens, outStatus);

		if (*outStatus == CalculatorStatus::STATUS_OK)
			return evaluateRPN(output, outStatus);
		else
			return 0.0;
	}

	vector<Token>	transformToRPN(vector<Token> &vecTokens, CalculatorStatus *outStatus);
	double			evaluateRPN(vector<Token> output, CalculatorStatus *outStatus);

	string			getErrorMessage(CalculatorStatus inStatus);

	double			evaluate(string inputExpression, CalculatorStatus *outStatus);

private:
	// TODO - što s funkcijama koje imaju više parametara?
	std::unordered_map<string, DefinedFunction *> _defFunc;

	vector<Operator>	_definedOperators;

	map<CalculatorStatus, string>		_errorMessages;

	void	initializeCalculator()
	{
		_defFunc["sin"] = new DefinedFunctionOneParam(sin);
		_defFunc["cos"] = new DefinedFunctionOneParam(cos);
		_defFunc["log"] = new DefinedFunctionOneParam(log);

		_definedOperators.push_back({ '+', 1, false, false });
		_definedOperators.push_back({ '-', 1, false, false });
		_definedOperators.push_back({ '~', 1, true,  false });
		_definedOperators.push_back({ '*', 2, false, false });
		_definedOperators.push_back({ '/', 2, false, false });
		_definedOperators.push_back({ '^', 3, false, true });

		_errorMessages[CalculatorStatus::STATUS_OK] = "OK";
		_errorMessages[CalculatorStatus::SYNTAX_ERROR] = "Syntax error";
		_errorMessages[CalculatorStatus::MISMATCHED_PARENTHESIS] = "Mismatched parenthesis";
		_errorMessages[CalculatorStatus::ERROR_IN_CALCULATION] = "Error during calculation";
		_errorMessages[CalculatorStatus::NO_EQUAL_SIGN_IN_EQUATION] = "Missing equal sign in equation";
		_errorMessages[CalculatorStatus::NO_VAR_IN_EQUATION] = "No variable in equation";
		_errorMessages[CalculatorStatus::INIFINITY_VARIABLE_VALUE] = "Infinite variable value";
	}

	void		checkTransition(TokenType from, TokenType to, CalculatorStatus *outStatus);

	bool		isFunctionName(string s);
	char		getOperatorChar(Token t);
	Operator&	getOperator(Token t);
};


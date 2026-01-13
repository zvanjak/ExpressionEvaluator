#pragma once
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <unordered_map>
#include <cmath>
#include <algorithm>

#include "Tokenizer.h"


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
	INFINITY_VARIABLE_VALUE,
	UNKNOWN_FUNCTION,
	INSUFFICIENT_OPERAND,
	DIVISION_BY_ZERO,
	DOMAIN_ERROR
};


class ExpressionEvaluator
{
private:
	// TODO - što s funkcijama koje imaju više parametara?
	std::unordered_map<string, DefinedFunction*> _defFunc;

	vector<Operator>	_definedOperators;

	map<CalculatorStatus, string>		_errorMessages;

public:
	ExpressionEvaluator()
	{
		initializeCalculator();
	}

	bool    addUserDefinedFunction(string inName, DefinedFunction* inFunc)
	{
		// TODO - check for function with that name already present
		_defFunc[inName] = inFunc;

		return true;
	}

	string	getErrorMessage(CalculatorStatus inStatus)
	{
		return _errorMessages[inStatus];
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

	double	evaluate(string inputExpr, string &outErrorMsg)
	{
		string		result;
		vector<Token> tokens;
		CalculatorStatus	outStatus = CalculatorStatus::STATUS_OK;

		tokens = tokenize(inputExpr);

		double res = evaluateExpression(tokens, &outStatus);

		if (outStatus != CalculatorStatus::STATUS_OK)
			outErrorMsg = _errorMessages[outStatus];

		return res;
	}

	double	evaluateExpression(vector<Token>& vecTokens, CalculatorStatus* outStatus)
	{
		vector<Token> output = transformToRPN(vecTokens, outStatus);

		if (*outStatus == CalculatorStatus::STATUS_OK)
			return evaluateRPN(output, outStatus);
		else
			return 0.0;
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

	///////////////////////////////////////////				RPN		  	/////////////////////////////////////////////////////////
	// Implementation of Shunting-Yard algorithm (http://en.wikipedia.org/wiki/Shunting-yard_algorithm) 
	vector<Token>	transformToRPN(vector<Token>& vecTokens, CalculatorStatus* outStatus)
	{
		vector<Token> output;
		stack<Token> stack;
		TokenType	lastElem = TokenType::end;

		*outStatus = CalculatorStatus::STATUS_OK;

		Token t;
		auto iter = begin(vecTokens);
		do
		{
			if (*outStatus != CalculatorStatus::STATUS_OK)
				return output;

			t = *iter;																			//Read a token.

			if (t.tokenType == TokenType::number) {												//If the token is a number, then add it to the output queue.
				checkTransition(lastElem, TokenType::number, outStatus);
				lastElem = TokenType::number;

				output.push_back(t);
			}
			else if (t.tokenType == TokenType::comma)											//If the token is a comma ...
			{
				checkTransition(lastElem, TokenType::comma, outStatus);
				lastElem = TokenType::comma;

				// do nothing :)
				//  Until the token at the top of the stack is a left parenthesis, pop operators off the stack onto the output queue.
				bool foundLeftParenth = false;
				if (stack.size() == 0)
				{
					*outStatus = CalculatorStatus::SYNTAX_ERROR;
					return output;
				}
				Token topStackToken = stack.top();
				while (Tokenizer::isTokenOperator(topStackToken) || topStackToken.tokenType == TokenType::function && topStackToken.tokenType != TokenType::left)
				{
					// TODO - write this cleaner!
					stack.pop();

					if (topStackToken.tokenType != TokenType::left)
						output.push_back(topStackToken);
					else
						foundLeftParenth = true;

					if (stack.size() > 0)
						topStackToken = stack.top();
					else
						break;
				}                
			}
			else if (t.tokenType == TokenType::name)											//If the token is a function token, then push it onto the stack.
			{
				checkTransition(lastElem, TokenType::name, outStatus);
				lastElem = TokenType::name;

				output.push_back(t);
			}
			else if (t.tokenType == TokenType::function)										//If the token is a function token, then push it onto the stack.
			{
				checkTransition(lastElem, TokenType::function, outStatus);
				lastElem = TokenType::function;

				stack.push(t);
			}
			else if (Tokenizer::isTokenOperator(t))														//If the token is an operator, o1, then :
			{
				checkTransition(lastElem, t.tokenType, outStatus);

				// check for unary minus
				if (t.tokenType == TokenType::minus &&
					(lastElem == TokenType::end || lastElem == TokenType::left || lastElem == TokenType::comma))
				{
					// we have unary minus	
					t.tokenType = TokenType::unary_minus;
				}

				lastElem = t.tokenType;

				Operator& o1 = getOperator(t);
				if (stack.size() > 0)
				{

					//	while there is an operator token, o2, at the top of the operator stack, and either
					Token topStackToken = stack.top();
					while (Tokenizer::isTokenOperator(topStackToken))
					{
						Operator& o2 = getOperator(topStackToken);

						//		o1 is left - associative and its precedence is less than or equal to that of o2, or
						//		o1 is right associative, and has precedence less than that of o2,
						if ((o1._isRightAssociative == false && o1._priority <= o2._priority) ||
							(o1._isRightAssociative == true && o1._priority < o2._priority))
						{
							//	then pop o2 off the operator stack, onto the output queue;
							stack.pop();
							output.push_back(topStackToken);

							if (stack.size() > 0)
								topStackToken = stack.top();
							else
								break;
						}
						else
							break;
					}
				}

				//	push o1 onto the operator stack.
				stack.push(t);
			}
			else if (t.tokenType == TokenType::left)											//If the token is a left parenthesis, then push it onto the stack.
			{
				checkTransition(lastElem, TokenType::left, outStatus);
				lastElem = TokenType::left;

				stack.push(t);
			}
			else if (t.tokenType == TokenType::right)											//If the token is a right parenthesis :
			{
				checkTransition(lastElem, TokenType::right, outStatus);
				lastElem = TokenType::right;

				//  Until the token at the top of the stack is a left parenthesis, pop operators off the stack onto the output queue.
				//	Pop the left parenthesis from the stack, but not onto the output queue.
				//	If the token at the top of the stack is a function token, pop it onto the output queue.
				//	If the stack runs out without finding a left parenthesis, then there are mismatched parentheses.
				bool foundLeftParenth = false;
                if (stack.size() == 0)
				{
					*outStatus = CalculatorStatus::MISMATCHED_PARENTHESIS;
					return output;
				}
				Token topStackToken = stack.top();
				while (Tokenizer::isTokenOperator(topStackToken) || topStackToken.tokenType == TokenType::function && topStackToken.tokenType != TokenType::left)
				{
					// TODO - write this cleaner!
					stack.pop();

					if (topStackToken.tokenType != TokenType::left)
						output.push_back(topStackToken);
					else
						foundLeftParenth = true;

					if (stack.size() > 0)
						topStackToken = stack.top();
					else
						break;
				}

				// if everything is OK, left parenthesis should be on top of stack 
				if (topStackToken.tokenType == TokenType::left)
				{
					stack.pop();
					foundLeftParenth = true;

					// now we have to check if function is on the top of the stack
					if (stack.size() > 0)
					{
						topStackToken = stack.top();
						if (topStackToken.tokenType == TokenType::function)
						{
							stack.pop();
							output.push_back(topStackToken);
						}
					}
				}

				if (foundLeftParenth == false)
				{
					*outStatus = CalculatorStatus::MISMATCHED_PARENTHESIS;
					return output;
				}
			}

			++iter;
		} while (t.tokenType != TokenType::end);

		//When there are no more tokens to read :
		//	While there are still operator tokens in the stack :
		while (!stack.empty())
		{
			//		If the operator token on the top of the stack is a parenthesis, then there are mismatched parentheses.
			//		Pop the operator onto the output queue.
			Token oper = stack.top();

			if (oper.tokenType == TokenType::left || oper.tokenType == TokenType::right)
				*outStatus = CalculatorStatus::MISMATCHED_PARENTHESIS;

			stack.pop();
			output.push_back(oper);
		};

		Token endToken{ TokenType::end };
		output.push_back(endToken);

		return output;
	}
	
	double evaluateRPN(vector<Token> output, CalculatorStatus* outStatus)
	{
		std::stack<Token>	evalStack;
		auto iter2 = begin(output);
		while (iter2->tokenType != TokenType::end)
		{
			Token t = *iter2;

			if (t.tokenType == TokenType::number || t.tokenType == TokenType::name)
				evalStack.push(t);
			else if (t.tokenType == TokenType::comma)
			{
				// do nothing :)
			}
			else if (Tokenizer::isTokenOperator(t))
			{
				Token res;

				// if it is a unary operator
				if (t.tokenType == TokenType::unary_minus)
				{
					if (evalStack.size() < 1)
					{
						*outStatus = CalculatorStatus::INSUFFICIENT_OPERAND;
						return 0.0;
					}

					// take operand from the top of the stack
					Token operand = evalStack.top(); evalStack.pop();

					res.numberValue = -operand.numberValue;
					res.tokenType = TokenType::number;
					if (isInvalidResult(res.numberValue))
					{
						*outStatus = CalculatorStatus::DOMAIN_ERROR;
						return 0.0;
					}
					evalStack.push(res);
				}
				else
				{
					// take two element from top of the evaluation stack and perform operation
					if (evalStack.size() < 2)
					{
						*outStatus = CalculatorStatus::INSUFFICIENT_OPERAND;
						return 0.0;
					}

					Token oper1 = evalStack.top(); evalStack.pop();
					Token oper2 = evalStack.top(); evalStack.pop();

					switch (t.tokenType)
					{
					case TokenType::plus:
						res.numberValue = oper1.numberValue + oper2.numberValue;
						break;
					case TokenType::minus:
						res.numberValue = oper2.numberValue - oper1.numberValue;
						break;
					case TokenType::mul:
						res.numberValue = oper1.numberValue * oper2.numberValue;
						break;
					case TokenType::div:
						if (oper1.numberValue == 0.0)
						{
							*outStatus = CalculatorStatus::DIVISION_BY_ZERO;
							return 0.0;
						}
						res.numberValue = oper2.numberValue / oper1.numberValue;
						break;
					case TokenType::pow:
						res.numberValue = pow(oper2.numberValue, oper1.numberValue);
						break;
					}

					res.tokenType = TokenType::number;
					if (isInvalidResult(res.numberValue))
					{
						*outStatus = CalculatorStatus::DOMAIN_ERROR;
						return 0.0;
					}
					evalStack.push(res);
				}
			}
			else if (t.tokenType == TokenType::function)
			{
				auto iter = _defFunc.find(t.stringValue);
				if (iter == end(_defFunc))
				{
					*outStatus = CalculatorStatus::UNKNOWN_FUNCTION;
					return 0.0;
				}

				Token res;
				switch (iter->second->_numParam)
				{
				case 1:
				{
					if (evalStack.size() < 1)
					{
						*outStatus = CalculatorStatus::INSUFFICIENT_OPERAND;
						return 0.0;
					}
					Token oper1 = evalStack.top(); evalStack.pop();
					DefinedFunctionOneParam* func = dynamic_cast<DefinedFunctionOneParam*> (iter->second);
					res.numberValue = func->_ptrFunc(oper1.numberValue);
					break;
				}
				case 2:
				{
					if (evalStack.size() < 2)
					{
						*outStatus = CalculatorStatus::INSUFFICIENT_OPERAND;
						return 0.0;
					}
					Token oper1 = evalStack.top(); evalStack.pop();
					Token oper2 = evalStack.top(); evalStack.pop();

					DefinedFunctionTwoParam* func = dynamic_cast<DefinedFunctionTwoParam*> (iter->second);
					// we are actually getting operands in reverse order
					res.numberValue = func->_ptrFunc(oper2.numberValue, oper1.numberValue);
					break;
				}
				case 3:
					*outStatus = CalculatorStatus::SYNTAX_ERROR;
					return 0.0;
				default:
					*outStatus = CalculatorStatus::ERROR_IN_CALCULATION;
					return 0.0;
				}
				res.tokenType = TokenType::number;
				if (isInvalidResult(res.numberValue))
				{
					*outStatus = CalculatorStatus::DOMAIN_ERROR;
					return 0.0;
				}
				evalStack.push(res);
			}

			++iter2;
		}

		// final result should be on top of the evalStack
		if (evalStack.size() == 1 && evalStack.top().tokenType == TokenType::number)
		{
			Token finalRes = evalStack.top();

			return finalRes.numberValue;
		}
		else if (*outStatus == CalculatorStatus::STATUS_OK)
			*outStatus = CalculatorStatus::ERROR_IN_CALCULATION;

		return 0.0;
	}
	
	// Function used during development of simple expression evaluator. Surpased by "driver" function (left in code because of a lot of tests that use its interface)
	double evaluate(string inputExpr, CalculatorStatus* outStatus)
	{
		vector<Token> tokens;

		tokens = tokenize(inputExpr);

		return evaluateExpression(tokens, outStatus);
	}

private:
	void	initializeCalculator()
	{
		_defFunc["sqrt"] = new DefinedFunctionOneParam(sqrt);
		_defFunc["cbrt"] = new DefinedFunctionOneParam(cbrt);
		_defFunc["log"] = new DefinedFunctionOneParam(log);
		_defFunc["exp"] = new DefinedFunctionOneParam(exp);
		_defFunc["log10"] = new DefinedFunctionOneParam(log10);

		_defFunc["sin"] = new DefinedFunctionOneParam(sin);
		_defFunc["cos"] = new DefinedFunctionOneParam(cos);
		_defFunc["tan"] = new DefinedFunctionOneParam(tan);
		_defFunc["asin"] = new DefinedFunctionOneParam(asin);
		_defFunc["acos"] = new DefinedFunctionOneParam(acos);
		_defFunc["atan"] = new DefinedFunctionOneParam(atan);

		_defFunc["sinh"] = new DefinedFunctionOneParam(sinh);
		_defFunc["cosh"] = new DefinedFunctionOneParam(cosh);
		_defFunc["tanh"] = new DefinedFunctionOneParam(tanh);
		_defFunc["asinh"] = new DefinedFunctionOneParam(asinh);
		_defFunc["acosh"] = new DefinedFunctionOneParam(acosh);
		_defFunc["atanh"] = new DefinedFunctionOneParam(atanh);

		_defFunc["erf"] = new DefinedFunctionOneParam(erf);
		_defFunc["erfc"] = new DefinedFunctionOneParam(erfc);
		_defFunc["tgamma"] = new DefinedFunctionOneParam(tgamma);
		_defFunc["lgamma"] = new DefinedFunctionOneParam(lgamma);

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
		_errorMessages[CalculatorStatus::INFINITY_VARIABLE_VALUE] = "Infinite variable value";
		_errorMessages[CalculatorStatus::UNKNOWN_FUNCTION] = "Unknown function";
		_errorMessages[CalculatorStatus::INSUFFICIENT_OPERAND] = "Insufficient operand";
		_errorMessages[CalculatorStatus::DIVISION_BY_ZERO] = "Division by zero";
		_errorMessages[CalculatorStatus::DOMAIN_ERROR] = "Domain error";
	}

	bool	isFunctionName(string s)
	{
		auto iter = _defFunc.find(s);
		if (iter != end(_defFunc))
			return true;
		else
			return false;
	}

	char	getOperatorChar(Token t)
	{
		switch (t.tokenType)
		{
		case TokenType::plus: return '+';
		case TokenType::minus: return '-';
		case TokenType::unary_minus: return '~';
		case TokenType::mul: return '*';
		case TokenType::div: return '/';
		case TokenType::pow: return '^';
		default: return 0;
		}
	}

	Operator& getOperator(Token t)
	{
		char operName = getOperatorChar(t);
		auto findOper = std::find_if(begin(_definedOperators), end(_definedOperators), [operName](Operator& o) { return o._name == operName; });

		return *findOper;
	}

	bool isInvalidResult(double value)
	{
		return std::isnan(value) || std::isinf(value);
	}

	void	checkTransition(TokenType from, TokenType to, CalculatorStatus* outStatus)
	{
		if (from == TokenType::end && (to == TokenType::number || to == TokenType::name || to == TokenType::function || to == TokenType::left || to == TokenType::minus))		*outStatus = CalculatorStatus::STATUS_OK;
		else if (from == TokenType::number && (to == TokenType::comma || to == TokenType::plus || to == TokenType::minus || to == TokenType::mul || to == TokenType::div || to == TokenType::pow || to == TokenType::right || to == TokenType::end))		*outStatus = CalculatorStatus::STATUS_OK;
		else if (from == TokenType::name && (to == TokenType::plus || to == TokenType::minus || to == TokenType::mul || to == TokenType::div || to == TokenType::pow || to == TokenType::right || to == TokenType::end))		*outStatus = CalculatorStatus::STATUS_OK;
		else if (from == TokenType::function && (to == TokenType::left))		*outStatus = CalculatorStatus::STATUS_OK;
		else if (from == TokenType::comma && (to == TokenType::number || to == TokenType::minus || to == TokenType::function || to == TokenType::left))		*outStatus = CalculatorStatus::STATUS_OK;
		else if (from == TokenType::left && (to == TokenType::number || to == TokenType::left || to == TokenType::name || to == TokenType::function || to == TokenType::minus))		*outStatus = CalculatorStatus::STATUS_OK;
		else if (from == TokenType::right && (to == TokenType::comma || to == TokenType::plus || to == TokenType::minus || to == TokenType::mul || to == TokenType::div || to == TokenType::pow || to == TokenType::right || to == TokenType::end))		*outStatus = CalculatorStatus::STATUS_OK;
		else if (from == TokenType::plus && (to == TokenType::name || to == TokenType::number || to == TokenType::function || to == TokenType::left))		*outStatus = CalculatorStatus::STATUS_OK;
		else if (from == TokenType::minus && (to == TokenType::name || to == TokenType::number || to == TokenType::function || to == TokenType::left))		*outStatus = CalculatorStatus::STATUS_OK;
		else if (from == TokenType::unary_minus && (to == TokenType::name || to == TokenType::number || to == TokenType::function || to == TokenType::left))		*outStatus = CalculatorStatus::STATUS_OK;
		else if (from == TokenType::mul && (to == TokenType::name || to == TokenType::number || to == TokenType::function || to == TokenType::left))		*outStatus = CalculatorStatus::STATUS_OK;
		else if (from == TokenType::div && (to == TokenType::name || to == TokenType::number || to == TokenType::function || to == TokenType::left))		*outStatus = CalculatorStatus::STATUS_OK;
		else if (from == TokenType::pow && (to == TokenType::name || to == TokenType::number || to == TokenType::function || to == TokenType::left))		*outStatus = CalculatorStatus::STATUS_OK;
		else
			*outStatus = CalculatorStatus::SYNTAX_ERROR;
	}

};


#include "ExpressionEvaluator.h"

#include <algorithm>
#include <vector>
#include <stack>

using std::vector;
using std::stack;

ExpressionEvaluator::ExpressionEvaluator()
{
	initializeCalculator();
}

ExpressionEvaluator::~ExpressionEvaluator()
{
}

void	ExpressionEvaluator::initializeCalculator()
{
	_definedFunctions.push_back("log");
	_definedFunctions.push_back("sin");
	_definedFunctions.push_back("cos");

	_definedOperators.push_back({ '+', 1, false });
	_definedOperators.push_back({ '-', 1, false });
	_definedOperators.push_back({ '*', 2, false });
	_definedOperators.push_back({ '/', 2, false });

	_errorMessages[CalculatorStatus::STATUS_OK] = "OK";
	_errorMessages[CalculatorStatus::SYNTAX_ERROR] = "Syntax error";
	_errorMessages[CalculatorStatus::MISMATCHED_PARENTHESIS] = "Mismatched parenthesis";
	_errorMessages[CalculatorStatus::ERROR_IN_CALCULATION] = "Error during calculation";
	_errorMessages[CalculatorStatus::NO_EQUAL_SIGN_IN_EQUATION] = "Missing equal sign in equation";
	_errorMessages[CalculatorStatus::NO_VAR_IN_EQUATION] = "No variable in equation";
	_errorMessages[CalculatorStatus::INIFINITY_VARIABLE_VALUE] = "Infinite variable value";
}

string	ExpressionEvaluator::driver(string inputExpr)
{
	string		result;
	vector<Token> tokens;
	CalculatorStatus	outStatus = CalculatorStatus::STATUS_OK;

	tokens = tokenize(inputExpr);

	if (isSimpleExpression(tokens))
	{
		double res = evaluateExpression(tokens, &outStatus);

		if (outStatus == CalculatorStatus::STATUS_OK)
			result = std::to_string(res);
		else
			result = _errorMessages[outStatus];
	}
	else
	{
		string varName;
		double varValue = solveLinearEquation(tokens, varName, &outStatus);

		if (outStatus == CalculatorStatus::STATUS_OK)
			result = varName + "=" + std::to_string(varValue);
		else
			result = _errorMessages[outStatus];
	}

	return result;
}

vector<Token> ExpressionEvaluator::tokenize(string inExpr)
{
	vector<Token> vecTokens;
	stringstream s(inExpr);
	Tokenizer tokenizer(s);

	Token t;
	do
	{
		t = tokenizer.getNext(_definedFunctions);
		vecTokens.push_back(t);
	} while (t.tokenType != TokenType::end);

	return vecTokens;
}

double	ExpressionEvaluator::evaluateExpression(vector<Token> &vecTokens, CalculatorStatus *outStatus)
{
	vector<Token> output = transformToRPN(vecTokens, outStatus);

	if (*outStatus == CalculatorStatus::STATUS_OK)
		return evaluateRPN(output, outStatus);
	else
		return 0.0;
}

// Implementation of Shunting-Yard algorithm (http://en.wikipedia.org/wiki/Shunting-yard_algorithm) 
vector<Token>	ExpressionEvaluator::transformToRPN(vector<Token> &vecTokens, CalculatorStatus *outStatus)
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

		t = *iter;																													//Read a token.

		if (t.tokenType == TokenType::number) {															//If the token is a number, then add it to the output queue.
			checkTransition(lastElem, TokenType::number, outStatus);
			lastElem = TokenType::number;

			output.push_back(t);
		}
		else if (t.tokenType == TokenType::name)														//If the token is a function token, then push it onto the stack.
		{
			checkTransition(lastElem, TokenType::name, outStatus);
			lastElem = TokenType::name;

			output.push_back(t);
		}
		else if (t.tokenType == TokenType::function)												//If the token is a function token, then push it onto the stack.
		{
			checkTransition(lastElem, TokenType::function, outStatus);
			lastElem = TokenType::function;

			stack.push(t);
		}
		else if (isTokenOperator(t))																				//If the token is an operator, o1, then :
		{
			checkTransition(lastElem, t.tokenType, outStatus);
			lastElem = t.tokenType;

			Operator& o1 = getOperator(t);
			if (stack.size() > 0)
			{

				//	while there is an operator token, o2, at the top of the operator stack, and either
				Token topStackToken = stack.top();
				while (isTokenOperator(topStackToken))
				{
					Operator &o2 = getOperator(topStackToken);

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
		else if (t.tokenType == TokenType::left)															//If the token is a left parenthesis, then push it onto the stack.
		{
			checkTransition(lastElem, TokenType::left, outStatus);
			lastElem = TokenType::left;

			stack.push(t);
		}
		else if (t.tokenType == TokenType::right)															//If the token is a right parenthesis :
		{
			checkTransition(lastElem, TokenType::right, outStatus);
			lastElem = TokenType::right;

			//  Until the token at the top of the stack is a left parenthesis, pop operators off the stack onto the output queue.
			//	Pop the left parenthesis from the stack, but not onto the output queue.
			//	If the token at the top of the stack is a function token, pop it onto the output queue.
			//	If the stack runs out without finding a left parenthesis, then there are mismatched parentheses.
			bool foundLeftParenth = false;
			Token topStackToken = stack.top();
			while (isTokenOperator(topStackToken) || topStackToken.tokenType == TokenType::function && topStackToken.tokenType != TokenType::left)
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

double ExpressionEvaluator::evaluateRPN(vector<Token> output, CalculatorStatus *outStatus)
{
	std::stack<Token>	evalStack;
	auto iter2 = begin(output);
	while (iter2->tokenType != TokenType::end)
	{
		Token t = *iter2;

		if (t.tokenType == TokenType::number || t.tokenType == TokenType::name)
			evalStack.push(t);
		else if (isTokenOperator(t))
		{
			// take two element from top of the evaluation stack and perform operation
			Token oper1 = evalStack.top(); evalStack.pop();
			Token oper2 = evalStack.top(); evalStack.pop();
			Token res;

			switch (t.tokenType)
			{
			case TokenType::plus:
				res.numberValue = oper1.numberValue + oper2.numberValue;
				res.tokenType = TokenType::number;
				evalStack.push(res);
				break;
			case TokenType::minus:
				res.numberValue = oper2.numberValue - oper1.numberValue;
				res.tokenType = TokenType::number;
				evalStack.push(res);
				break;
			case TokenType::mul:
				res.numberValue = oper1.numberValue * oper2.numberValue;
				res.tokenType = TokenType::number;
				evalStack.push(res);
				break;
			case TokenType::div:
				res.numberValue = oper2.numberValue / oper1.numberValue;
				res.tokenType = TokenType::number;
				evalStack.push(res);
				break;
			}
		}
		else if (t.tokenType == TokenType::function)
		{
			Token oper1 = evalStack.top(); evalStack.pop();
			Token res;

			if (t.stringValue == "log") {
				res.numberValue = log(oper1.numberValue);
				res.tokenType = TokenType::number;
				evalStack.push(res);
			}
			else if (t.stringValue == "sin") {
				res.numberValue = sin(oper1.numberValue);
				res.tokenType = TokenType::number;
				evalStack.push(res);
			}
			else if (t.stringValue == "cos") {
				res.numberValue = cos(oper1.numberValue);
				res.tokenType = TokenType::number;
				evalStack.push(res);
			}
		}

		iter2++;
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

double	ExpressionEvaluator::solveLinearEquation(vector<Token> &vecTokens, string &outVarName, CalculatorStatus *outStatus)
{
	//CalculatorStatus status = CalculatorStatus::STATUS_OK;

	// first, we'll find a variable name
	auto varName = find_if(begin(vecTokens), end(vecTokens), [](Token &t) { return t.tokenType == TokenType::name; });

	if (varName == end(vecTokens))
	{
		*outStatus = CalculatorStatus::NO_VAR_IN_EQUATION;
		return 0.0;
	}
	outVarName = (*varName).stringValue;

	// find assing (=) sign and split token array in two parts, left and right
	auto assignSign = find_if(begin(vecTokens), end(vecTokens), [](Token &t) { return t.tokenType == TokenType::assign; });

	if (assignSign != end(vecTokens))
	{
		// splitting left and right side (and adding TokenType::end to each expression)
		vector<Token> leftSide(begin(vecTokens), assignSign);
		vector<Token> rightSide(assignSign + 1, end(vecTokens) - 1);

		leftSide.push_back(Token{ TokenType::end });
		rightSide.push_back(Token{ TokenType::end });

		// FIRST - we have to change all terms of the form [name] * [number] to form [number] * [name]
		reorderVariableMultiplication(leftSide);
		reorderVariableMultiplication(rightSide);

		// now we need to transform them to RPN
		vector<Token> leftRPN = transformToRPN(leftSide, outStatus);
		vector<Token> rightRPN = transformToRPN(rightSide, outStatus);

		// and then simplify expressions 
		vector<Token> leftSimple = simplifyExpression(leftRPN, outStatus);
		vector<Token> rightSimple = simplifyExpression(rightRPN, outStatus);

		// Now we expect to have two simple expressions. Since the requirement of the task is that there is only addition and multiplication in this equations, 
		// we expect to have a list of terms of the form [number, mul, name] plus/minus [number, mul, name] plus/minus [number]

		double leftVarCoef = 0;
		double leftNum = 0;
		double rightVarCoef = 0;
		double rightNum = 0;

		sumNameCoefficients(leftSimple, &leftVarCoef, &leftNum);
		sumNameCoefficients(rightSimple, &rightVarCoef, &rightNum);

		if (leftVarCoef - rightVarCoef == 0.0)
		{
			*outStatus = CalculatorStatus::INIFINITY_VARIABLE_VALUE;
			return 0.0;
		}

		return (rightNum - leftNum) / (leftVarCoef - rightVarCoef);
	}
	else		// ERROR - no = sign?
		*outStatus = CalculatorStatus::NO_EQUAL_SIGN_IN_EQUATION;

	return 0.0;
}

vector<Token> ExpressionEvaluator::simplifyExpression(vector<Token> output, CalculatorStatus *outStatus)
{
	std::stack<Token>	evalStack;
	auto iter2 = begin(output);
	while (iter2->tokenType != TokenType::end)
	{
		Token t = *iter2;

		if (t.tokenType == TokenType::number || t.tokenType == TokenType::name)
		{
			if (t.tokenType == TokenType::name)
				t.numberValue = 1.0;

			evalStack.push(t);
		}
		else if (isTokenOperator(t))
		{
			// take two element from top of the evaluation stack and perform operation
			Token oper1 = evalStack.top(); evalStack.pop();
			Token oper2 = evalStack.top(); evalStack.pop();
			Token res;

			// if we have undefined variables, we are just going to push them back to the stack
			if (oper1.tokenType == TokenType::name || oper2.tokenType == TokenType::name)
			{
				// BUT, if we have a multiplication operation ([number] * [name]), we are going to "perform" it virtually - ie. we'll write multiplication coefficient inside token representing variable
				if (t.tokenType == TokenType::mul)
				{
					res.tokenType = TokenType::name;
					if (oper1.tokenType == TokenType::name)
						res.numberValue = oper2.numberValue;
					else
						res.numberValue = oper1.numberValue;
					evalStack.push(res);
				}
				else
				{
					evalStack.push(oper2);
					evalStack.push(t);
					evalStack.push(oper1);
				}
			}
			else
			{
				switch (t.tokenType)
				{
				case TokenType::plus:
					res.numberValue = oper1.numberValue + oper2.numberValue;
					res.tokenType = TokenType::number;
					evalStack.push(res);
					break;
				case TokenType::minus:
					res.numberValue = oper2.numberValue - oper1.numberValue;
					res.tokenType = TokenType::number;
					evalStack.push(res);
					break;
				case TokenType::mul:
					res.numberValue = oper1.numberValue * oper2.numberValue;
					res.tokenType = TokenType::number;
					evalStack.push(res);
					break;
				case TokenType::div:
					res.numberValue = oper2.numberValue / oper1.numberValue;
					res.tokenType = TokenType::number;
					evalStack.push(res);
					break;
				}
			}
		}
		else if (t.tokenType == TokenType::function)
		{
			Token oper1 = evalStack.top(); evalStack.pop();
			Token res;

			if (t.stringValue == "log") {
				res.numberValue = log(oper1.numberValue);
				res.tokenType = TokenType::number;
				evalStack.push(res);
			}
			else if (t.stringValue == "sin") {
				res.numberValue = sin(oper1.numberValue);
				res.tokenType = TokenType::number;
				evalStack.push(res);
			}
			else if (t.stringValue == "cos") {
				res.numberValue = cos(oper1.numberValue);
				res.tokenType = TokenType::number;
				evalStack.push(res);
			}
		}

		iter2++;
	}

	vector<Token> vecOut;
	while (evalStack.size() > 0)
	{
		vecOut.insert(begin(vecOut), evalStack.top());			// reversing order
		evalStack.pop();
	}

	return vecOut;
}

void ExpressionEvaluator::reorderVariableMultiplication(vector<Token>& expr)
{
	for (int i = 0; i < expr.size(); i++)
	{
		Token& currToken = expr[i];

		if (currToken.tokenType == TokenType::name && i < expr.size() - 1)
		{
			Token& nextToken = expr[i + 1];
			if (nextToken.tokenType == TokenType::mul)
			{
				// switch positions
				Token s = expr[i];
				expr[i] = expr[i + 2];
				expr[i + 2] = s;
			}
		}
	}
}

void ExpressionEvaluator::sumNameCoefficients(vector<Token>& expr, double *outVarCoef, double *outRemainingNumberValue)
{
	*outVarCoef = 0.0;
	*outRemainingNumberValue = 0.0;

	for (int i = 0; i < expr.size(); i++)
	{
		Token& currToken = expr[i];

		if (currToken.tokenType == TokenType::name)
		{
			double coef = currToken.numberValue;
			if (i > 0)		// this means there is possibility there is a coefficient in front of name
			{
				Token& prevToken = expr[i - 1];
				if (prevToken.tokenType == TokenType::mul)			// we have a coefficient in front
				{
					coef *= expr[i - 2].numberValue;
					*outVarCoef += coef;
				}
				else if (prevToken.tokenType == TokenType::plus)			// it means we have a "+ x" expression
					*outVarCoef += coef;
				else if (prevToken.tokenType == TokenType::minus)			// it means we have a "- x" expression
					*outVarCoef -= coef;
			}
			else				// this means we have a standalone "x" at the beggining
				*outVarCoef += coef;
		}
		else if (currToken.tokenType == TokenType::number)
		{
			// lets see if this is a standalone number, or var coefficient
			if (i == expr.size() - 1)							// last element in expression, certainly stand alone number
			{
				Token& prevToken = expr[i - 1];

				if (prevToken.tokenType == TokenType::minus)
					*outRemainingNumberValue -= currToken.numberValue;
				else
					*outRemainingNumberValue += currToken.numberValue;
			}
			else if (i == 0)
			{
				Token& nextToken = expr[i + 1];

				if (nextToken.tokenType == TokenType::plus || nextToken.tokenType == TokenType::minus)
					*outRemainingNumberValue += currToken.numberValue;
			}
			else
			{
				Token& prevToken = expr[i - 1];
				Token& nextToken = expr[i + 1];

				if ((prevToken.tokenType == TokenType::plus || prevToken.tokenType == TokenType::minus) &&
					(nextToken.tokenType == TokenType::plus || nextToken.tokenType == TokenType::minus))
				{
					if (prevToken.tokenType == TokenType::minus)
						*outRemainingNumberValue -= currToken.numberValue;
					else
						*outRemainingNumberValue += currToken.numberValue;
				}
			}
		}
	}
}

string	ExpressionEvaluator::getErrorMessage(CalculatorStatus inStatus)
{
	return _errorMessages[inStatus];
}

// Function used during development of simple expression evaluator. Surpased by "driver" function (left in code because of a lot of tests that use its interface)
double ExpressionEvaluator::evaluate(string inputExpr, CalculatorStatus *outStatus)
{
	vector<Token> tokens;

	tokens = tokenize(inputExpr);

	if (isSimpleExpression(tokens))
		return evaluateExpression(tokens, outStatus);

	return 0.0;
}

bool	 ExpressionEvaluator::isFunctionName(string s)
{
	return std::find(begin(_definedFunctions), end(_definedFunctions), s) != end(_definedFunctions);
}

bool		ExpressionEvaluator::isSimpleExpression(vector<Token> &vecTokens)
{
	// pass through all tokens and see if there is assign (=) token in list
	auto foundAssign = std::find_if(begin(vecTokens), end(vecTokens), [](Token a)->bool {return a.tokenType == TokenType::assign; });

	// if we found =, then it is NOT a simple expression
	if (foundAssign != end(vecTokens))
		return false;

	// but, we have to check if there are variables in list of tokens, because that also means it is not a simple expression
	auto foundVar = std::find_if(begin(vecTokens), end(vecTokens), [](Token a)->bool {return a.tokenType == TokenType::name; });

	if (foundVar != end(vecTokens))
		return false;

	return true;
}

char ExpressionEvaluator::getOperatorChar(Token t)
{
	switch (t.tokenType)
	{
	case TokenType::plus: return '+';
	case TokenType::minus: return '-';
	case TokenType::mul: return '*';
	case TokenType::div: return '/';
	default: return 0;
	}
}

Operator&		ExpressionEvaluator::getOperator(Token t)
{
	char operName = getOperatorChar(t);
	auto findOper = std::find_if(begin(_definedOperators), end(_definedOperators), [operName](Operator &o) { return o._name == operName; });

	return *findOper;
}

void ExpressionEvaluator::checkTransition(TokenType from, TokenType to, CalculatorStatus *outStatus)
{
	if (from == TokenType::end && (to == TokenType::number || to == TokenType::name || to == TokenType::function || to == TokenType::left))
		*outStatus = CalculatorStatus::STATUS_OK;
	else if (from == TokenType::number && (to == TokenType::plus || to == TokenType::minus || to == TokenType::mul || to == TokenType::div || to == TokenType::right || to == TokenType::end))
		*outStatus = CalculatorStatus::STATUS_OK;
	else if (from == TokenType::name && (to == TokenType::plus || to == TokenType::minus || to == TokenType::mul || to == TokenType::div || to == TokenType::right || to == TokenType::end))
		*outStatus = CalculatorStatus::STATUS_OK;
	else if (from == TokenType::function && (to == TokenType::left))
		*outStatus = CalculatorStatus::STATUS_OK;
	else if (from == TokenType::left && (to == TokenType::number || to == TokenType::left || to == TokenType::name || to == TokenType::function))
		*outStatus = CalculatorStatus::STATUS_OK;
	else if (from == TokenType::right && (to == TokenType::plus || to == TokenType::minus || to == TokenType::mul || to == TokenType::div || to == TokenType::right || to == TokenType::end))
		*outStatus = CalculatorStatus::STATUS_OK;
	else if (from == TokenType::plus && (to == TokenType::name || to == TokenType::number || to == TokenType::function || to == TokenType::left))
		*outStatus = CalculatorStatus::STATUS_OK;
	else if (from == TokenType::minus && (to == TokenType::name || to == TokenType::number || to == TokenType::function || to == TokenType::left))
		*outStatus = CalculatorStatus::STATUS_OK;
	else if (from == TokenType::mul && (to == TokenType::name || to == TokenType::number || to == TokenType::function || to == TokenType::left))
		*outStatus = CalculatorStatus::STATUS_OK;
	else if (from == TokenType::div && (to == TokenType::name || to == TokenType::number || to == TokenType::function || to == TokenType::left))
		*outStatus = CalculatorStatus::STATUS_OK;
	else
		*outStatus = CalculatorStatus::SYNTAX_ERROR;
}


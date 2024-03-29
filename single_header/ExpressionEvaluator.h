#pragma once

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <stack>

using std::string;
using std::vector;
using std::stack;
using std::map;


enum class TokenType : char
{
	name, number, function, end,
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
	Tokenizer(std::stringstream& s) : _inputStream{ &s } { _currentToken.tokenType = TokenType::end; }
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

	Token getNext(std::unordered_map<string, double(*)(double)> inFunctionNames)
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
				while (_inputStream->get(ch) && (isalnum(ch) || ch == '_'))		// we can have _ in function and variable names
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
	std::stringstream *_inputStream;
	Token	_currentToken;
};

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
	typedef double(*FunctionOneParam)(double);


	 ExpressionEvaluator()
	{
		initializeCalculator();
	}

	 ~ExpressionEvaluator()
	{
	}

	void	 initializeCalculator()
	{
		_defFunc["sin"] = sin;
		_defFunc["cos"] = cos;
		_defFunc["log"] = log;

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

	bool  addUserDefinedFunction(string inName, FunctionOneParam inFunc)
	{
		// TODO - check for function with that name already present
		_defFunc[inName] = inFunc;

		return true;
	}

	string	 driver(string inputExpr)
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

	vector<Token>  tokenize(string inExpr)
	{
		vector<Token> vecTokens;
		std::stringstream s(inExpr);
		Tokenizer tokenizer(s);

		Token t;
		do
		{
			t = tokenizer.getNext(_defFunc);
			vecTokens.push_back(t);
		} while (t.tokenType != TokenType::end);

		return vecTokens;
	}

	double	 evaluateExpression(vector<Token> &vecTokens, CalculatorStatus *outStatus)
	{
		vector<Token> output = transformToRPN(vecTokens, outStatus);

		if (*outStatus == CalculatorStatus::STATUS_OK)
			return evaluateRPN(output, outStatus);
		else
			return 0.0;
	}

	// Implementation of Shunting-Yard algorithm (http://en.wikipedia.org/wiki/Shunting-yard_algorithm) 
	vector<Token>	 transformToRPN(vector<Token> &vecTokens, CalculatorStatus *outStatus)
	{
		vector<Token> output;
		std::stack<Token> stack;
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
					(lastElem == TokenType::end || lastElem == TokenType::left))
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

	double  evaluateRPN(vector<Token> output, CalculatorStatus *outStatus)
	{
		std::stack<Token>	evalStack;
		auto iter2 = begin(output);
		while (iter2->tokenType != TokenType::end)
		{
			Token t = *iter2;

			if (t.tokenType == TokenType::number || t.tokenType == TokenType::name)
				evalStack.push(t);
			else if (Tokenizer::isTokenOperator(t))
			{
				Token res;

				// if it is a unary operator
				if (t.tokenType == TokenType::unary_minus)
				{
					// take operand from the top of the stack
					Token operand = evalStack.top(); evalStack.pop();

					res.numberValue = -operand.numberValue;
					res.tokenType = TokenType::number;
					evalStack.push(res);
				}
				else
				{
					// take two element from top of the evaluation stack and perform operation
					if( evalStack.size() < 2 )
					{
						*outStatus = CalculatorStatus::SYNTAX_ERROR;
						return 0.0;
					}

					Token oper1 = evalStack.top(); evalStack.pop();
					Token oper2 = evalStack.top(); evalStack.pop();

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
					case TokenType::pow:
						res.numberValue = pow(oper2.numberValue, oper1.numberValue);
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

				auto iter = _defFunc.find(t.stringValue);
				if (iter != end(_defFunc))
				{
					res.numberValue = iter->second(oper1.numberValue);
					res.tokenType = TokenType::number;
					evalStack.push(res);
				}
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

	string	 getErrorMessage(CalculatorStatus inStatus)
	{
		return _errorMessages[inStatus];
	}

	// Function used during development of simple expression evaluator. Surpased by "driver" function (left in code because of a lot of tests that use its interface)
	double  evaluate(string inputExpr, CalculatorStatus *outStatus)
	{
		vector<Token> tokens;

		tokens = tokenize(inputExpr);

		return evaluateExpression(tokens, outStatus);
	}

	bool	  isFunctionName(string s)
	{
		auto iter = _defFunc.find(s);
		if (iter != end(_defFunc))
			return true;
		else
			return false;
	}

	char  getOperatorChar(Token t)
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

	Operator&		 getOperator(Token t)
	{
		char operName = getOperatorChar(t);
		auto findOper = std::find_if(begin(_definedOperators), end(_definedOperators), [operName](Operator &o) { return o._name == operName; });

		return *findOper;
	}

	void  checkTransition(TokenType from, TokenType to, CalculatorStatus *outStatus)
	{
		if (from == TokenType::end && (to == TokenType::number || to == TokenType::name || to == TokenType::function || to == TokenType::left || to == TokenType::minus))
			*outStatus = CalculatorStatus::STATUS_OK;
		else if (from == TokenType::number && (to == TokenType::plus || to == TokenType::minus || to == TokenType::mul || to == TokenType::div || to == TokenType::pow || to == TokenType::right || to == TokenType::end))
			*outStatus = CalculatorStatus::STATUS_OK;
		else if (from == TokenType::name && (to == TokenType::plus || to == TokenType::minus || to == TokenType::mul || to == TokenType::div || to == TokenType::pow || to == TokenType::right || to == TokenType::end))
			*outStatus = CalculatorStatus::STATUS_OK;
		else if (from == TokenType::function && (to == TokenType::left))
			*outStatus = CalculatorStatus::STATUS_OK;
		else if (from == TokenType::left && (to == TokenType::number || to == TokenType::left || to == TokenType::name || to == TokenType::function || to == TokenType::minus))
			*outStatus = CalculatorStatus::STATUS_OK;
		else if (from == TokenType::right && (to == TokenType::plus || to == TokenType::minus || to == TokenType::mul || to == TokenType::div || to == TokenType::pow || to == TokenType::right || to == TokenType::end))
			*outStatus = CalculatorStatus::STATUS_OK;
		else if (from == TokenType::plus && (to == TokenType::name || to == TokenType::number || to == TokenType::function || to == TokenType::left))
			*outStatus = CalculatorStatus::STATUS_OK;
		else if (from == TokenType::minus && (to == TokenType::name || to == TokenType::number || to == TokenType::function || to == TokenType::left))
			*outStatus = CalculatorStatus::STATUS_OK;
		else if (from == TokenType::unary_minus && (to == TokenType::name || to == TokenType::number || to == TokenType::function || to == TokenType::left))
			*outStatus = CalculatorStatus::STATUS_OK;
		else if (from == TokenType::mul && (to == TokenType::name || to == TokenType::number || to == TokenType::function || to == TokenType::left))
			*outStatus = CalculatorStatus::STATUS_OK;
		else if (from == TokenType::div && (to == TokenType::name || to == TokenType::number || to == TokenType::function || to == TokenType::left))
			*outStatus = CalculatorStatus::STATUS_OK;
		else if (from == TokenType::pow && (to == TokenType::name || to == TokenType::number || to == TokenType::function || to == TokenType::left))
			*outStatus = CalculatorStatus::STATUS_OK;
		else
			*outStatus = CalculatorStatus::SYNTAX_ERROR;
	}


private:
	// TODO - sto s funkcijama koje imaju vise parametara?
	std::unordered_map<string, FunctionOneParam> _defFunc;

	vector<Operator>	_definedOperators;

	map<CalculatorStatus, string>		_errorMessages;
};


#include <string>
#include <iostream>

#include "../ExpressionEvaluatorLib/ExpressionEvaluator.h"

using std::getline;
using std::string;

int _tmain(int argc, _TCHAR* argv[])
{
	string inputString;
	ExpressionEvaluator calculator;

	while (getline(std::cin, inputString))
		std::cout << calculator.driver(inputString) << std::endl;

	return 0;
}

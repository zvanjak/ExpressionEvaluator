#include <string>
#include <iostream>

#include "ExpressionEvaluator.h"

using std::getline;
using std::string;

int main()
{
	string inputString;
	ExpressionEvaluator calculator;

	while (getline(std::cin, inputString))
		std::cout << calculator.driver(inputString) << std::endl;

	return 0;
}

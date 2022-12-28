#include <string>
#include <iostream>

#include "ExpressionEvaluator.h"

using std::getline;
using std::string;

int main()
{
	string inputString;
	ExpressionEvaluator calculator;

    std::cout << "Enter expression :";
   	while (getline(std::cin, inputString))
    {
		std::cout << calculator.driver(inputString) << std::endl;
        std::cout << "Enter expression :";
    }

	return 0;
}

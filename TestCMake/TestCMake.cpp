// TestCMake.cpp : Defines the entry point for the application.
//

#include "TestCMake.h"

#include "Function.h"

//#define CATCH_CONFIG_MAIN

int main()
{
	int c = func(2, 3);

	std::cout << c;
}


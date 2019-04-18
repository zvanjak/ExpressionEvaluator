#pragma once

class DefinedFunction
{
public:
	DefinedFunction(int n) : _numParam(n) {}

	virtual int getNumParam() { return _numParam; }

	int _numParam;
};

class DefinedFunctionOneParam : public DefinedFunction
{
public:
	DefinedFunctionOneParam() : DefinedFunction(1) { }
	DefinedFunctionOneParam(double(*inFunc)(double)) : DefinedFunction(1) { _ptrFunc = inFunc; }

	DefinedFunctionOneParam& operator=(DefinedFunctionOneParam &b)
	{
		_numParam = 1;
		_ptrFunc = b._ptrFunc;
	}

	double(*_ptrFunc)(double);
};

class DefinedFunctionTwoParam : public DefinedFunction
{
public:
	DefinedFunctionTwoParam() : DefinedFunction(2) { }
	DefinedFunctionTwoParam(double(*inFunc)(double, double)) : DefinedFunction(2) { _ptrFunc = inFunc; }

	DefinedFunctionTwoParam& operator=(DefinedFunctionTwoParam &b)
	{
		_numParam = 2;
		_ptrFunc = b._ptrFunc;
	}

	double(*_ptrFunc)(double, double);
};

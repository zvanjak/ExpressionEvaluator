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

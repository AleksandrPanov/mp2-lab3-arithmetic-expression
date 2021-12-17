#pragma once
#include <vector>
#include <string>
#include "Enums.h"

class Lexeme
{
public:
	TypeLexeme type;
	std::string value;
	Priority prior;
};

class Func : public Lexeme
{
protected:
    std::vector<std::string> allNames;
	std::vector<LexemeState> possibleState;
	int amountOfArgs;

public:
    std::vector<std::string> getAllNames();
	int getAmountOfArgs();
	std::vector<LexemeState> getPossibleState();
	virtual double doOperation(std::vector<double>) = 0;
};

class Func0arg : public Func
{
public:
	Func0arg();
};

class Func1arg : public Func
{
public:
	Func1arg();
};

class Func2arg : public Func
{
public:
	Func2arg();
};

class Error : public Func
{
public:
	Error();
	double doOperation(std::vector<double> = {});
};

class AllFunc
{
public:
    std::vector<Func*> allFunc;
	AllFunc();

    ~AllFunc();
};

class Number : public Func0arg
{ 
public:
	Number(double val = 0);
	double doOperation(std::vector<double> = {});

};

class LeftScope : public Func0arg { public: LeftScope(); double doOperation(std::vector<double> = {}); };
class RightScope : public Func0arg { public: RightScope(); double doOperation(std::vector<double> = {}); };

class Pi : public Func0arg { public: Pi(); double doOperation(std::vector<double> = {}); };

class Exp : public Func0arg { public: Exp(); double doOperation(std::vector<double> = {}); };

class Plus : public Func2arg { public: Plus(); double doOperation(std::vector<double>); };

class BinMinus : public Func2arg { public: BinMinus(); double doOperation(std::vector<double>); };

class Mul : public Func2arg { public: Mul(); double doOperation(std::vector<double>); };

class Div : public Func2arg { public: Div(); double doOperation(std::vector<double>); };

class Pow : public Func2arg { public: Pow(); double doOperation(std::vector<double>); };

class UnMinus : public Func1arg { public: UnMinus(); double doOperation(std::vector<double>); };

class Log : public Func1arg { public: Log(); double doOperation(std::vector<double>); };

class Lg : public Func1arg { public: Lg(); double doOperation(std::vector<double>); };

class Sqrt : public Func1arg { public: Sqrt(); double doOperation(std::vector<double>); };

class Cbrt : public Func1arg { public: Cbrt(); double doOperation(std::vector<double>); };

class Abs : public Func1arg { public: Abs(); double doOperation(std::vector<double>); };

class Sin : public Func1arg { public: Sin(); double doOperation(std::vector<double>); };

class Cos : public Func1arg { public: Cos(); double doOperation(std::vector<double>); };

class Tan : public Func1arg { public: Tan(); double doOperation(std::vector<double>); };

class Cot : public Func1arg { public: Cot(); double doOperation(std::vector<double>); };

class Sec : public Func1arg { public: Sec(); double doOperation(std::vector<double>); };

class Csc : public Func1arg { public: Csc(); double doOperation(std::vector<double>); };

class Asin : public Func1arg { public: Asin(); double doOperation(std::vector<double>); };

class Acos : public Func1arg { public: Acos(); double doOperation(std::vector<double>);  };

class Atan : public Func1arg { public: Atan(); double doOperation(std::vector<double>); };

class Acot : public Func1arg { public: Acot(); double doOperation(std::vector<double>); };

class Asec : public Func1arg { public: Asec(); double doOperation(std::vector<double>); };

class Acsc : public Func1arg { public: Acsc(); double doOperation(std::vector<double>); };

class Sinh : public Func1arg { public: Sinh(); double doOperation(std::vector<double>); };

class Cosh : public Func1arg { public: Cosh(); double doOperation(std::vector<double>); };

class Tanh : public Func1arg { public: Tanh(); double doOperation(std::vector<double>); };

class Coth : public Func1arg { public: Coth(); double doOperation(std::vector<double>); };

class Asinh : public Func1arg { public: Asinh(); double doOperation(std::vector<double>); };

class Acosh : public Func1arg { public: Acosh(); double doOperation(std::vector<double>); };

class Atanh : public Func1arg { public: Atanh(); double doOperation(std::vector<double>); };

class Acoth : public Func1arg { public: Acoth(); double doOperation(std::vector<double>); };
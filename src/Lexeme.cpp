#include <cmath>
#include "Lexeme.h"


const double pi = 3.14159265;
const double e = 2.71828182;

AllFunc::AllFunc()
{
	std::vector<Func*> buffAllFunc =
	{
		new Number, new RightScope, new LeftScope, new Pi, new Exp, new Plus, new BinMinus, new Mul, new Div, new Pow, new UnMinus,
		new Log, new Lg, new Sqrt, new Cbrt, new Abs, new Sin, new Cos, new Tan, new Cot, new Cot, new Sec, new Csc, new Asin, new Acos,
		new Atan, new Acot, new Asec, new Acsc, new Sinh, new Cosh, new Tanh, new Coth, new Asinh, new Acosh, new Atanh, new Acoth
	};
	
	allFunc = buffAllFunc;
}
AllFunc::~AllFunc()
{
	for (auto pLex : allFunc)
	{
		delete pLex;
	}
}

std::vector<std::string> Func::getAllNames() { return allNames; }
int Func::getAmountOfArgs() { return amountOfArgs; }
std::vector<LexemeState> Func::getPossibleState() { return possibleState; }

Func0arg::Func0arg()
{
	amountOfArgs = 0;
	type = TypeLexeme::number;
	prior = Priority::hight; 
	std::vector<LexemeState> buffState = {LexemeState::state_start, LexemeState::state_bin_op, LexemeState::state_left_scope, LexemeState::state_un_op };
	possibleState = buffState;
}


Func1arg::Func1arg()
{
	amountOfArgs = 1;
	type = TypeLexeme::un_op;
	prior = Priority::hight;
	std::vector<LexemeState> buffState = { LexemeState::state_start, LexemeState::state_bin_op, LexemeState::state_left_scope, LexemeState::state_un_op };
	possibleState = buffState;
}

Func2arg::Func2arg()
{
	amountOfArgs = 2;
	type = TypeLexeme::bin_op;
	prior = Priority::low;
	std::vector<LexemeState> buffState = { LexemeState::state_variable,  LexemeState::state_right_scope,  LexemeState::state_un_op};
	possibleState = buffState;
}

Error::Error()
{
	amountOfArgs = -1;
	type = TypeLexeme::error;
	prior = Priority::none;
}
double Error::doOperation(std::vector<double>) { return -1; }

Number::Number(double val) : Func0arg()
{
	possibleState.push_back(LexemeState::state_variable);
	std::vector<std::string> buffName = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0", ".", "," };
	allNames = buffName;
	value = std::to_string(val);
}
double Number::doOperation(std::vector<double> arg) { return std::stod(value); }

LeftScope::LeftScope() : Func0arg()
{
	possibleState.push_back(LexemeState::state_right_scope);
	type = TypeLexeme::left_scope;
	prior = Priority::scope;
	allNames.push_back("(");
	value = allNames[0];
}
double LeftScope::doOperation(std::vector<double>) { return -1; }

RightScope::RightScope() : Func0arg()
{
	possibleState.push_back(LexemeState::state_right_scope);
	possibleState.push_back(LexemeState::state_variable);
	type = TypeLexeme::right_scope;
	prior = Priority::scope;
	allNames.push_back(")");
	value = allNames[0];
}
double RightScope::doOperation(std::vector<double>) { return -1; }

Pi::Pi(): Func0arg()
{
	type = TypeLexeme::variable;
	allNames.push_back(std::to_string(pi));
	allNames.push_back("pi");
	allNames.push_back("Pi");
	value = allNames[0];
}
double Pi::doOperation(std::vector<double> arg) { return std::stod(value); }

Exp::Exp() : Func0arg()
{
	type = TypeLexeme::variable;
	allNames.push_back(std::to_string(e));
	allNames.push_back("exp");
	allNames.push_back("e");
	value = allNames[0];
}
double Exp::doOperation(std::vector<double> arg) { return std::stod(value); }

Plus::Plus() : Func2arg()
{
	possibleState.push_back(LexemeState::state_right_scope);
	allNames.push_back("+");
	value = allNames[0];
}
double Plus::doOperation(std::vector<double> arg){ return arg[0] + arg[1]; }

BinMinus::BinMinus() : Func2arg()
{
	std::vector<std::string> buffAllNames = { "-" , "–" , "—" };
	allNames.push_back("-");
	allNames.push_back("–");
	allNames.push_back("—");
	value = allNames[0];
}
double BinMinus::doOperation(std::vector<double> arg) { return arg[0] - arg[1]; }

Mul::Mul() : Func2arg()
{
	prior = Priority::mid;
	allNames.push_back("*");
	value = allNames[0];
}
double Mul::doOperation(std::vector<double> arg) { return arg[0] * arg[1]; }

Div::Div() : Func2arg()
{
	prior = Priority::mid;
	allNames.push_back("/");
	value = allNames[0];
}
double Div::doOperation(std::vector<double> arg) { return arg[0] / arg[1]; }

Pow::Pow() : Func2arg()
{
	prior = Priority::mid;
	allNames.push_back("^");
	allNames.push_back("**");
	value = allNames[0];
}
double Pow::doOperation(std::vector<double> arg) { return std::pow(arg[0], arg[1]); }

UnMinus::UnMinus() : Func1arg()
{
	allNames.push_back("-");
	allNames.push_back("–");
	allNames.push_back("—");
	value = allNames[0];
}
double UnMinus::doOperation(std::vector<double> arg) { return -arg[0]; }


Log::Log() : Func1arg()
{
	allNames.push_back("log");
	allNames.push_back("ln");
	value = allNames[0];
}
double Log::doOperation(std::vector<double> arg) { return std::log(arg[0]); }

Lg::Lg() : Func1arg()
{
	allNames.push_back("lg");
	value = allNames[0];
}
double Lg::doOperation(std::vector<double> arg) { return std::log10(arg[0]); }

Sqrt::Sqrt() : Func1arg()
{
	allNames.push_back("sqrt");
	value = allNames[0];
}
double Sqrt::doOperation(std::vector<double> arg) { return std::sqrt(arg[0]); }

Cbrt::Cbrt() : Func1arg()
{
	allNames.push_back("cbrt");
	value = allNames[0];
}
double Cbrt::doOperation(std::vector<double> arg) { return std::cbrt(arg[0]); }

Sin::Sin() : Func1arg()
{
	allNames.push_back("sin");
	value = allNames[0];
}
double Sin::doOperation(std::vector<double> arg) { return std::sin(arg[0]); }


Cos::Cos() : Func1arg()
{
	allNames.push_back("cos");
	value = allNames[0];
}
double Cos::doOperation(std::vector<double> arg) { return std::cos(arg[0]); }


Abs::Abs() : Func1arg()
{
	allNames.push_back("abs");
	value = allNames[0];
}
double Abs::doOperation(std::vector<double> arg) { return std::abs(arg[0]); }


Tan::Tan() : Func1arg()
{
	allNames.push_back("tan");
	allNames.push_back("tg");
	value = allNames[0];
}
double Tan::doOperation(std::vector<double> arg) { return std::tan(arg[0]); }

Cot::Cot() : Func1arg()
{
	allNames.push_back("cot");
	allNames.push_back("ctg");
	value = allNames[0];
}
double Cot::doOperation(std::vector<double> arg) { return 1 / std::tan(arg[0]); }

Sec::Sec() : Func1arg()
{
	allNames.push_back("sec");
	value = allNames[0];
}
double Sec::doOperation(std::vector<double> arg) { return 1 / std::cos(arg[0]); }

Csc::Csc() : Func1arg()
{
	allNames.push_back("csc");
	value = allNames[0];
}
double Csc::doOperation(std::vector<double> arg) { return 1 / std::sin(arg[0]); }

Asin::Asin() : Func1arg()
{
	allNames.push_back("asin");
	allNames.push_back("arcsin");
	value = allNames[0];
} 
double Asin::doOperation(std::vector<double> arg) { return std::asin(arg[0]); }

Acos::Acos() : Func1arg()
{;
	allNames.push_back("acos");
	allNames.push_back("arccos");
	value = allNames[0];
}
double Acos::doOperation(std::vector<double> arg) { return std::acos(arg[0]); }

Atan::Atan() : Func1arg()
{
	allNames.push_back("atan");
	allNames.push_back("arctan");
	allNames.push_back("atg");
	allNames.push_back("arctg");
	value = allNames[0];
}
double Atan::doOperation(std::vector<double> arg) { return std::atan(arg[0]); }

Acot::Acot() : Func1arg()
{
	allNames.push_back("acot");
	allNames.push_back("arccot");
	allNames.push_back("actg");
	allNames.push_back("arcctg");
	value = allNames[0];
}
double Acot::doOperation(std::vector<double> arg) { return std::atan(1 / arg[0]); }

Asec::Asec() : Func1arg()
{
	allNames.push_back("asec");
	allNames.push_back("arsec");
	allNames.push_back("arcsec");
	value = allNames[0];
}
double Asec::doOperation(std::vector<double> arg) { return std::acos(1 / arg[0]); }

Acsc::Acsc() : Func1arg()
{
	allNames.push_back("acsc");
	allNames.push_back("arcsc");
	allNames.push_back("arccsc");
	value = allNames[0];
}
double Acsc::doOperation(std::vector<double> arg) { return std::asin(1 / arg[0]); }

Sinh::Sinh() : Func1arg()
{
	allNames.push_back("sinh");
	allNames.push_back("sh");
	value = allNames[0];
}
double Sinh::doOperation(std::vector<double> arg) { return std::sinh(arg[0]); }

Cosh::Cosh() : Func1arg()
{
	allNames.push_back("cosh");
	allNames.push_back("ch");
	value = allNames[0];
}
double Cosh::doOperation(std::vector<double> arg) { return std::cosh(arg[0]); }


Tanh::Tanh() : Func1arg()
{
	allNames.push_back("tanh");
	allNames.push_back("th");
	value = allNames[0];
}
double Tanh::doOperation(std::vector<double> arg) { return std::tanh(arg[0]); }

Coth::Coth() : Func1arg()
{
	allNames.push_back("coth");
	allNames.push_back("cth");
	value = allNames[0];
}
double Coth::doOperation(std::vector<double> arg) { return 1 / std::tanh(arg[0]); }

Asinh::Asinh() : Func1arg()
{
	allNames.push_back("asinh");
	allNames.push_back("arcsinh");
	allNames.push_back("arsinh");
	allNames.push_back("arsh");
	allNames.push_back("arcsh");
	value = allNames[0];
}
double Asinh::doOperation(std::vector<double> arg) { return std::asinh(1 / arg[0]); }

Acosh::Acosh() : Func1arg()
{
	allNames.push_back("acosh");
	allNames.push_back("arccosh");
	allNames.push_back("arcch");
	allNames.push_back("arch");
	allNames.push_back("arcch");
	value = allNames[0];
}
double Acosh::doOperation(std::vector<double> arg) { return std::acosh(1 / arg[0]); }

Atanh::Atanh() : Func1arg()
{
	allNames.push_back("atanh");
	allNames.push_back("arth");
	allNames.push_back("arcth");
	value = allNames[0];
}
double Atanh::doOperation(std::vector<double> arg) { return std::atanh(1 / arg[0]); }

Acoth::Acoth() : Func1arg()
{
	allNames.push_back("acoth");
	allNames.push_back("arcth");
	allNames.push_back("arccth");
	value = allNames[0];
}
double Acoth::doOperation(std::vector<double> arg) { return std::atanh(1 / arg[0]); }


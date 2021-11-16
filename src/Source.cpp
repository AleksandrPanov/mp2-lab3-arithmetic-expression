#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Document, standart
// компилятор
// gcc
// mvsc, gcc, clang, icc

// Арифмитические выражения
// Калькулятор
// input data:
//      std::string, арифмитическое выражение вида: 7+3-2-(-5)*4+3^2
// output data:
// 1. проверка корректности выражения: ++)2(-3532-5+3)(25235++2352(35---235
// 2. вычислить выражение
// список поддерживаемых операций (+,-,*,/,^, un -) и фич (поддержка переменных x, y, z; 
// поддержка функций sin, cos, ...; поддержка десятичных дробей 3.14)

// арифмитические знаки ~ оператор
// бинарный оператор f  // (a1, a2)
// унарный оператор f   // (a1)
// тернарный оператор f // (a1, a2, a3)
// ...
// число, переменная или выражение ~ операнд
// 123, x, (sin(7)*2+3)

// Текущее состояние ~ текущая, обрабатываемая лексема.


// первый в выражении:
// унарный оператор или операнд или (

// унарный оператор, что может идти за ним?
// (, операнд

// операнд (число, переменная), что может идти за ним?
// бинарный оператор, ), конец выражения


// бинарный оператор (+, -, /, *, ^),  что может идти за ним?
// операнд, (

// "(" что может идти за ней?
// опернад, унарный оператор

enum class TypeLexeme
{
	number,
	variable,
	un_op,
	bin_op,
	left_bracket,
	right_bracket,
	sin,
	cos,
	error
};

enum class Priority
{
	number,
	bracket,
	low,
	mid,
	high
};

struct Lexeme
{
	TypeLexeme type;
	std::string value;
	Priority priority;
};

enum class LexemeState
{
	start,
	number,
	variable,
	un_op,
	bin_op,
	left_bracket,
	right_bracket,
	end,
	sincos,
	error
};

Lexeme convertToLexeme(const std::string &expression, size_t &index, LexemeState state)
{
	std::string value;
	if (expression[index] >= '0' && expression[index] <= '9')
	{
		if (expression[index + 1] == '0')
		{
			value.push_back('0');
			++index;
			return Lexeme({ TypeLexeme::number, value, Priority::number });
		}
		while (expression[index] >= '0' && expression[index] <= '9')
		{
			value.push_back(expression[index]);
			++index;
		}
		return Lexeme({ TypeLexeme::number, value, Priority::number });
	}
	else if (expression[index] == '*' || expression[index] == '/')
	{
		value.push_back(expression[index]);
		index++;
		return Lexeme({ TypeLexeme::bin_op, value, Priority::mid });
	}
	else if (expression[index] == '^')
	{
		value.push_back(expression[index]);
		index++;
		return Lexeme({ TypeLexeme::bin_op, value, Priority::high });
	}
	else if (expression[index] == '+' || (expression[index] == '-' && (state == LexemeState::variable || state == LexemeState::number || state == LexemeState::right_bracket)))
	{
		value.push_back(expression[index]);
		index++;
		return Lexeme({ TypeLexeme::bin_op, value, Priority::low });
	}
	else if (expression[index] == '-' && (state == LexemeState::left_bracket || state == LexemeState::start))
	{
		value.push_back(expression[index]);
		index++;
		return Lexeme({ TypeLexeme::un_op, value, Priority::high });
	}
	else if (expression[index] == '(' && (state == LexemeState::sincos || state == LexemeState::bin_op || state == LexemeState::start || state == LexemeState::un_op))
	{
		value.push_back(expression[index]);
		index++;
		return Lexeme({ TypeLexeme::left_bracket, value, Priority::bracket });
	}
	else if (expression[index] == ')' && (state == LexemeState::variable || state == LexemeState::number))
	{
		value.push_back(expression[index]);
		index++;
		return Lexeme({ TypeLexeme::right_bracket, value, Priority::bracket });
	}
	else if ((expression.substr(index, 3) == "sin" || expression.substr(index, 3) == "cos") && (state == LexemeState::un_op || state == LexemeState::left_bracket || state == LexemeState::bin_op || state == LexemeState::start))
	{
		if (expression.substr(index, 3) == "sin")
		{
			value = "sin";
			index += 3;
			return Lexeme({ TypeLexeme::sin, value, Priority::high });
		}
		else if (expression.substr(index, 3) == "cos")
		{
			value = "cos";
			index += 3;
			return Lexeme({ TypeLexeme::cos, value, Priority::high });
		}
	}
	else if (expression[index] >= 'a' && expression[index] <= 'z')
	{
		value.push_back(expression[index]);
		index++;
		return Lexeme({ TypeLexeme::variable, value, Priority::low });
	}
	return Lexeme({ TypeLexeme::error, value, Priority::low });
}

// все лексемы состоят из одного символа, то есть число 12345 недопустимо
// нужно проконтролировать корректность скобок
// счетчик скобок scopeCounter
// if (scope == '(') scopeCounter++;
// if (scope == ')') scopeCounter--;
// в каждый момент scopeCounter >= 0
// (()))(
std::vector<Lexeme> convertStrToLexemes(const std::string& expression)
{
	int scopeCounter = 0;
	std::vector<Lexeme> res;
	LexemeState state = LexemeState::start;
	size_t index = 0;
	while (index < expression.size())
	{
		const Lexeme lexeme = convertToLexeme(expression, index, state);
		if (state == LexemeState::start)
		{
			if (lexeme.type == TypeLexeme::un_op)
			{
				state = LexemeState::un_op;
			}
			else if (lexeme.type == TypeLexeme::variable || lexeme.type == TypeLexeme::number)
			{
				state = LexemeState::variable;
			}
			else if (lexeme.type == TypeLexeme::left_bracket)
			{
				state = LexemeState::left_bracket;
				scopeCounter++;
			}
			else if (lexeme.type == TypeLexeme::sin || lexeme.type == TypeLexeme::cos)
			{
				state = LexemeState::sincos;
			}
			else throw "Error: invalid expression format (start lexeme is incorrect)";
		}
		else if (state == LexemeState::un_op)
		{
			if (lexeme.type == TypeLexeme::variable || lexeme.type == TypeLexeme::number)
			{
				state = LexemeState::variable;
			}
			else if (lexeme.type == TypeLexeme::sin || lexeme.type == TypeLexeme::cos)
			{
				state = LexemeState::sincos;
			}
			else if (lexeme.type == TypeLexeme::left_bracket)
			{
				state = LexemeState::left_bracket;
				scopeCounter++;
			}
			else throw "Error: invalid expression after unary operator";
		}
		else if (state == LexemeState::variable)
		{
			if (lexeme.type == TypeLexeme::bin_op)
			{
				state = LexemeState::bin_op;
			}
			else if (lexeme.type == TypeLexeme::right_bracket)
			{
				state = LexemeState::right_bracket;
				scopeCounter--;
			}
			else throw "Error: invalid expression after a variable";
		}
		else if (state == LexemeState::bin_op)
		{
			if (lexeme.type == TypeLexeme::variable || lexeme.type == TypeLexeme::number)
			{
				state = LexemeState::variable;
			}
			else if (lexeme.type == TypeLexeme::left_bracket)
			{
				state = LexemeState::left_bracket;
				scopeCounter++;
			}
			else if (lexeme.type == TypeLexeme::sin || lexeme.type == TypeLexeme::cos)
			{
				state = LexemeState::sincos;
			}
			else throw "Error: invalid expression after a binary operator";
		}
		else if (state == LexemeState::left_bracket)
		{
			if (lexeme.type == TypeLexeme::left_bracket)
			{
				state = LexemeState::left_bracket;
				scopeCounter++;
			}
			else if (lexeme.type == TypeLexeme::un_op)
			{
				state = LexemeState::un_op;
			}
			else if (lexeme.type == TypeLexeme::variable || lexeme.type == TypeLexeme::number)
			{
				state = LexemeState::variable;
			}
			else if (lexeme.type == TypeLexeme::sin || lexeme.type == TypeLexeme::cos)
			{
				state = LexemeState::sincos;
			}
			else throw "Error: invalid expression after a left bracket";
		}
		else if (state == LexemeState::right_bracket)
		{
			if (lexeme.type == TypeLexeme::right_bracket)
			{
				state = LexemeState::right_bracket;
				scopeCounter--;
			}
			else if (lexeme.type == TypeLexeme::bin_op)
			{
				state = LexemeState::bin_op;
			}
			else throw "Error: invalid expression after a right bracket";
		}
		else if (state == LexemeState::sincos)
		{
			if (lexeme.type == TypeLexeme::variable)
			{
				state = LexemeState::variable;
			}
			else if (lexeme.type == TypeLexeme::left_bracket)
			{
				state = LexemeState::left_bracket;
				scopeCounter++;
			}
			else throw "Error: invalid sin/cos argument";
		}
		else if (scopeCounter < 0) throw "Error: brackets are out of order";
		else throw "error";
		res.push_back(lexeme);
	}
	return res;
}

// 2^3/4

int main()
{
	std::vector<Lexeme> test = convertStrToLexemes("-3+19*sin(x)+(-3)+18*cos(2398)");
	for (int i = 0; i < test.size(); i++)
	{
		std::cout << test[i].value << '\t' << static_cast<int>(test[i].type) << std::endl;
	}
	getchar();
	return 0;
}

// API
bool check_arithmetic_expression(const std::string& s)
{
	return false;
}

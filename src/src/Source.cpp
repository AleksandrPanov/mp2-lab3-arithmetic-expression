#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cmath>
using namespace std;
#define PI 3.14159265358979323846

enum class TypeLexeme {
	number,
	variable,
	bin_op,
	un_op,
	left_scope,
	right_scope,
	error
};

enum class Priority {
	number, 
	plus,
	mult,
	pow,
	un_op,
	scope,
};

struct Lexeme {
	TypeLexeme type;
	std::string value;
	Priority priority;
};

enum class StateLexeme {
	start,
	number,
	variable,
	un_op,
	bin_op,
	left_scope,
	right_scope,
	end,
	error
};

Lexeme crToLexeme(const std::string& exp, size_t& index, StateLexeme state) {
	std::string val;
	if (exp[index] == '*' || exp[index] == '/')
	{
		val.push_back(exp[index++]);
		return Lexeme({ TypeLexeme::bin_op, val, Priority::mult });
	}
	else if (exp[index] == '+')
	{
		val.push_back(exp[index++]);
		return Lexeme({ TypeLexeme::bin_op, val, Priority::plus });
	}
	else if (exp[index] == '-')
	{
		if (state == StateLexeme::start || state == StateLexeme::left_scope)
		{
			return Lexeme({ TypeLexeme::un_op ,val, Priority::un_op });
		}
		else if (state == StateLexeme::variable || state == StateLexeme::number || state == StateLexeme::right_scope)
		{
			return Lexeme{ TypeLexeme::bin_op,val ,Priority::plus};
		}
	}

	else if (exp[index] == '^')
	{
		val.push_back(exp[index++]);
		return Lexeme({TypeLexeme::bin_op, val, Priority::pow});
	}

	else if (exp[index] == '(')
	{
		val.push_back(exp[index++]);
		return Lexeme({TypeLexeme::left_scope, val, Priority::scope});
	}
	else if (exp[index] == ')')
	{
		val.push_back(exp[index++]);
		return Lexeme({TypeLexeme::right_scope, val, Priority::scope});

	}

	else if (exp.substr(index, 3) == "sin" || exp.substr(index, 3) == "cos" || exp.substr(index, 3) == "sqrt" || exp.substr(index, 3) == "tg" || exp.substr(index, 3) == "ctg" || exp.substr(index, 3) == "ln" || exp.substr(index, 3) == "arcsin" || exp.substr(index, 3) == "arccos" || exp.substr(index, 3) == "arctg" || exp.substr(index, 3) == "arcctg")
	{
		val = exp.substr(index, 3);
		index += 3;
		return Lexeme({ TypeLexeme::un_op, val, Priority::un_op});
	}

	else if (exp[index] >= '0' && exp[index] <= '9')
	{
		while (exp[index] >= '0' && exp[index] <= '9')
		{
			val.push_back(exp[index++]);
		}
		return Lexeme({ TypeLexeme::number, val, Priority::number });
	}

	return Lexeme({ TypeLexeme::error, val, Priority::plus });
}

std::vector<Lexeme> convertStrToLexeme(const std::string& str)
{
	int counter = 0;
	size_t index = 0;
	std::vector<Lexeme> res;
	StateLexeme state = StateLexeme::start;
	for (index; index < str.size(); index++)
	{
		const Lexeme lexeme = crToLexeme(str, index, state);
		if (state == StateLexeme::start)
		{
			if (lexeme.type == TypeLexeme::un_op)
				state = StateLexeme::un_op;
			else if (lexeme.type == TypeLexeme::variable || lexeme.type == TypeLexeme::number)
				state = StateLexeme::variable;
			else if (lexeme.type == TypeLexeme::left_scope)
			{
				state = StateLexeme::left_scope;
				counter++;
			}
			else throw "Error";
		}
		else if (state == StateLexeme::un_op)
		{
			if (lexeme.type == TypeLexeme::variable || lexeme.type == TypeLexeme::number)
			{
				state = StateLexeme::variable;
			}
			else if (lexeme.type == TypeLexeme::left_scope)
			{
				state = StateLexeme::left_scope;
				counter++;
			}
			else if (lexeme.type == TypeLexeme::un_op)
			{
				state = StateLexeme::un_op;
			}
			else throw "Error";
		}

		else if (state == StateLexeme::variable)
		{
			if (lexeme.type == TypeLexeme::bin_op)
			{
				state = StateLexeme::bin_op;
			}
			else if (lexeme.type == TypeLexeme::right_scope)
			{
				state = StateLexeme::right_scope;
				counter++;
			}
			else throw "Error";
		}

		else if (state == StateLexeme::bin_op)
		{
			if (lexeme.type == TypeLexeme::variable || lexeme.type == TypeLexeme::number)
			{
				
				state = StateLexeme::variable;
			}
			else if (lexeme.type == TypeLexeme::left_scope)
			{
				state = StateLexeme::left_scope;
				counter++;
			}
			else if (lexeme.type == TypeLexeme::un_op)
			{
				state = StateLexeme::un_op;
			}
			else throw "Error";
		}

		else if (state == StateLexeme::right_scope)
		{
			if (lexeme.type == TypeLexeme::right_scope)
			{
				state = StateLexeme::right_scope;
				counter--;
			}
			else if (lexeme.type == TypeLexeme::bin_op)
			{
				state = StateLexeme::bin_op;
			}
			else throw "Error";
		}

		else if (state == StateLexeme::left_scope)
		{
			if (lexeme.type == TypeLexeme::left_scope)
			{
				state = StateLexeme::left_scope;
				counter++;
			}
			else if (lexeme.type == TypeLexeme::un_op)
			{
				state = StateLexeme::un_op;
			}
			else if (lexeme.type == TypeLexeme::variable || lexeme.type == TypeLexeme::number)
			{
				state = StateLexeme::variable;
			}
			else if (lexeme.type == TypeLexeme::un_op)
			{
				state = StateLexeme::un_op;
			}
			else throw "Error";
		}
		else throw "Error";
		res.push_back(lexeme);
	}
	return res;
}

std::vector<Lexeme> reverse(const std::vector<Lexeme>& src)
{
	std::vector<Lexeme> out;
	std::vector<Lexeme> stack;
	for (const Lexeme& lex : src)
	{
		if (lex.type == TypeLexeme::variable || lex.type == TypeLexeme::number)
		{
			out.push_back(lex);
		}
		else if (lex.type == TypeLexeme::un_op || lex.type == TypeLexeme::left_scope)
		{
			stack.push_back(lex);
		}
		else if (lex.type == TypeLexeme:: right_scope)
		{
			while (stack.back().type != TypeLexeme::left_scope)
			{
				out.push_back(stack.back());
				stack.pop_back();
			}
			stack.pop_back();
		}
		else if (lex.type == TypeLexeme::bin_op)
		{
			while (!stack.empty() && stack.back().priority > lex.priority)
			{
				out.push_back(stack.back());
				stack.pop_back();
			}
			stack.push_back(lex);
		}
	}
	while (!stack.empty())
	{
		out.push_back(stack.back());
		stack.pop_back();
	}
	return out;
}

double calcReverse(const std::vector<Lexeme>& reverse)
{
	std::vector<Lexeme> stack;
	for (const Lexeme& lex: reverse)
	{
		if (lex.type == TypeLexeme::number)
			stack.push_back(lex);
		else if (lex.type == TypeLexeme::un_op)
		{
			double a = stod(stack.back().value);
			stack.pop_back();
			if (lex.value == "-")
				stack.push_back({ TypeLexeme::number, to_string(-a), Priority::number });
			else if (lex.value == "sin")
				stack.push_back({ TypeLexeme::number, to_string(sin(a)), Priority::number });
			else if (lex.value == "cos")
				stack.push_back({ TypeLexeme::number, to_string(cos(a)), Priority::number });
			else if (lex.value == "tg")
				stack.push_back({ TypeLexeme::number, to_string(tan(a)), Priority::number });
			else if (lex.value == "ctg")
				stack.push_back({ TypeLexeme::number, to_string(1 / tan(a)), Priority::number });
			else if (lex.value == "arcsin")
				stack.push_back({ TypeLexeme::number, to_string(asin(a)), Priority::number });
			else if (lex.value == "arccos")
				stack.push_back({ TypeLexeme::number, to_string(acos(a)), Priority::number });
			else if (lex.value == "arctg")
				stack.push_back({ TypeLexeme::number, to_string(atan(a)), Priority::number });
			else if (lex.value == "arcctg")
				stack.push_back({ TypeLexeme::number, to_string(PI / 2 - atan(a)), Priority::number });
			else if (lex.value == "sqrt")
				stack.push_back({ TypeLexeme::number, to_string(pow(a, 0.5)), Priority::number });
			else if (lex.value == "ln")
				stack.push_back({ TypeLexeme::number, to_string(log(a)), Priority::number });
		}
		else if (lex.type == TypeLexeme::bin_op)
		{
			Lexeme a2 = stack.back();
			stack.pop_back();
			Lexeme a1 = stack.back();
			stack.pop_back();
			double b1 = std::stod(a1.value);
			double b2 = std::stod(a2.value);
			std::string t;
			if (lex.value == "+")
			{
				t = std::to_string(b1 + b2);
			}
			else if (lex.value == "-")
			{
				t = std::to_string(b1 - b2);
			}
			else if (lex.value == "*")
			{
				t = std::to_string(b1 * b2);
			}
			else if (lex.value == "/")
			{
				t = std::to_string(b1 / b2);
			}
			else if (lex.value == "^")
			{
				t = std::to_string(pow(b1, b2));
			}
			Lexeme t2 = { TypeLexeme::number, t, Priority::number };
			stack.push_back(t2);
		}
	}
	double res = std::stod(stack.back().value);
	return res;
}

bool checkArithmExp(const string& str) {
	try {
		vector<Lexeme> result = convertStrToLexeme(str);
	}
	catch (const char* str) {
		cout << str;
		return false;
	}
	catch (const string str) {
		cout << "Incorrect";
		return false;
	}
	return true;
}

void calcExp(string& str) {
	if (checkArithmExp(str)) 
	{
		vector<Lexeme> lexemes = convertStrToLexeme(str);
		cout << calcReverse(reverse(lexemes));
	}
}


int main() {
	string exp;
	getline(cin, exp);

	cout << endl;
	calcExp(exp);
	cout << endl;

	return 0;
}

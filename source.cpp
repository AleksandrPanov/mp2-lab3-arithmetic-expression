#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <sstream>
#include <cmath>

#define PI 3.1415926535

using namespace std;

enum TypeLex
{
	number,
	bin_op,
	un_op,
	variable,
	left_scope,
	right_scope,
	error
};

enum class Priority
{
	scope,
	number,
	plus,
	mult,
	pow,
	un_op,
};

struct Lex
{
	TypeLex type;
	std::string val;
	Priority priority;
};

enum class LexState
{
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

Lex elToLex(const std::string& ex, int& ind, LexState state)
{
	std::string val;
	if (ex[ind] == '*' || ex[ind] == '/')
	{
		val.push_back(ex[ind++]);
		return Lex({ TypeLex::bin_op, val, Priority::mult });
	}
	else if (ex[ind] == '^')
	{
		val.push_back(ex[ind++]);
		return Lex({ TypeLex::bin_op, val, Priority::pow });
	}
	else if (ex[ind] == '+')
	{
		val.push_back(ex[ind++]);
		return Lex({ TypeLex::bin_op, val, Priority::plus });
	}
	else if (ex[ind] == '-')
	{
		if (state == LexState::variable || state == LexState::number || state == LexState::right_scope)
		{
			val.push_back(ex[ind++]);
			return Lex({ TypeLex::bin_op, val, Priority::plus });
		}
		else if (state == LexState::start || state == LexState::left_scope)
		{
			val.push_back(ex[ind++]);
			return Lex({ TypeLex::un_op, val, Priority::un_op });
		}
	}
	else if (ex[ind] == '(')
	{
		val.push_back(ex[ind++]);
		return Lex({ TypeLex::left_scope, val, Priority::scope });
	}
	else if (ex[ind] == ')')
	{
		val.push_back(ex[ind++]);
		return Lex({ TypeLex::right_scope, val, Priority::scope });
	}
	else if (ex.substr(ind, 3) == "sin" || ex.substr(ind, 3) == "cos" || ex.substr(ind, 3) == "tg" || ex.substr(ind, 3) == "ctg" || ex.substr(ind, 3) == "ln" || ex.substr(ind, 3) == "arcsin" || ex.substr(ind, 3) == "arccos" || ex.substr(ind, 3) == "arctg" || ex.substr(ind, 3) == "arcctg" || ex.substr(ind, 3) == "sqrt")
	{
		val = ex.substr(ind, 3);
		ind += 3;
		return Lex({ TypeLex::un_op, val, Priority::un_op });
	}
	else if (ex[ind] >= '0' && ex[ind] <= '9')
	{
		while (ex[ind] >= '0' && ex[ind] <= '9')
		{
			val.push_back(ex[ind++]);
		}
		return Lex({ TypeLex::number, val, Priority::number });
	}
	else
		return Lex{ TypeLex::error, val, Priority::plus };
}

std::vector<Lex> convertStrtoLex(std::string str)
{
	int count = 0;
	int ind = 0;
	std::vector<Lex> res;
	LexState state = LexState::start;
	for (ind; ind < str.size(); ind++)
	{
		const Lex lexeme = elToLex(str, ind, state);
		if (state == LexState::start)
		{
			if (lexeme.type == TypeLex::un_op)
				state = LexState::un_op;
			else if (lexeme.type == TypeLex::variable || lexeme.type == TypeLex::number)
				state = LexState::variable;
			else if (lexeme.type == TypeLex::left_scope)
			{
				state = LexState::left_scope;
				count++;
			}
			else throw "Error";
		}
		else if (state == LexState::un_op)
		{
			if (lexeme.type == TypeLex::variable || lexeme.type == TypeLex::number)
				state = LexState::variable;
			else if (lexeme.type == TypeLex::un_op)
				state = LexState::un_op;
			else if (lexeme.type == TypeLex::left_scope)
			{
				state = LexState::left_scope;
				count++;
			}
			else throw "Error";
		}
		else if (state == LexState::bin_op)
		{
			if (lexeme.type == TypeLex::variable || lexeme.type == TypeLex::number)
				state = LexState::variable;
			else if (lexeme.type == TypeLex::un_op)
				state = LexState::un_op;
			else if (lexeme.type == TypeLex::left_scope)
			{
				state = LexState::left_scope;
				count++;
			}
			else throw "Error";
		}
		else if (state == LexState::variable)
		{
			if (lexeme.type == TypeLex::bin_op)
				state = LexState::bin_op;
			else if (lexeme.type == TypeLex::right_scope)
			{
				state = LexState::right_scope;
				count++;
			}
			else throw "Error";
		}
		else if (state == LexState::left_scope)
		{
			if (lexeme.type == TypeLex::left_scope)
			{
				state = LexState::left_scope;
				count++;
			}
			else if (lexeme.type == TypeLex::un_op)
				state = LexState::un_op;
			else if (lexeme.type == TypeLex::variable || lexeme.type == TypeLex::number)
				state = LexState::variable;
			else if (lexeme.type == TypeLex::un_op)
				state = LexState::un_op;
			else throw "Error";
		}
		else if (state == LexState::right_scope)
		{
			if (lexeme.type == TypeLex::right_scope)
			{
				state = LexState::right_scope;
				count--;
			}
			else if (lexeme.type == TypeLex::bin_op)
				state = LexState::bin_op;
			else throw "Error";
		}
		else if (count < 0) throw "Error";
		else throw "Error";
		res.push_back(lexeme);
	}
	return res;
}

const std::vector<Lex> reverse(const std::vector<Lex>& pam)
{
	std::vector<Lex>out;
	std::vector<Lex>stack;
	for (const Lex& lex : pam)
	{
		if (lex.type == number)
			out.push_back(lex);
		else if (lex.type == un_op || lex.type == left_scope)
			stack.push_back(lex);
		else if (lex.type == right_scope)
		{
			while (stack.back().type != left_scope)
			{
				out.push_back(stack.back());
				stack.pop_back();
			}
			stack.pop_back();
		}
		else if (lex.type == bin_op)
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

double calc_rev(const std::vector<Lex>& rev)
{
	vector<Lex> stack;
	for (const Lex& lex : rev)
	{
		if (lex.type == TypeLex::number)
			stack.push_back(lex);
		else if (lex.type == TypeLex::un_op)
		{
			double a = stod(stack.back().val);
			stack.pop_back();
			if (lex.val == "-")
				stack.push_back({ TypeLex::number, to_string(-a), Priority::number });
			else if (lex.val == "sin")
				stack.push_back({ TypeLex::number, to_string(sin(a)), Priority::number });
			else if (lex.val == "cos")
				stack.push_back({ TypeLex::number, to_string(cos(a)), Priority::number });
			else if (lex.val == "tg")
				stack.push_back({ TypeLex::number, to_string(tan(a)), Priority::number });
			else if (lex.val == "ctg")
				stack.push_back({ TypeLex::number, to_string(1 / tan(a)), Priority::number });
			else if (lex.val == "arcsin")
				stack.push_back({ TypeLex::number, to_string(asin(a)), Priority::number });
			else if (lex.val == "arccos")
				stack.push_back({ TypeLex::number, to_string(acos(a)), Priority::number });
			else if (lex.val == "arctg")
				stack.push_back({ TypeLex::number, to_string(atan(a)), Priority::number });
			else if (lex.val == "arcctg")
				stack.push_back({ TypeLex::number, to_string(PI / 2 - atan(a)), Priority::number });
			else if (lex.val == "sqrt")
				stack.push_back({ TypeLex::number, to_string(pow(a, 0.5)), Priority::number });
			else if (lex.val == "ln")
				stack.push_back({ TypeLex::number, to_string(log(a)), Priority::number });
		}
		else if (lex.type == TypeLex::bin_op)
		{
			double b = stod(stack.back().val);
			stack.pop_back();
			double a = stod(stack.back().val);
			stack.pop_back();
			if (lex.val == "+")
				stack.push_back({ TypeLex::number, to_string(a + b), Priority::number });
			else if (lex.val == "-")
				stack.push_back({ TypeLex::number, to_string(a - b), Priority::number });
			else if (lex.val == "*")
				stack.push_back({ TypeLex::number, to_string(a * b), Priority::number });
			else if (lex.val == "/")
				stack.push_back({ TypeLex::number, to_string(a / b), Priority::number });
			else if (lex.val == "^")
				stack.push_back({ TypeLex::number, to_string(pow(a, b)), Priority::number });
		}
	}
	double res = stod(stack.back().val);
	return res;
}

bool check_first(const string& value) 
{
	try
	{
		vector<Lex> res = convertStrtoLex(value);
	}
	catch (const char* str) 
	{
		cout << str;
		return false;
	}
	catch (const string str) 
	{
		cout << "Incorrect";
		return false;
	}
	return true;
}
void check_ex(string& value) 
{
	if (check_first(value)) 
	{
		vector<Lex> lexemes = convertStrtoLex(value);
		cout << calc_rev(reverse(lexemes));
	}
}
int main()
{
	string ex;
	cout << "Enter expression: \n";
	getline(cin, ex);
	cout << endl;
	check_ex(ex);
	cout << endl;

	return 0;
} 


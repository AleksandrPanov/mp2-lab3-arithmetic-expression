#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <sstream>
#include <cmath>
#define PI 3,1415926535
std::vector<std::string>math = { "sin","cos","tg","ctg","arcsin","arccos","arctg","arcctg","sh","ch","th","cth","arsh","arch","arth","sec","cosec","sqrt","abs","ln"};
enum TypeLexeme {
	number,
	bin_op,
	un_op,
	variable,
	left_scope,
	right_scope,
	error
};
enum LexemeState {
	state_start,
	state_bin_op,
	state_un_op,
	state_variable,
	state_left_scope,
	state_right_scope,
	state_end,
	state_error
};
enum Priority{
	pnumber,
	scope,
	low,
	mid,
	hight,
	p_un_op
};
struct Lexeme {

	TypeLexeme type;
	std::string value;
	Priority priority;
};
Lexeme createNotNumberLexeme(const std::string el, LexemeState state)
{
	if(el=="*")
	{ 
		return Lexeme({ TypeLexeme::bin_op,el,Priority::mid });
	}
	else if(el == "/")
	{
		return Lexeme({ TypeLexeme::bin_op,el,Priority::mid });
	}
	else if (el == "+")
	{
		return Lexeme({ TypeLexeme::bin_op,el,Priority::low });
	}
	else if (el == "-")
	{
		if ( state == LexemeState::state_left_scope || state == LexemeState::state_start)
		{
			return Lexeme({ TypeLexeme::un_op,el,Priority::p_un_op });
		}
		else
		{
			return Lexeme{ TypeLexeme::bin_op,el,Priority::low };
		}
	}
	else if (el == "^")
	{
		return Lexeme{ TypeLexeme:: bin_op,el,Priority::hight };
	}
	else if (el == "sin"|| el == "cos" || el == "tg" || el == "ctg" || el == "arcsin" || el == "arccos" || el == "arctg" || el == "arcctg" || el == "sh" || el == "ch" || el == "th" || el == "cth" || el == "arsh" || el == "arch" || el == "arth" || el == "arcth" || el == "sec" || el == "cosec" || el == "sqrt" || el == "abs" || el == "ln")
	{
		return Lexeme{ TypeLexeme::un_op,el,Priority::p_un_op };
	}
	else if (el == "(")
	{
		return Lexeme{ TypeLexeme::left_scope,el,Priority::scope };
	}
	else if (el == ")")
	{
		return Lexeme{ TypeLexeme::right_scope,el,Priority::scope };
	}
	else {
	return Lexeme{ TypeLexeme::error,el,Priority::low };
	}
}
std::vector<Lexeme> convertStrtoLex(std::string str)
{
	std::vector<Lexeme> lex;
	LexemeState state = LexemeState::state_start;
	while (str[0] != '\0')
	{
		if (str[0] != ' ') {
			Lexeme tmp;
			if (isdigit(str[0]))
			{   
				char* str2;
				std::string number;
				double f = strtod(str.c_str(), &str2);
				str = str2;
				number = std::to_string(f);
				tmp = { TypeLexeme::number,number, Priority::pnumber };
			}
			else  
			{
				std::string tmp2;
				for (size_t i = 0; i < math.size(); ++i)
				{
					if (str.find(math[i]) == 0)
					{
						tmp2 = math[i];
						break;
					}
				} 
				if(tmp2.empty())
					tmp2.push_back(str[0]);
				tmp = createNotNumberLexeme(tmp2, state);
				str.erase(0, tmp2.size());
				
			}
			if (state==LexemeState::state_start)
			{
				if (tmp.type == TypeLexeme::left_scope)
				{
					state = LexemeState::state_left_scope;
				}
				else if (tmp.type == TypeLexeme::number|| tmp.type == TypeLexeme::variable)
				{
					state = LexemeState::state_variable;
				}
				else if (tmp.type == TypeLexeme::un_op)
				{
					state = LexemeState::state_un_op;
				}
				else
				{
					throw "error";
				}
			}
			else if (state == LexemeState::state_left_scope)
			{
				if (tmp.type == TypeLexeme::left_scope)
				{
					state = LexemeState::state_left_scope;
				}
				else if (tmp.type == TypeLexeme::un_op)
				{
					state = LexemeState::state_un_op;
				}
				else if (tmp.type == TypeLexeme::number || tmp.type == TypeLexeme::variable)
				{
					state = LexemeState::state_variable;
				}
				else
				{
					throw "error";
				}
			}
			 else if (state == LexemeState::state_variable)
			{
				if (tmp.type == TypeLexeme::right_scope)
				{
					state = LexemeState::state_right_scope;
				}
				else if (tmp.type == TypeLexeme::bin_op)
				{
					state = LexemeState::state_bin_op;
				}
				else
				{
					throw "error";
				}
			}
			 else if (state == LexemeState::state_right_scope)
			{
			    if (tmp.type == TypeLexeme::right_scope)
				{
					state = LexemeState::state_right_scope;
				}
				else if (tmp.type == TypeLexeme::bin_op)
				{
					state = LexemeState::state_bin_op;
				}
				else
				{
					throw "error";
				}
			}
			else if (state == LexemeState::state_un_op)
			{
				if (tmp.type == TypeLexeme::left_scope)
				{
					state = LexemeState::state_left_scope;
				}
				else if (tmp.type == TypeLexeme::number|| tmp.type == TypeLexeme::variable)
				{
					state = LexemeState::state_variable;
				}
				else
				{
					throw "error";
				}
			}
			else if (state == LexemeState::state_bin_op)
			{
				if (tmp.type == TypeLexeme::left_scope)
				{
					state = LexemeState::state_left_scope;
				}
				else if (tmp.type == TypeLexeme::number || tmp.type == TypeLexeme::variable)
				{
					state = LexemeState::state_variable;
				}
				else if(tmp.type == TypeLexeme::un_op)
				{
					state = LexemeState::state_un_op;
				}
				else
				{
					throw "error";
				}
			}
			lex.push_back(tmp);
		}
		else {
			str.erase(0, 1);
		}
	}
	return lex;
}
const std::vector<Lexeme> reverse(const std::vector<Lexeme>& tmp)
{
	std::vector<Lexeme>reverse;
	std::vector<Lexeme>stack;
	for (const Lexeme &lex : tmp)
	{
		if (lex.type == number)
		{
			reverse.push_back(lex);
		}
		else if (lex.type == un_op|| lex.type == left_scope)
		{
			stack.push_back(lex);
		}
		else if (lex.type == right_scope)
		{
			while (stack.back().type != left_scope)
			{
				reverse.push_back(stack.back());
				stack.pop_back();
			}
			stack.pop_back();
		}
		else if (lex.type == bin_op)
		{
			while (!stack.empty() && stack.back().priority > lex.priority)
			{
				reverse.push_back(stack.back());
				stack.pop_back();
			}
			stack.push_back(lex);
		}
	}
	while (!stack.empty())
	{
		reverse.push_back(stack.back());
		stack.pop_back();
	}
	return reverse;
}
double calculate_reverse(const std::vector<Lexeme>& reverse)
{
	std::vector<Lexeme>stack;
	for (const Lexeme& lex : reverse)
	{
		if (lex.type == TypeLexeme::number)
		{
			std::string tmp=std::to_string(stod(lex.value));
			Lexeme lex2 = { TypeLexeme::number, tmp, Priority::pnumber };
			stack.push_back(lex2);
		}
		else if (lex.type == TypeLexeme::un_op)
		{
			if (lex.value == "-")
			{
				stack.back().value = std::to_string(-stod(stack.back().value));
			}
			if (lex.value == "sin")
			{
				stack.back().value = std::to_string(sin(stod(stack.back().value)));
			}
			if (lex.value == "cos")
			{
				stack.back().value = std::to_string(cos(stod(stack.back().value)));
			}
			if (lex.value == "tg")
			{
				stack.back().value = std::to_string(tan(stod(stack.back().value)));
			}
			if (lex.value == "ctg")
			{
				stack.back().value = std::to_string(1/tan(stod(stack.back().value)));
			}
			if (lex.value == "arcsin")
			{
				stack.back().value = std::to_string(asin(stod(stack.back().value)));
			}
			if (lex.value == "arccos")
			{
				stack.back().value = std::to_string(acos(stod(stack.back().value)));
			}
			if (lex.value == "arctg")
			{
				stack.back().value = std::to_string(atan(stod(stack.back().value)));
			}
			if (lex.value == "arcctg")
			{
				stack.back().value = std::to_string((PI/2) - atan(stod(stack.back().value)));
			}
			if (lex.value == "sh")
			{
				stack.back().value = std::to_string(sinh(stod(stack.back().value)));
			}
			if (lex.value == "ch")
			{
				stack.back().value = std::to_string(cosh(stod(stack.back().value)));
			}
			if (lex.value == "th")
			{
				stack.back().value = std::to_string(tanh(stod(stack.back().value)));
			}
			if (lex.value == "cth")
			{
				stack.back().value = std::to_string(1/tanh(stod(stack.back().value)));
			}
			if (lex.value == "arsh")
			{
				stack.back().value = std::to_string(asinh(stod(stack.back().value)));
			}
			if (lex.value == "arch")
			{
				stack.back().value = std::to_string(acosh(stod(stack.back().value)));
			}
			if (lex.value == "arth")
			{
				stack.back().value = std::to_string( atanh(stod(stack.back().value)));
			}
			if (lex.value == "sec")
			{
				stack.back().value = std::to_string(1/cos(stod(stack.back().value)));
			}
			if (lex.value == "cosec")
			{
					stack.back().value = std::to_string(1/sin(stod(stack.back().value)));
			}
			if (lex.value == "sqrt")
			{
				stack.back().value = std::to_string(sqrt(stod(stack.back().value)));
			}
			if(lex.value == "abs")
			{
				stack.back().value = std::to_string(abs(stod(stack.back().value)));
			}
			if(lex.value == "ln")
			{
				stack.back().value = std::to_string(log(stod(stack.back().value)));
			}
		}
		else if (lex.type == TypeLexeme::bin_op)
		{
			Lexeme x2 = stack.back();
			stack.pop_back();
			Lexeme x1 = stack.back();
			stack.pop_back();
			double _x1 = std::stod(x1.value);
			double  _x2 = std::stod(x2.value);
			std::string tmp;
			if (lex.value == "+")
			{
				 tmp = std::to_string(_x1 + _x2);
			}
			else if (lex.value == "-")
			{
				 tmp = std::to_string(_x1 - _x2);
			}
			else if (lex.value == "*")
			{
				tmp = std::to_string(_x1 * _x2);
			}
			else if (lex.value == "/")
			{
				 tmp = std::to_string(_x1 / _x2);
			}
			else if (lex.value == "^")
			{
				 tmp = std::to_string(pow(_x1, _x2));
			}
			Lexeme tmp2 = { TypeLexeme::number, tmp, Priority::pnumber };
			stack.push_back(tmp2);
		}
	}
	double res = std::stod(stack.back().value);
	return res;
}
bool check_scops_in_expression(const std::string& str)
{
	std::cout << "checking the validity of scopes " << std::endl;
	size_t errors = 0;
	std::vector<int>stack;
	for (size_t i = 0; i < str.size(); ++i)
	{
		if (str[i] == '(')
		{
			stack.push_back(i);
		}
		if (str[i] == ')')
		{
			size_t tos = 0;
			if (!stack.empty())
			{
				tos = stack.back();
				stack.pop_back();
			}
			else {
				errors++;
			}
			std::cout << tos << "  " << i << std::endl;
		}
	}
	if (!stack.empty()) {
		while (!stack.empty())
		{
			std::cout << stack.back() << " " << "0" << std::endl;
			stack.pop_back();
			errors++;
		}
	}
	if (errors > 0)
	{
		std::cout << "incorrect"<<std::endl;
		std::cout << "errors:" << errors << std::endl;
		return 0;
	}
	else { 
		std::cout << "correct"<<std::endl;
	return 1;
      }
}
int main()
{
	std::string str = "2^0.5+(10.5+(-12)+arcsin1)*sin9^2+ln5";
	if (check_scops_in_expression(str)) {
		std::vector<Lexeme> test = convertStrtoLex(str);
		auto res = reverse(test);
		std::cout << calculate_reverse(res) << std::endl;
	}
	check_scops_in_expression("(123+(12-6*(12))*(7)");
	check_scops_in_expression("(123+(12-6*(12))*(7)+8)+9)+10)");
	check_scops_in_expression("123)+8*9+10");
	return 0;
}
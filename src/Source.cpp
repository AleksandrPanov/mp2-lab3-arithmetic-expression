#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <set>
using namespace std;

//оператор ^ не работает корректно не учтена правая ассоциативность
//для корректной работы используйте скобки: 2^(3^2) вместо 2^3^2
enum class TypeLexeme
{
	number,
	variable,
	un_op,
	bin_op,
	left_bracket,
	right_bracket,
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
	error
};
bool check_arithmetic_expression(const std::string& s);
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
	else if (expression[index] == '(')
	{
		value.push_back(expression[index]);
		index++;
		return Lexeme({ TypeLexeme::left_bracket, value, Priority::bracket });
	}
	else if (expression[index] == ')')
	{
		value.push_back(expression[index]);
		index++;
		return Lexeme({ TypeLexeme::right_bracket, value, Priority::bracket });
	}
	else if (expression.substr(index, 3) == "sin" || expression.substr(index, 3) == "cos")
	{
		if (expression.substr(index, 3) == "sin")
		{
			value = "sin";
			index += 3;
			return Lexeme({ TypeLexeme::un_op, value, Priority::high });
		}
		else if (expression.substr(index, 3) == "cos")
		{
			value = "cos";
			index += 3;
			return Lexeme({ TypeLexeme::un_op, value, Priority::high });
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
			else throw "Error: invalid expression format (start lexeme is incorrect)";
		}
		else if (state == LexemeState::un_op)
		{
			if (lexeme.type == TypeLexeme::variable || lexeme.type == TypeLexeme::number)
			{
				state = LexemeState::variable;
			}
			else if (lexeme.type == TypeLexeme::un_op)
			{
				state = LexemeState::un_op;
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
			else if (lexeme.type == TypeLexeme::un_op)
			{
				state = LexemeState::un_op;
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
			else if (lexeme.type == TypeLexeme::un_op)
			{
				state = LexemeState::un_op;
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
		else if (scopeCounter < 0) throw "Error: brackets are out of order";
		else throw "error";
		res.push_back(lexeme);
	}
	return res;
}

std::vector<Lexeme> reverse_Polish_notation(const std::vector<Lexeme> &src)
{
	std::vector<Lexeme> out;
	std::stack<Lexeme> op_stack;
	for (const Lexeme &lex : src)
	{
		if (lex.type == TypeLexeme::variable || lex.type == TypeLexeme::number)
		{
			out.push_back(lex);
		}
		else if (lex.type == TypeLexeme::left_bracket || lex.type == TypeLexeme::un_op)
		{
			op_stack.push(lex);
		}
		else if (lex.type == TypeLexeme::right_bracket)
		{
			while (!op_stack.empty() && (op_stack.top().type != TypeLexeme::left_bracket))
			{
				out.push_back(op_stack.top());
				op_stack.pop();
			}
			op_stack.pop();
		}
		else if (lex.type == TypeLexeme::bin_op)
		{
			while (!op_stack.empty() && (op_stack.top().type == TypeLexeme::un_op || op_stack.top().priority >= lex.priority))
			{
				out.push_back(op_stack.top());
				op_stack.pop();
			}
			op_stack.push(lex);
		}
	}
	while (!op_stack.empty())
	{
		out.push_back(op_stack.top());
		op_stack.pop();
	}
	return out;
}

double calc_rpn(std::vector<Lexeme> &src_rpn)
{
	std::stack<double> tmp_stack;
	for (size_t i = 0; i < src_rpn.size(); ++i)
	{
		if (src_rpn[i].type == TypeLexeme::un_op)
		{
			if (src_rpn[i].value == "-")
			{
				double operand = tmp_stack.top();
				tmp_stack.pop();
				tmp_stack.push(-operand);
			}
			else if (src_rpn[i].value == "sin")
			{
				double operand = tmp_stack.top();
				tmp_stack.pop();
				tmp_stack.push(sin(operand));
			}
			else if (src_rpn[i].value == "cos")
			{
				double operand = tmp_stack.top();
				tmp_stack.pop();
				tmp_stack.push(cos(operand));
			}
		}
		else if (src_rpn[i].type == TypeLexeme::number || src_rpn[i].type == TypeLexeme::variable)
		{
			tmp_stack.push(stoi(src_rpn[i].value));
		}
		else if (src_rpn[i].type == TypeLexeme::bin_op)
		{
			double second_operand = tmp_stack.top();
			tmp_stack.pop();
			double first_operand = tmp_stack.top();
			tmp_stack.pop();
			if (src_rpn[i].value == "+")
			{
				tmp_stack.push(first_operand + second_operand);
			}
			else if (src_rpn[i].value == "-")
			{
				tmp_stack.push(first_operand - second_operand);
			}
			else if (src_rpn[i].value == "*")
			{
				tmp_stack.push(first_operand * second_operand);
			}
			else if (src_rpn[i].value == "^")
			{
				tmp_stack.push(std::pow(first_operand, second_operand));
			}
			else if (src_rpn[i].value == "/")
			{
				if (second_operand == 0) throw "Division by zero";
				tmp_stack.push(first_operand / second_operand);
			}
		}
	}
	return tmp_stack.top();
}

int main()
{
	std::string expression("-3+19^5*(-sin(x))-(-3)+18*cos(y)");
	std::vector<Lexeme> lexems = convertStrToLexemes(expression);
	std::set<char> variables;
	for (size_t i = 0; i < lexems.size(); ++i)
	{
		if (lexems[i].type == TypeLexeme::variable)
		{
			variables.insert(lexems[i].value[0]);
		}
	}
	std::set<char>::iterator it = variables.begin();
	while (it != variables.end())
	{
		double variable_value;
		std::cout << "Enter the value of " << *it << ": " << std::endl;
		std::cin >> variable_value;
		for (size_t j = 0; j < lexems.size(); ++j)
		{
			if (lexems[j].value[0] == *it)
			{
				lexems[j].value = std::to_string(variable_value);
				lexems[j].type = TypeLexeme::number;
			}
		}
		it++;
	}
	std::vector<Lexeme> RPN = reverse_Polish_notation(lexems);
	std::cout << calc_rpn(RPN);
	system("pause");
	return 0;
}

// API
bool check_arithmetic_expression(const std::string& s)
{
	return false;
}

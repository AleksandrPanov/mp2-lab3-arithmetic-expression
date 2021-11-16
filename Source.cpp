#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cmath>
using namespace std;

#define PI 3.14159265358979323846
#define EXP 2.71828182845904523536

enum class LexType {
	number,
	un_op,
	post_op,
	bin_op,
	left_scope,
	right_scope
};
enum class LexState {
	number,
	un_op,
	post_op,
	bin_op,
	left_scope,
	right_scope,
	start,
	end
};
enum class Priority {
	scope,
	number,
	plus,
	mult,
	pow,
	un_op,
	post_op
};

struct Lexeme {
	LexType type;
	string val;
	Priority priority;
};

bool isNumber(const char el) {
	return (el >= '0' && el <= '9');
}
bool isValidScopes(const vector<Lexeme> vec) {
	int scopesCounter = 0;
	for (size_t i = 0; i < vec.size(); i++)
	{
		if (vec[i].val[0] == '(') scopesCounter++;
		else if (vec[i].val[0] == ')') scopesCounter--;

		if (scopesCounter < 0) return false;
	}
	if (scopesCounter != 0) return false;
	return true;
}
int fact(int a) {
	int res = 1;
	for (size_t i = 0; i < a; i++)
		a *= (i + 1);
	return res;
}

Lexeme convertValToLexeme(const string& val, LexState state = LexState::number) {
	if (val.size() == 1) {
		char el = val[0]; 
		if (isNumber(el))
			return Lexeme{ LexType::number, val, Priority::number };
		else if (el == '*' || el == '/')
			return Lexeme{ LexType::bin_op, val, Priority::mult };
		else if (el == '^')
			return Lexeme{ LexType::bin_op, val, Priority::pow };
		else if (el == '!')
			return Lexeme{ LexType::post_op, val, Priority::post_op };
		else if (el == '+')
			return Lexeme{ LexType::bin_op, val, Priority::plus };
		else if (el == '-') {
			if (state == LexState::number || state == LexState::right_scope || state == LexState::post_op)
				return Lexeme{ LexType::bin_op, val, Priority::plus };
			else 
				return Lexeme{ LexType::un_op, val, Priority::un_op };
		}
		else if (el == 'e')
			return Lexeme{ LexType::number, to_string(EXP), Priority::number };
		else if (el == '(')
			return Lexeme{ LexType::left_scope, val, Priority::scope };
		else if (el == ')')
			return Lexeme{ LexType::right_scope, val, Priority::scope };
		else
			throw val + ": unknown const or function";
	}
	else {
		if (isNumber(val[0]))
			return Lexeme{ LexType::number, val, Priority::number };
		else if (val == "sin" || val == "cos" || val == "tg" || val == "ctg" ||
			val == "arcsin" || val == "arccos" || val == "arctg" || val == "arcctg" ||
			val == "sh" || val == "ch" || val == "th" || val == "cth" ||
			val == "arcsh" || val == "arcch" || val == "arcth" || val == "arccth" ||
			val == "sec" || val == "csc" || val == "exp" || val == "sqrt" ||
			val == "sqr" || val == "cbrt" || val == "abs" || val == "sgn" ||
			val == "ln")
			return Lexeme{ LexType::un_op, val, Priority::un_op };
		else if (val == "pi")
			return Lexeme{ LexType::number, to_string(PI), Priority::number };
		else {
			throw val + ": unknown const or function";
		}
		

	}
}
void changeState(const Lexeme lex, LexState& state) {
	switch (state)
	{
	case LexState::start:
		switch (lex.type)
		{
			case LexType::un_op:
			case LexType::number:
			case LexType::left_scope:
				state = (LexState)lex.type;
				break;
			default:
				throw "Incorrect start lexeme";
		}
		break;
	case LexState::un_op:
		switch (lex.type)
		{
			case LexType::number:
			case LexType::left_scope:
			case LexType::un_op:
				state = (LexState)lex.type;
				break;
			default:
				throw lex.val;
		}
		break;
	case LexState::post_op:
		switch (lex.type)
		{
		case LexType::bin_op:
		case LexType::right_scope:
			state = (LexState)lex.type;
			break;
		default:
			throw lex.val;
		}
		break;
	case LexState::number:
		switch (lex.type)
		{
			case LexType::bin_op:
			case LexType::right_scope:
			case LexType::post_op:
				state = (LexState)lex.type;
				break;
			default:
				throw lex.val;
		}
		break;
	case LexState::bin_op:
		switch (lex.type)
		{
			case LexType::un_op:
			case LexType::left_scope:
			case LexType::number:
				state = (LexState)lex.type;
				break;
			default:
				throw lex.val;
		}
		break;
	case LexState::left_scope:
		switch (lex.type)
		{
			case LexType::un_op:
			case LexType::left_scope:
			case LexType::number:
				state = (LexState)lex.type;
				break;
			default:
				throw lex.val;
		}
		break;
	case LexState::right_scope:
		switch (lex.type)
		{
			case LexType::bin_op:
			case LexType::right_scope:
			case LexType::post_op:
				state = (LexState)lex.type;
				break;
			default:
				throw lex.val;
		}
		break;
	default:
		throw "Something went wrong";
	}
}

vector<Lexeme> convertStrToLexemes(const string& s) {
	vector<Lexeme> res;
	LexState state = LexState::start;
	string current;
	for (size_t i = 0; i < s.size(); i++)
	{
		if (s[i] == ' ') continue;
		current.push_back(s[i]);
		if (i < s.size() - 1 && (isNumber(s[i]) && (isNumber(s[i + 1]) || s[i + 1] == '.') || (s[i] == '.' && isNumber(s[i + 1])) ))
			continue;
		if (i < s.size() - 1 && isalpha(s[i]) && isalpha(s[i + 1]))
			continue;
		
		const Lexeme lex = convertValToLexeme(current, state);
		changeState(lex, state);
		res.push_back(lex);
		current.clear();
	}
	state = LexState::end;
	if (!isValidScopes(res)) throw "Incorrect count of scopes";
	return res;
}

bool checkArithmeticExpression(const string& s) {
	try {
		vector<Lexeme> result = convertStrToLexemes(s);
	}
	catch (const char* str) {
		cout << str;
		return false;
	}
	catch (const string str) {
		cout << "Incorrect " << str << " after lexeme";
		return false;
	}
	return true;
}

const vector<Lexeme> create_reverseP(const vector<Lexeme> input) {
	vector<Lexeme> reverseP;
	vector<Lexeme> stack;
	for (const Lexeme& lex : input) {
		switch (lex.type)
		{
		case LexType::number:
		case LexType::post_op:
			reverseP.push_back(lex);
			break;
		case LexType::left_scope:
			stack.push_back(lex);
			break;
		case LexType::right_scope:
			while (stack.back().type != LexType::left_scope) {
				reverseP.push_back(stack.back());
				stack.pop_back();
			}
			stack.pop_back();
			break;
		case LexType::un_op:
			stack.push_back(lex);
			break;
		case LexType::bin_op:
			while (stack.size() > 0 && stack.back().priority >= lex.priority) {
				reverseP.push_back(stack.back());
				stack.pop_back();
			}
			stack.push_back(lex);
		default:
			break;
		}
	}
	while (stack.size() > 0) {
		reverseP.push_back(stack.back());
		stack.pop_back();
	}

	return reverseP;
}
double calculate_reverse_p(const vector<Lexeme>& reverseP) {
	vector<Lexeme> stack;
	for (const Lexeme& lex : reverseP) {
		if (lex.type == LexType::number)
			stack.push_back(lex);
		else if (lex.type == LexType::bin_op) {
			double b = stod(stack.back().val);
			stack.pop_back();
			double a = stod(stack.back().val);
			stack.pop_back();
			if (lex.val == "+")
				stack.push_back({ LexType::number, to_string(a + b), Priority::number });
			else if (lex.val == "-")
				stack.push_back({ LexType::number, to_string(a - b), Priority::number });
			else if (lex.val == "*")
				stack.push_back({ LexType::number, to_string(a * b), Priority::number });
			else if (lex.val == "/")
				stack.push_back({ LexType::number, to_string(a / b), Priority::number });
			else if (lex.val == "^")
				stack.push_back({ LexType::number, to_string(pow(a, b)), Priority::number });
		}
		else if (lex.type == LexType::un_op) {
			double a = stod(stack.back().val);
			stack.pop_back();
			if (lex.val == "-")
				stack.push_back({ LexType::number, to_string(-a), Priority::number });
			else if (lex.val == "sin")
				stack.push_back({ LexType::number, to_string(sin(a)), Priority::number });
			else if (lex.val == "cos")
				stack.push_back({ LexType::number, to_string(cos(a)), Priority::number });
			else if (lex.val == "tg")
				stack.push_back({ LexType::number, to_string(tan(a)), Priority::number });
			else if (lex.val == "ctg")
				stack.push_back({ LexType::number, to_string(1/tan(a)), Priority::number });
			else if (lex.val == "arcsin")
				stack.push_back({ LexType::number, to_string(asin(a)), Priority::number });
			else if (lex.val == "arccos")
				stack.push_back({ LexType::number, to_string(acos(a)), Priority::number });
			else if (lex.val == "arctg")
				stack.push_back({ LexType::number, to_string(atan(a)), Priority::number });
			else if (lex.val == "arcctg")
				stack.push_back({ LexType::number, to_string(PI/2 - atan(a)), Priority::number });
			else if (lex.val == "sh")
				stack.push_back({ LexType::number, to_string(sinh(a)), Priority::number });
			else if (lex.val == "ch")
				stack.push_back({ LexType::number, to_string(cosh(a)), Priority::number });
			else if (lex.val == "th")
				stack.push_back({ LexType::number, to_string(tanh(a)), Priority::number });
			else if (lex.val == "cth")
				stack.push_back({ LexType::number, to_string(1 / tanh(a)), Priority::number });
			else if (lex.val == "sec")
				stack.push_back({ LexType::number, to_string(1 / cos(a)), Priority::number });
			else if (lex.val == "csc")
				stack.push_back({ LexType::number, to_string(1 / sin(a)), Priority::number });
			else if (lex.val == "exp")
				stack.push_back({ LexType::number, to_string(pow(EXP, a)), Priority::number });
			else if (lex.val == "sqrt")
				stack.push_back({ LexType::number, to_string(pow(a, 0.5)), Priority::number });
			else if (lex.val == "sqr")
				stack.push_back({ LexType::number, to_string(pow(a, 2)), Priority::number });
			else if (lex.val == "cbrt")
				stack.push_back({ LexType::number, to_string(pow(a, 3)), Priority::number });
			else if (lex.val == "abs")
				stack.push_back({ LexType::number, to_string(abs(a)), Priority::number });
			else if (lex.val == "sgn")
				stack.push_back({ LexType::number, to_string((a > 0) - (a < 0)), Priority::number });
			else if (lex.val == "ln")
				stack.push_back({ LexType::number, to_string(log(a)), Priority::number });
		}
		else if (lex.type == LexType::post_op) {
			double a = stod(stack.back().val);
			stack.pop_back();
			stack.push_back({ LexType::number, to_string(fact(a)), Priority::number });
		}
	}
	double res = stod(stack.back().val);
	return res;
}



void calculate_expression(string& s) {
	if (checkArithmeticExpression(s)) {
		vector<Lexeme> lexemes = convertStrToLexemes(s);
		cout << calculate_reverse_p(create_reverseP(lexemes));
	}
}


int main() {
	string ex;
	cout << "Enter expression: \n\n"; 
	getline(cin, ex);
	
	cout << endl;
	calculate_expression(ex);
	cout << endl;

	return 0;
}

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cmath>
using namespace std;

vector<string> functions = { "abs", "sin", "cos", "tg", "ctg", "arcsin", "arccos", "arctg", "arcctg","ln", "e", "sqrt"};

enum class TypeLexeme
{
    no_type,
    number,
    variable,
    un_min, // унарный минус
    func,   // функция
    bin_op,
    left_bracket,
    right_bracket
};

enum class Priority
{
    no_priority,
    low,
    mid,
    hight,
    bracket
};

enum class LexemeState
{
    state_start,
    state_number,
    state_operand,
    state_un_min, // статус унарного минуса
    state_func,   // статус функции
    state_bin_op,
    state_left_bracket,
    state_right_bracket,
    state_error
};

class Lexeme
{
public:
    TypeLexeme type;
    std::string value;
    Priority priority;

    Lexeme() : type(TypeLexeme::no_type), value(), priority(Priority::no_priority) {}
    Lexeme(TypeLexeme type_, const std::string& value_, Priority priority_) : type(type_), value(value_), priority(priority_) {}
    ~Lexeme() {}

    void push_back(const char el) { value.push_back(el); }
    void push_back(const string& str) { value += str; }
    void setLexeme(TypeLexeme type_, const std::string& value_, Priority priority_) { type = type_; value = value_; priority = priority_; }

};
//не забыть удалить эту функцию
TypeLexeme getLexemeType(const char el, LexemeState state)
{
    if ((el >= '0' && el <= '9') || el == ',') // '0', '1', ..., '9', ','
        return TypeLexeme::number;
    else if ((el == 'x' && state != LexemeState::state_func) || el == 'y' || el == 'z')
        return TypeLexeme::variable;
    else if (el == '-' && (LexemeState::state_left_bracket == state || LexemeState::state_start == state))
        return TypeLexeme::un_min;
    else if (el >= 'a' && el <= 'x')
        return TypeLexeme::un_min;
    else if (el == '+' || el == '-' || el == '*' || el == '/' || el == '^')
        return TypeLexeme::bin_op;
    else if (el == '(')
        return TypeLexeme::left_bracket;
    else if (el == ')')
        return TypeLexeme::right_bracket;
    else
        throw "Invalid character in the string!";
}

int readFuncNumber(const std::string& s, int& ind)
{
    std::string str;
    char el = s[ind];
    while (el >= 'a' && el <= 'w')
    {
        str.push_back(el);
        el = s[++ind];
    }
    ind--;
    int funcNumber = -1;
    for (int i=0; i < functions.size(); i++)
    {
        if (str == functions[i])
        {
            funcNumber = i;
            break;
        }
    }
    if (funcNumber == -1)
    {
        std::string error = "FATAL ERROR. Wrong function name: ";
        error += str;
        throw error;
    }

    return funcNumber;
}

Lexeme getNextLexeme(const std::string& s, int& ind, LexemeState state)
{
    std::string value;
    char el = s[ind];
    if (el >= '0' && el <= '9') //считывание числа
    {
        const char* beginptr = &s[ind];
        char* endptr = nullptr;
        double num = strtod(beginptr, &endptr);
        value = std::to_string(num);
        ind += (static_cast<const char*>(endptr) - beginptr)-1;
        return Lexeme(TypeLexeme::number, value, Priority::no_priority);
    }
    if (el >= 'a' && el <= 'w') //считывание функции
    {
        int funcNumber = readFuncNumber(s, ind);
        value = std::to_string(funcNumber);
        return Lexeme(TypeLexeme::func, value, Priority::hight);
    }
    value.push_back(el);
    if (el == '*' || el == '/')
        return Lexeme(TypeLexeme::bin_op, value, Priority::mid);
    else if (el == '^')
        return Lexeme(TypeLexeme::bin_op, value, Priority::hight );
    else if (el == '-' && (state == LexemeState::state_left_bracket || state == LexemeState::state_start))
        return Lexeme(TypeLexeme::un_min, value, Priority::hight);
    else if (el == '+' || el == '-')
        return Lexeme(TypeLexeme::bin_op, value, Priority::low);
    else if (el == '(')
        return Lexeme(TypeLexeme::left_bracket, value, Priority::bracket);
    else if (el == ')')
        return Lexeme(TypeLexeme::right_bracket, value, Priority::bracket);
    else if (el == 'x' || el == 'y' || el == 'z')
        return Lexeme(TypeLexeme::variable, value, Priority::no_priority);
    std::string error = "FATAL ERROR. Invalid character in the string: ";
    error.push_back(el);
    throw error;
}

std::vector<Lexeme> parser(const std::string& s)
{
    std::vector<Lexeme> res;
    LexemeState state = LexemeState::state_start;
    int bracketCounter = 0;                         //  счётчик кол-ва скобок
    int ind = 0;                                    // индекс текущего элемента строки
    for (ind; ind < s.size(); ind++)
    {
        const Lexeme lexeme = getNextLexeme(s, ind, state);
        TypeLexeme lexemType = lexeme.type;

        if (state == LexemeState::state_start)
        {
            if (lexemType == TypeLexeme::un_min)
            {
                state = LexemeState::state_un_min;
            }
            else if (lexemType == TypeLexeme::func)
            {
                state = LexemeState::state_func;
            }
            else if (lexemType == TypeLexeme::variable || lexemType == TypeLexeme::number)
            {
                state = LexemeState::state_operand;
            }
            else if (lexemType == TypeLexeme::left_bracket)
            {
                state = LexemeState::state_left_bracket;
                bracketCounter++;
            }
            else throw "ERROR: wrong start lexeme.";
        }
        else if (state == LexemeState::state_un_min)
        {
            if (lexemType == TypeLexeme::variable || lexemType == TypeLexeme::number)
            {
                state = LexemeState::state_operand;
            }
            else if (lexemType == TypeLexeme::func)
            {
                state = LexemeState::state_func;
            }
            else if (lexemType == TypeLexeme::left_bracket)
            {
                state = LexemeState::state_left_bracket;
                bracketCounter++;
            }
            else throw "ERROR: Incorrect expression.";
        }
        else if (state == LexemeState::state_operand)
        {
            if (lexemType == TypeLexeme::bin_op)
            {
                state = LexemeState::state_bin_op;
            }
            else if (lexemType == TypeLexeme::right_bracket)
            {
                state = LexemeState::state_right_bracket;
                bracketCounter--;
                if (bracketCounter < 0) throw "ERROR: Incorrect brackets.";
            }
            else throw "ERROR: Incorrect expression.";
        }
        else if (state == LexemeState::state_bin_op)
        {
            if (lexemType == TypeLexeme::variable || lexemType == TypeLexeme::number)
            {
                state = LexemeState::state_operand;
            }
            else if (lexemType == TypeLexeme::func)
            {
                state = LexemeState::state_func;
            }
            else if (lexemType == TypeLexeme::left_bracket)
            {
                state = LexemeState::state_left_bracket;
                bracketCounter++;
            }
            else throw "ERROR: Incorrect expression.";
        }
        else if (state == LexemeState::state_left_bracket)
        {
            if (lexemType == TypeLexeme::left_bracket)
            {
                state = LexemeState::state_left_bracket;
                bracketCounter++;
            }
            else if (lexemType == TypeLexeme::un_min)
            {
                state = LexemeState::state_un_min;
            }
            else if (lexemType == TypeLexeme::func)
            {
                state = LexemeState::state_func;
            }
            else if (lexemType == TypeLexeme::variable || lexemType == TypeLexeme::number)
            {
                state = LexemeState::state_operand;
            }
            else throw "ERROR: Incorrect expression.";
        }
        else if (state == LexemeState::state_right_bracket)
        {
            if (lexemType == TypeLexeme::right_bracket)
            {
                state = LexemeState::state_right_bracket;
                bracketCounter--;
                if (bracketCounter < 0) throw "ERROR: Incorrect brackets.";
            }
            else if (lexemType == TypeLexeme::bin_op)
            {
                state = LexemeState::state_bin_op;
            }
            else throw "ERROR: Incorrect expression.";
        }
        else if (state == LexemeState::state_func)
        {
            if (lexemType == TypeLexeme::left_bracket)
            {
                state = LexemeState::state_left_bracket;
                bracketCounter++;
            }
            else throw "ERROR: Incorrect expression.";
        }
        else throw "Unknown error...";
        res.push_back(lexeme);
    }
    if (bracketCounter != 0) throw "ERROR: Incorrect brackets.";
    return res;
}

void setVars(std::vector<Lexeme> &vec, double x, double y, double z)
{
    for (int i=0; i< vec.size(); i++)
    {
        if (vec[i].type == TypeLexeme::variable)
        {
            vec[i].type = TypeLexeme::number;
            if (vec[i].value == "x") { vec[i].value = std::to_string(x); }
            if (vec[i].value == "y") { vec[i].value = std::to_string(y); }
            if (vec[i].value == "z") { vec[i].value = std::to_string(z); }
        }
    }
}

double calculate_reverseP(const std::vector<Lexeme>& reverseP);
const std::vector<Lexeme> create_reverseP(const vector<Lexeme>& input);

int main()
{
    try
    {
        double x = 0, y = 0, z = 0;
        string s, ans, ans2;
        bool again = true;
        cout << "Calculator v.1.0" << endl;
        cout << "*********************************************************************************" << endl;
        cout << "Program features:" << endl;
        cout << "> floating point numbers support;" << endl;
        cout << "> support for x, y, z variables;" << endl;
        cout << "> support functions:" << endl;
        cout << "abs, sin, cos, tg, ctg, arcsin, arccos, arctg, arcctg, ln, e, sqrt" << endl << endl;
        cout << "*********************************************************************************" << endl;

        while (again)
        {
            x = 0, y = 0, z = 0;
            s.clear(); 
            ans.clear();
            ans2.clear();

            cout << "Do you want to use variables x,y,z? [Yes/No]" << endl;
            cin >> ans;
            cout << "Enter the expression:" << endl;
            cin >> s;
            std::vector<Lexeme> vecLex = parser(s);
            if (ans == "Yes")
            {
                l:                
                cout << "Enter the values of ALL variables x, y, z:" << endl;
                cin >> x >> y >> z;
                setVars(vecLex, x, y, z);
            }
            else
            {
                if (s.find('x') != string::npos || s.find('y') != string::npos || s.find('z') != string::npos)
                {
                    cout<< "There are uninitialized variables.";
                    goto l;
                }
            }

            cout << endl;

            std::vector<Lexeme> rpn = create_reverseP(vecLex);
            double res = calculate_reverseP(rpn);
            cout << "= "<< res << endl;
            //string str = "-2.5*arctg(2)+sin(5*x-2.11^2+ln(456))";
            //std::vector<Lexeme> a = parser(s);

            //for (auto el : rpn)
            //{
            //    cout << el.value;
            //}
            //cout << endl;

            cout << "Do you want to calculate again? [Yes/No]" << endl;
            cin >> ans2;
            if (ans2 == "Yes") again = true;
            else again = false;
        }
    }
    catch (std::string exception)
    {
        cout << exception<<endl;
    }
    catch (const char* exception)
    {
        cout << exception << endl;
    }
    return 0;
}


const std::vector<Lexeme> create_reverseP(const vector<Lexeme>& input)
{
    std::vector<Lexeme> reverseP;
    std::vector<Lexeme> stack;

    for (const Lexeme& lex : input)
    {
        if (lex.type == TypeLexeme::number)
            reverseP.push_back(lex);
        else if (lex.type == TypeLexeme::un_min || lex.type == TypeLexeme::func)//-x, sin(x), ...
            stack.push_back(lex);
        else if (lex.type == TypeLexeme::left_bracket)
            stack.push_back(lex);
        else if (lex.type == TypeLexeme::right_bracket)
        {
            while (stack.back().type != TypeLexeme::left_bracket)
            {
                reverseP.push_back(stack.back());
                stack.pop_back();
            }
            stack.pop_back();
        }
        else if (lex.type == TypeLexeme::bin_op)
        {
            while (!stack.empty() && (stack.back().type == TypeLexeme::func || stack.back().type == TypeLexeme::un_min ||
                 (stack.back().type == TypeLexeme::bin_op && stack.back().priority >= lex.priority)))
            {
                reverseP.push_back(stack.back());
                stack.pop_back();
            }
            stack.push_back(lex);
        }
    }

    while (stack.size() > 0) // Почему цикл имеет такой вид? Потому что через for придётся идти с конца, а это нехорошо.
    {
        reverseP.push_back(stack.back());
        stack.pop_back();
    }

    return reverseP;
}

double calculate_reverseP(const std::vector<Lexeme>& reverseP)
{
    std::vector<Lexeme> stack;
    for (const Lexeme& lex : reverseP)
    {
        if (lex.type == TypeLexeme::number)
            stack.push_back(lex);
        else if (lex.type == TypeLexeme::bin_op)
        {
            Lexeme x2 = stack.back();
            stack.pop_back();
            Lexeme x1 = stack.back();
            stack.pop_back();
            double _x1 = std::atof(x1.value.c_str());//atoi(x1.value.c_str());
            double _x2 = std::atof(x2.value.c_str());//atoi(x2.value.c_str());
            if (lex.value == "+")
            {
                stack.push_back({ TypeLexeme::number, std::to_string(_x1 + _x2), Priority::no_priority });
            }
            else if (lex.value == "-")
            {
                stack.push_back({ TypeLexeme::number, std::to_string(_x1 - _x2), Priority::no_priority });
            }
            else if (lex.value == "*")
            {
                stack.push_back({ TypeLexeme::number, std::to_string(_x1 * _x2), Priority::no_priority });
            }
            else if (lex.value == "/")
            {
                if (_x2 == 0) throw "Division by zero!";
                stack.push_back({ TypeLexeme::number, std::to_string(_x1 / _x2), Priority::no_priority });
            }
            else if (lex.value == "^")
            {
                stack.push_back({ TypeLexeme::number, std::to_string(pow(_x1, _x2)), Priority::no_priority });
            }
            else throw "Wrong operation.";
        }
        else if (lex.type == TypeLexeme::un_min)
        {
            Lexeme x = stack.back();
            stack.pop_back();
            double _x = std::atof(x.value.c_str());
            stack.push_back({ TypeLexeme::number, std::to_string(-_x), Priority::no_priority });
        }
        else if (lex.type == TypeLexeme::func)
        {
            Lexeme x = stack.back();
            stack.pop_back();
            double _x = std::atof(x.value.c_str());
            int funcNum = std::atoi(lex.value.c_str());

            //"abs", "sin", "cos", "tg", "ctg", "arcsin", "arccos", "arctg", "arcctg","ln", "exp", "sqrt"
            switch (funcNum)
            {
            case 0: _x = abs(_x);
                break;
            case 1: _x = sin(_x);
                break;
            case 2: _x = cos(_x);
                break;
            case 3: _x = tan(_x);
                break;
            case 4: _x = 1/tan(_x);
                break;
            case 5: _x = asin(_x);
                break;
            case 6: _x = acos(_x);
                break;
            case 7: _x = atan(_x);
                break;
            case 8: _x = ((double)3.1415926535 / 2 - atan(_x));
                break;
            case 9: _x = log(_x);
                break;
            case 10:_x = exp(_x);
                break;
            case 11:_x = sqrt(_x);
                break;
            }
            stack.push_back({ TypeLexeme::number, std::to_string(_x), Priority::no_priority });
        }
    }
    double res = std::stod(stack.back().value);
    return res;
}

// API
bool check_arithmetic_expression(const std::string& s)
{
    return false;
}



int calculate_expression(const std::string& s)
{
    return 0;
}

#include "Expr.h"

double Expr::getRes() { return res; }

Expr::Expr(const std::string& _str) : s(_str), state(LexemeState::state_start), indx(0)
{

    try
    {
        convertStrToInfix();
        convertInfixToRPN();
        calculate();
    }
    catch (const char* exception)
    {
        throw exception;
    }
}

void Expr::convertStrToInfix()
{
    for (; indx < s.size() + 1; indx++)
    {
        Lexeme lexeme = getNextLexeme();
        getNextState(lexeme);
        exprInf.push_back(lexeme);
    }

    exprInf.pop_back();
}

Lexeme Expr::getNextLexeme()
{
    int maxNameSize = 1;
    Lexeme _lex = Error();

    for (; indx < s.size(); indx++)
    {
        if (s[indx] == ' ')
            continue;

        else
        {
            int maxNameSize = 0;

            for (auto func : allFunc.allFunc)
            {
                for (auto name : func->getAllNames())
                {
                    if (s.find(name, indx) == indx)
                    {
                        if (func->type == TypeLexeme::number)
                        {
                            _lex = { func->type, std::string(1, s[indx]), func->prior };
                            maxNameSize++;

                            for (auto digit : func->getAllNames())
                            {
                                if (std::string(1, s[indx + 1]) == digit && indx + 1 < s.size())
                                {
                                    indx++;
                                    std::string P = std::string(1, s[indx]);
                                    _lex.value += getNextLexeme().value;
                                }
                            }

                            return _lex;
                        }


                        for (int i = 0; i < func->getPossibleState().size(); i++)
                        {
                            if ((func->getPossibleState()[i] == state) && (name.size() > maxNameSize))
                            {
                                maxNameSize = name.size();
                                _lex = { func->type, func->value, func->prior };
                            }
                        }
                    }
                }
            }
            indx += maxNameSize - 1;
            return _lex;
        }
    }

    _lex.type = TypeLexeme::end;
    return _lex;
}


void Expr::getNextState(const Lexeme lexeme)
{
    if (state == LexemeState::state_start)
    {
        if (lexeme.type == TypeLexeme::un_op) {
            state = LexemeState::state_un_op;
        }

        else if (lexeme.type == TypeLexeme::variable || lexeme.type == TypeLexeme::number) {
            state = LexemeState::state_variable;
        }

        else if (lexeme.type == TypeLexeme::left_scope) {
            state = LexemeState::state_left_scope;
        }

        else if (lexeme.type == TypeLexeme::end) {
            state = LexemeState::state_end;
        }

        else throw "Error start lexeme";
    }

    else if (state == LexemeState::state_un_op)
    {
        if (lexeme.type == TypeLexeme::variable || lexeme.type == TypeLexeme::number) {
            state = LexemeState::state_variable;
        }

        else if (lexeme.type == TypeLexeme::left_scope) {
            state = LexemeState::state_left_scope;
        }

        else if (lexeme.type == TypeLexeme::un_op) {
            state = LexemeState::state_un_op;
        }

        else if (lexeme.type == TypeLexeme::left_scope) {
            state = LexemeState::state_left_scope;
        }

        else throw "Error unary operator lexeme";
    }

    else if (state == LexemeState::state_variable)
    {
        if (lexeme.type == TypeLexeme::bin_op) {
            state = LexemeState::state_bin_op;
        }

        else if (lexeme.type == TypeLexeme::right_scope) {
            state = LexemeState::state_right_scope;
        }

        else if (lexeme.type == TypeLexeme::end) {
            state = LexemeState::state_end;
        }

        else throw "Error variable lexeme";
    }

    else if (state == LexemeState::state_bin_op)
    {
        if (lexeme.type == TypeLexeme::variable || lexeme.type == TypeLexeme::number) {
            state = LexemeState::state_variable;
        }

        else if (lexeme.type == TypeLexeme::left_scope) {
            state = LexemeState::state_left_scope;
        }

        else if (lexeme.type == TypeLexeme::un_op) {
            state = LexemeState::state_un_op;
        }

        else throw "Error binary operator lexeme";
    }

    else if (state == LexemeState::state_left_scope)
    {
        if (lexeme.type == TypeLexeme::left_scope) {
            state = LexemeState::state_left_scope;
        }

        else if (lexeme.type == TypeLexeme::un_op) {
            state = LexemeState::state_un_op;
        }

        else if (lexeme.type == TypeLexeme::variable || lexeme.type == TypeLexeme::number) {
            state = LexemeState::state_variable;
        }

        else throw "Error left scope lexeme";
    }

    else if (state == LexemeState::state_right_scope)
    {
        if (lexeme.type == TypeLexeme::right_scope) {
            state = LexemeState::state_right_scope;
        }

        else if (lexeme.type == TypeLexeme::bin_op) {
            state = LexemeState::state_bin_op;
        }

        else if (lexeme.type == TypeLexeme::end) {
            state = LexemeState::state_end;
        }

        else throw "Error right scope lexeme";
    }

    else throw "error";
}

void Expr::convertInfixToRPN()
{
    for (auto lex : exprInf)
    {
        if (lex.type == TypeLexeme::number)
            reversedP.push_back(lex);

        else if (lex.type == TypeLexeme::variable)
            stack.push_back(lex);

        else if (lex.type == TypeLexeme::left_scope)
            stack.push_back(lex);

        else if (lex.type == TypeLexeme::right_scope)
        {
            while (stack.size() > 0 && stack.back().type != TypeLexeme::left_scope)
            {
                reversedP.push_back(stack.back());
                stack.pop_back();
            }

            if (stack.size() > 0 && stack.back().type == TypeLexeme::left_scope)
            {
                stack.pop_back();
            }

            else throw "The brackets are not consistent";
        }

        else if (lex.type == TypeLexeme::un_op)
            stack.push_back(lex);

        else if (lex.type == TypeLexeme::bin_op)
        {
            while (!stack.empty() && stack.back().prior >= lex.prior)
            {
                reversedP.push_back(stack.back());
                stack.pop_back();
            }
            stack.push_back(lex);
        }
    }

    while (stack.size() > 0)
    {
        Lexeme back = stack.back();
        reversedP.push_back(back);
        stack.pop_back();
        if (back.type == TypeLexeme::number || back.type == TypeLexeme::left_scope || back.type == TypeLexeme::right_scope)
            throw "The brackets are not consistent";
    }
}

void Expr::calculate()
{
    for (int i = 0; i < reversedP.size(); i++)
        doOper(reversedP[i]);

    std::string srtr = stack.back().value;
    res = std::stod(stack.back().value);
}

void Expr::doOper(Lexeme lex)
{
    for (auto func : allFunc.allFunc)
    {
        if (lex.type == func->type)
        {
            for (auto name : func->getAllNames())
            {
                if (lex.value.find(name) == 0)
                {
                    if (lex.type == TypeLexeme::number || lex.type == TypeLexeme::variable)
                    {
                        stack.push_back(lex);
                    }

                    else
                    {
                        std::vector <double> arg = { std::stod(stack.back().value) };
                        stack.pop_back();
                        
                        for (int i = func->getAmountOfArgs() - 1; i > 0; i--)
                        {
                            arg.push_back(std::stod(stack.back().value));
                            stack.pop_back();
                        }

                        std::reverse(arg.begin(), arg.end());
                        stack.push_back(Number(func->doOperation(arg)));

                    }
                    return;
                }
            }
        }
    }
}

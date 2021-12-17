#pragma once
#include <vector>
#include "enums.h"
#include "Lexeme.h"

class Expr
{
private:
    std::string s;
    std::vector<Lexeme> exprInf;
    std::vector<Lexeme> reversedP;
    std::vector<Lexeme> stack;
    double res;

    AllFunc allFunc;

    LexemeState state;
    int indx;

public:
    Expr(const std::string& _str);
    Expr() = default;

    double getRes();

private:
    Lexeme getNextLexeme();
    void getNextState(const Lexeme lexeme);
    void convertStrToInfix();
    void convertInfixToRPN();
    void calculate();
    void doOper(Lexeme lex);
};

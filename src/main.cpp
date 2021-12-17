#include <iostream>
#include <vector>
#include <string>
#include <cmath>  

#include "Enums.h"
#include "Expr.h"
#include "Lexeme.h"

bool checkExpr(const std::string& expr)
{
    try
    {
        Expr exprL(expr);
    }
    catch (const char* exception)
    {
        return false;
    }

    return true;
}

int main()
{
    std::string exprStr;

    std::cout << "Enter the expression:" << std::endl << std::endl;
    std::cin >> exprStr;

    Expr expr;

    //if (checkExpr(exprStr)) std::cout << "OK";
    //else std::cout << "NOT OK";

    try
    {
        Expr expr(exprStr);
        std::cout << " =" << std::endl << expr.getRes() << std::endl;
    }
    catch (const char* exception)
    {
        std::cerr << exception;
    }
    
    return 0;
}
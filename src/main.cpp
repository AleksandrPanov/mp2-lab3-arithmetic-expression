#include <iostream>
#include "Parser.hpp"
#include "Computer.hpp"

double countExpr(const std::string& expr)
{
    ap::Variables variables;
    variables.setVariable("x", 1);
    variables.setVariable("y", 10);
    variables.setVariable("z", 100.1);

    ap::Parser parser(expr, variables);
    auto graph = parser.parse();
    ap::Computer computer(graph, variables);
    return computer.compute();
}

void printOperations()
{
    std::cout << "Supported unary operations:\t";
    for (auto& unar : ap::Operations::Instance().getUnarys())
        std::cout << unar->getSymbols() << '\t';

    std::cout << "\nSupported binary operations:\t";
    for (auto& binar : ap::Operations::Instance().getBynarys())
        std::cout << binar->getSymbols() << '\t';
    std::cout << '\n';
}

int main()
{
    std::string expression;

    printOperations();
    std::cout << "Enter expression:\n";
    while (true)
    {
        std::getline(std::cin, expression);
        try
        {
            std::cout << countExpr(expression) << std::endl;
        }
        catch (std::exception& exception)
        {
            std::cout << exception.what() << std::endl;
        }
        std::cout << '\n';
    }

    return 0;
}

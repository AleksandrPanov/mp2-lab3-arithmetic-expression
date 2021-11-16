#pragma once 
#include "Token.hpp"
#include "Variables.hpp"
#include "Expression.hpp"
#include "Operations.hpp"

namespace ap
{
	class Computer
	{
	public:
		Computer(const Expression& exprGraphg, const Variables& variables) :
			_graph(exprGraphg),
			_variables(variables)
		{	}

		double compute()
		{
			return compute(_graph);
		}

	private:
		const Expression& _graph;
		const Variables& _variables;

		double compute(const Expression& expr)
		{
			TokenType type = expr.token.type;
			if (type == TokenType::DIGIT)
				return std::stod(expr.token.expr);
			else if (type == TokenType::VARIABLE)
				return _variables.findVariable(expr.token.expr)->second;
			else if (type == TokenType::UNARY_OPERATOR)
				return applyUnaryOperation(expr.token.expr, expr.operands[0]);
			else if (type == TokenType::BINARY_OPERATOR)
				return applyBinaryOperation(expr.token.expr, expr.operands[0], expr.operands[1]);
			else
				throw std::runtime_error("unordered TokenType");
		}

		double applyUnaryOperation(const std::string& operation, const Expression& expr)
		{
			return Operations::Instance().findUnaryBySymbols(operation)->compute(compute(expr));
		}

		double applyBinaryOperation(const std::string& operation, const Expression& expr1, const Expression& expr2)
		{
			return Operations::Instance().findBinaryBySymbols(operation)->compute(compute(expr1), compute(expr2));
		}
	};
}
#pragma once
#include <string>
#include <vector>
#include <map>
#include "Token.hpp"
#include "Variables.hpp"
#include "Expression.hpp"
#include "Operations.hpp"
#include "Exceptions.hpp"

namespace ap
{
	class Parser
	{
	public:
		Parser(const std::string& expr,
			const Variables& variables = Variables()) :
			_input(expr),
			_inputOffset(0),
			_variables(variables)
		{
			validateInput();
		}

		Expression parse()
		{
			return getBinaryOperation(0);
		}

	private:
		std::string _input;
		size_t _inputOffset;
		const Variables& _variables;

		std::string getInputLess()
		{
			return _input.substr(_inputOffset);
		}

		inline void moveInputOffset(int step)
		{
			_inputOffset += step;
		}

		void validateInput()
		{
			_input.erase(std::remove(_input.begin(), _input.end(), ' '), _input.end());
			int braceSum = 0;
			for (int i = 0; i < _input.size(); i++)
			{
				char symbol = _input[i];
				braceSum += symbol == '(';
				braceSum -= symbol == ')';
				if (braceSum < 0)
					throw UnexpectedSymbols(i, symbol);
			}
		}

		inline Token parseDigit(const std::string& expr)
		{
			Token token(TokenType::DIGIT);
			bool isDotFirstTime = true;
			for (char symbol : expr)
			{
				if (std::isdigit(symbol) || ((symbol == '.') && isDotFirstTime))
				{
					if (symbol == '.')
						isDotFirstTime = false;
					token.expr += symbol;
				}
				else
				{
					break;
				}
			}
			return token;
		}

		Token getToken(const std::string& expr, bool isBinaryFirstSearch = false)
		{
			if (expr.empty())
			{
				return TokenType::EMPTY;
			}
			else if (expr[0] == '(')
			{
				return Token(TokenType::OPEN_BRACE);
			}
			else if (expr[0] == ')')
			{
				return Token(TokenType::CLOSE_BRACE);
			}
			else if (std::isdigit(expr[0]))
			{
				return parseDigit(expr);
			}
			else
			{
				if (isBinaryFirstSearch)
					if (auto operation = Operations::Instance().findBinaryBySymbols(expr))
						return Token(TokenType::BINARY_OPERATOR, operation->getSymbols(), operation->getPriority());

				if (auto operation = Operations::Instance().findUnaryBySymbols(expr))
					return Token(TokenType::UNARY_OPERATOR, operation->getSymbols());
				
				if (!isBinaryFirstSearch)
					if (auto operation = Operations::Instance().findBinaryBySymbols(expr))
						return Token(TokenType::BINARY_OPERATOR, operation->getSymbols(), operation->getPriority());

				if (auto pair = _variables.findVariable(expr))
					return Token(TokenType::VARIABLE, pair->first);
			}

			throw UnorderedSymbols(_inputOffset, _input[_inputOffset]);
		}

		Expression getUnaryOperation()
		{
			Token token = getToken(getInputLess());
			if (token.type == TokenType::EMPTY)
			{
				throw UnexpectedSymbols(_inputOffset, "end");
			}
			else if (token.type == TokenType::OPEN_BRACE)
			{
				moveInputOffset(1);
				Expression operation = getBinaryOperation(0);
				Token closeBrace = getToken(getInputLess());

				if (closeBrace.type != TokenType::CLOSE_BRACE)
					throw ExpectedSymbols(_inputOffset, ")");
				moveInputOffset(1);
				return operation;
			}
			else if (token.type == TokenType::CLOSE_BRACE)
			{
				throw UnexpectedSymbols(_inputOffset, ")");
			}
			else if (token.type == TokenType::DIGIT ||
				token.type == TokenType::VARIABLE)
			{
				_inputOffset += token.expr.size();
				return Expression(token);
			}
			else if (token.type == TokenType::UNARY_OPERATOR)
			{
				_inputOffset += token.expr.size();
				return Expression(token, getUnaryOperation());
			}
			else if (token.type == TokenType::BINARY_OPERATOR)
			{
				throw UnexpectedSymbols(_inputOffset, _input[_inputOffset]);
			}
			throw std::runtime_error("Unknowing token type");
		}

		Expression getBinaryOperation(size_t prevPriority)
		{
			Expression left = getUnaryOperation();

			while (true)
			{
				Token token = getToken(getInputLess(), true);
				if (token.type != TokenType::BINARY_OPERATOR &&
					token.type != TokenType::CLOSE_BRACE && 
					token.type != TokenType::EMPTY)
					throw UnexpectedSymbols(_inputOffset, _input[_inputOffset]);
				if (token.priority <= prevPriority)
					return left;

				moveInputOffset(token.expr.size());
				Expression right = getBinaryOperation(token.priority);

				left = Expression(token, left, right);
			}
		}
	};
}
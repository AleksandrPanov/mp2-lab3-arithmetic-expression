#pragma once
#include <string>
#include <vector>
#include "Token.hpp"

namespace ap
{
	struct Expression
	{
		Expression(Token digit) :
			token(digit)
		{	}

		Expression(Token operation, Expression operand) :
			token(operation),
			operands{ operand }
		{	}

		Expression(Token operation, Expression operand1, Expression operand2) :
			token(operation),
			operands{ operand1, operand2 }
		{	}

		Token token;
		std::vector<Expression> operands;
	};
}
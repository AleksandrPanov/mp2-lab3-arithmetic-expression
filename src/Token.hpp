#pragma once

namespace ap
{
	enum class TokenType
	{
		EMPTY,
		DIGIT,
		OPEN_BRACE,
		CLOSE_BRACE,
		UNARY_OPERATOR,
		BINARY_OPERATOR,
		VARIABLE,
	};

	struct Token
	{
		Token(TokenType type = TokenType::EMPTY,
			const std::string& expr = std::string(),
			int priority = 0) :
			type(type),
			expr(expr),
			priority(priority)
		{	}

		TokenType type;
		std::string expr;
		size_t priority;
	};
}
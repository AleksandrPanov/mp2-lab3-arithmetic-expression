#pragma once
#include <string>
#include <stdexcept>

namespace ap
{
	class UnorderedSymbols : public std::runtime_error
	{
	public:
		UnorderedSymbols(size_t where, const std::string& ch) :
			std::runtime_error("Unordered symbols at " + std::to_string(where) + " : \"" + ch + "\"")
		{	}

		UnorderedSymbols(size_t where, char ch) :
			UnorderedSymbols(where, std::string() + ch)
		{	}
	};

	class UnexpectedSymbols : public std::runtime_error
	{
	public:
		UnexpectedSymbols(size_t where, const std::string& ch) :
			std::runtime_error("Unexpected symbols at " + std::to_string(where) + " : \"" + ch + "\"")
		{	}

		UnexpectedSymbols(size_t where, char ch) :
			UnexpectedSymbols(where, std::string() + ch)
		{	}
	};

	class ExpectedSymbols : public std::runtime_error
	{
	public:
		ExpectedSymbols(size_t where, const std::string& expectedChar) :
			std::runtime_error("Symbol \"" + std::string(expectedChar) + std::string("\" expected at ") + std::to_string(where))
		{	}
	};
}
#pragma once
#include <string>
#include <map>

namespace ap
{
	class Variables
	{
		using TVariables = std::map<std::string, double>;
	public:
		Variables(const TVariables& variables = TVariables()) :
			_variables(variables)
		{	}

		void setVariable(const std::string& var, double num)
		{
			_variables.emplace(var, num);
		}

		void eraseVariable(const std::string& var)
		{
			auto ivar = _variables.find(var);
			if (ivar != _variables.end())
				_variables.erase(ivar);
		}

		const std::pair<const std::string, double>* findVariable(const std::string& expression) const
		{
			for (auto& pair : _variables)
				if (std::strncmp(pair.first.c_str(), expression.c_str(), pair.first.size()) == 0)
					return &pair;
			return nullptr;
		}

	private:
		TVariables _variables;
	};
}
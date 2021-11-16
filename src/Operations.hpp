#pragma once 
#include <vector>

namespace ap
{
	class IUnaryOperation
	{
	public:
		virtual std::string getSymbols() = 0;
		virtual double compute(double arg) = 0;
	};

	class IBinaryOperation
	{
	public:
		virtual int getPriority() = 0;
		virtual std::string getSymbols() = 0;
		virtual double compute(double arg1, double arg2) = 0;
	};


	class UnaryMinus : public IUnaryOperation
	{
		virtual std::string getSymbols() override { return "-"; }
		virtual double compute(double arg) override { return -arg; }
	};

	class Sinus : public IUnaryOperation
	{
		virtual std::string getSymbols() override { return "sin"; }
		virtual double compute(double arg) override { return std::sin(arg); }
	};

	class Cosinus : public IUnaryOperation
	{
		virtual std::string getSymbols() override { return "cos"; }
		virtual double compute(double arg) override { return std::cos(arg); }
	};

	class BinaryPlus : public IBinaryOperation
	{
	public:
		virtual int getPriority() override { return 10; }
		virtual std::string getSymbols() override { return "+"; }
		virtual double compute(double arg1, double arg2) override { return arg1 + arg2; }
	};

	class BinaryMinus : public IBinaryOperation
	{
	public:
		virtual int getPriority() override { return 10; }
		virtual std::string getSymbols() override { return "-"; }
		virtual double compute(double arg1, double arg2) override { return arg1 - arg2; }
	};

	class Multiplication : public IBinaryOperation
	{
	public:
		virtual int getPriority() override { return 30; }
		virtual std::string getSymbols() override { return "*"; }
		virtual double compute(double arg1, double arg2) override { return arg1 * arg2; }
	};

	class Division : public IBinaryOperation
	{
	public:
		virtual int getPriority() override { return 40; }
		virtual std::string getSymbols() override { return "/"; }
		virtual double compute(double arg1, double arg2) override { return arg1 / arg2; }
	};

	class Pow : public IBinaryOperation
	{
	public:
		virtual int getPriority() override { return 50; }
		virtual std::string getSymbols() override { return "^"; }
		virtual double compute(double arg1, double arg2) override { return std::pow(arg1, arg2); }
	};

	// ƒл€ добавлени€ операции - добавить ее экземпл€р в вектор соответствующего типа операции в Operations()
	class Operations
	{
		using TUnarys = std::vector<std::unique_ptr<IUnaryOperation>>;
		using TBinarys = std::vector<std::unique_ptr<IBinaryOperation>>;
	public:
		static Operations& Instance()
		{
			static Operations operations;
			return operations;
		}

		IUnaryOperation* findUnaryBySymbols(const std::string& symbols) const
		{
			for (const auto& operation : _unarys)
				if (std::strncmp(operation->getSymbols().c_str(), symbols.c_str(), operation->getSymbols().size()) == 0)
					return operation.get();
			return nullptr;
		}

		IBinaryOperation* findBinaryBySymbols(const std::string& symbols) const
		{
			for (const auto& operation : _binarys)
				if (std::strncmp(operation->getSymbols().c_str(), symbols.c_str(), operation->getSymbols().size()) == 0)
					return operation.get();
			return nullptr;
		}

		const TUnarys& getUnarys()
		{
			return _unarys;
		}

		const TBinarys& getBynarys()
		{
			return _binarys;
		}

	private:
		TUnarys _unarys;
		TBinarys _binarys;

		Operations()
		{
			_unarys.emplace_back(new UnaryMinus());
			_unarys.emplace_back(new Sinus());
			_unarys.emplace_back(new Cosinus());

			_binarys.emplace_back(new BinaryPlus());
			_binarys.emplace_back(new BinaryMinus());
			_binarys.emplace_back(new Multiplication());
			_binarys.emplace_back(new Division());
			_binarys.emplace_back(new Pow());
		}
	};
}
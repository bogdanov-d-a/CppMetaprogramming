#pragma once

namespace MovableFunctionDetails
{

template<typename ResultType>
using FreeFunction = ResultType(*)();

template<typename ResultType>
class Holder
{
public:
	virtual ~Holder() = default;
	virtual ResultType operator()() const = 0;
};

template<typename ResultType>
class FreeFunctionHolder : public Holder<ResultType>
{
public:
	explicit FreeFunctionHolder(FreeFunction<ResultType> function)
		: m_function(function)
	{
		if (!m_function)
		{
			throw std::invalid_argument("FreeFunctionHolder: function is null");
		}
	}

	ResultType operator()() const final
	{
		return m_function();
	}

private:
	FreeFunction<ResultType> m_function = nullptr;
};

template<typename T, typename ResultType>
class TemplatedFunctionHolder : public Holder<ResultType>
{
public:
	explicit TemplatedFunctionHolder(T && function)
		: m_function(std::move(function))
	{
	}

	ResultType operator()() const final
	{
		return m_function();
	}

private:
	T m_function;
};

}

template<typename ResultType>
class MovableFunction
{
public:
	MovableFunction()
	{
	}

	explicit MovableFunction(MovableFunctionDetails::FreeFunction<ResultType> function)
		: m_holder(std::make_unique<MovableFunctionDetails::FreeFunctionHolder<ResultType>>(function))
	{
	}

	template<typename FunctionType>
	explicit MovableFunction(FunctionType && function)
		: m_holder(std::make_unique<MovableFunctionDetails::TemplatedFunctionHolder<FunctionType, ResultType>>(std::move(function)))
	{
	}

	ResultType operator()() const
	{
		if (!m_holder)
		{
			throw std::bad_function_call();
		}
		return (*m_holder)();
	}

private:
	std::unique_ptr<MovableFunctionDetails::Holder<ResultType>> m_holder;
};

#pragma once

namespace MovableFunctionDetails
{

template<typename ResultType, typename... ArgsType>
using FreeFunction = ResultType(*)(ArgsType&&...);

template<typename ResultType, typename... ArgsType>
class Holder
{
public:
	virtual ~Holder() = default;
	virtual ResultType operator()(ArgsType&&...) const = 0;
};

template<typename ResultType, typename... ArgsType>
class FreeFunctionHolder : public Holder<ResultType, ArgsType...>
{
public:
	explicit FreeFunctionHolder(FreeFunction<ResultType, ArgsType...> function)
		: m_function(function)
	{
		if (!m_function)
		{
			throw std::invalid_argument("FreeFunctionHolder: function is null");
		}
	}

	ResultType operator()(ArgsType&&... args) const final
	{
		return std::invoke(m_function, std::forward<ArgsType>(args)...);
	}

private:
	FreeFunction<ResultType, ArgsType...> m_function = nullptr;
};

template<typename T, typename ResultType, typename... ArgsType>
class TemplatedFunctionHolder : public Holder<ResultType, ArgsType...>
{
public:
	explicit TemplatedFunctionHolder(T && function)
		: m_function(std::forward<T>(function))
	{
	}

	ResultType operator()(ArgsType&&... args) const final
	{
		return std::invoke(m_function, std::forward<ArgsType>(args)...);
	}

private:
	T m_function;
};

}

template<typename ResultType, typename... ArgsType>
class MovableFunction
{
public:
	MovableFunction()
	{
	}

	explicit MovableFunction(MovableFunctionDetails::FreeFunction<ResultType, ArgsType...> function)
		: m_holder(std::make_unique<MovableFunctionDetails::FreeFunctionHolder<ResultType, ArgsType...>>(function))
	{
	}

	template<typename FunctionType>
	explicit MovableFunction(FunctionType && function)
		: m_holder(std::make_unique<MovableFunctionDetails::TemplatedFunctionHolder<FunctionType, ResultType, ArgsType...>>(std::forward<FunctionType>(function)))
	{
	}

	ResultType operator()(ArgsType&&... args) const
	{
		if (!m_holder)
		{
			throw std::bad_function_call();
		}
		return std::invoke(*m_holder, std::forward<ArgsType>(args)...);
	}

private:
	std::unique_ptr<MovableFunctionDetails::Holder<ResultType, ArgsType...>> m_holder;
};

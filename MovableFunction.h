#pragma once

namespace MovableFunctionDetails
{

typedef void(*FreeFunction)();

class Holder
{
public:
	virtual ~Holder() = default;
	virtual void operator()() const = 0;
};

class FreeFunctionHolder : public Holder
{
public:
	explicit FreeFunctionHolder(FreeFunction function)
		: m_function(function)
	{
		if (!m_function)
		{
			throw std::invalid_argument("FreeFunctionHolder: function is null");
		}
	}

	void operator()() const final
	{
		m_function();
	}

private:
	FreeFunction m_function = nullptr;
};

template<typename T>
class TemplatedFunctionHolder : public Holder
{
public:
	explicit TemplatedFunctionHolder(T function)
		: m_function(function)
	{
	}

	void operator()() const final
	{
		m_function();
	}

private:
	T m_function;
};

}

class MovableFunction
{
public:
	MovableFunction()
	{
	}

	explicit MovableFunction(MovableFunctionDetails::FreeFunction function)
		: m_holder(std::make_unique<MovableFunctionDetails::FreeFunctionHolder>(function))
	{
	}

	template<typename FunctionType>
	explicit MovableFunction(FunctionType function)
		: m_holder(std::make_unique<MovableFunctionDetails::TemplatedFunctionHolder<FunctionType>>(function))
	{
	}

	void operator()() const
	{
		if (!m_holder)
		{
			throw std::bad_function_call();
		}
		(*m_holder)();
	}

private:
	std::unique_ptr<MovableFunctionDetails::Holder> m_holder;
};

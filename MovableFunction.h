#pragma once

class MovableFunction
{
public:
	typedef void(*FreeFunction)();

	MovableFunction()
	{
	}

	explicit MovableFunction(FreeFunction f)
		: m_function(f)
	{
	}

	void operator()() const
	{
		if (!m_function)
		{
			throw std::bad_function_call();
		}
		m_function();
	}

private:
	FreeFunction m_function = nullptr;
};

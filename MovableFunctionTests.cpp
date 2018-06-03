#include "stdafx.h"
#include "MovableFunction.h"

namespace
{

bool g_freeFunctionCalled = false;

void FreeFunction()
{
	g_freeFunctionCalled = true;
}

class MinimalFunction : public boost::noncopyable
{
public:
	explicit MinimalFunction(bool &called)
		: m_called(called)
	{
	}

	explicit MinimalFunction(MinimalFunction && other)
		: m_called(other.m_called)
	{
	}

	void operator()() const
	{
		m_called = true;
	}

private:
	bool &m_called;
};

}

BOOST_AUTO_TEST_CASE(EmptyTest)
{
	MovableFunction<void> f;
	BOOST_CHECK(!g_freeFunctionCalled);

	bool thrown = false;
	try
	{
		f();
	}
	catch (std::bad_function_call&)
	{
		thrown = true;
	}

	BOOST_CHECK(thrown);
	BOOST_CHECK(!g_freeFunctionCalled);
}

BOOST_AUTO_TEST_CASE(FreeFunctionTest)
{
	MovableFunction<void> f(FreeFunction);
	BOOST_CHECK(!g_freeFunctionCalled);
	f();
	BOOST_CHECK(g_freeFunctionCalled);
}

BOOST_AUTO_TEST_CASE(LambdaTest)
{
	bool called = false;
	MovableFunction<void> f([&called]() { called = true; });
	f();
	BOOST_CHECK(called);
}

BOOST_AUTO_TEST_CASE(MinimalFunctionTest)
{
	bool called = false;
	MinimalFunction mf(called);
	MovableFunction<void> f(std::move(mf));
	BOOST_CHECK(!called);
	f();
	BOOST_CHECK(called);
}

BOOST_AUTO_TEST_CASE(BindTest)
{
	const auto add = [](int a, int b) {
		return a + b;
	};
	MovableFunction<int> f(std::bind(add, 42, 1337));
	BOOST_CHECK(f() == 1379);
}

BOOST_AUTO_TEST_CASE(ArgsTest)
{
	const auto add = [](int a, int b) {
		return a + b;
	};
	MovableFunction<int, int, int> f(add);
	BOOST_CHECK(f(42, 1337) == 1379);
}

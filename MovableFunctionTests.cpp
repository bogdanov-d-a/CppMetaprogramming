#include "stdafx.h"
#include "MovableFunction.h"

namespace
{

bool g_freeFunctionCalled = false;

void FreeFunction()
{
	g_freeFunctionCalled = true;
}

class MinimalFunction
{
public:
	explicit MinimalFunction(bool &called)
		: m_called(called)
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
	MovableFunction f;
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
	MovableFunction f(FreeFunction);
	BOOST_CHECK(!g_freeFunctionCalled);
	f();
	BOOST_CHECK(g_freeFunctionCalled);
}

BOOST_AUTO_TEST_CASE(LambdaTest)
{
	bool called = false;
	MovableFunction f([&called]() { called = true; });
	f();
	BOOST_CHECK(called);
}

BOOST_AUTO_TEST_CASE(MinimalFunctionTest)
{
	bool called = false;
	MinimalFunction mf(called);
	MovableFunction f(mf);
	BOOST_CHECK(!called);
	f();
	BOOST_CHECK(called);
}

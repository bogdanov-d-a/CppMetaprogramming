#include "stdafx.h"
#include "MovableFunction.h"

namespace
{

bool g_freeFunctionCalled = false;

void FreeFunction()
{
	g_freeFunctionCalled = true;
}

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

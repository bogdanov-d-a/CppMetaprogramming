#include "stdafx.h"
#include "FindIf.h"

using namespace std;

BOOST_AUTO_TEST_CASE(FindIndexVectorTest)
{
	vector<int> items = { 5, 4, 3, 2, 1 };
	BOOST_CHECK(FindIndex(items, [](int item) { return item == 2; }) == 3);
	BOOST_CHECK(FindIndex(items, [](int item) { return false; }) == -1);
	BOOST_CHECK(FindIndex(items, [](int item) { return true; }) == 0);
	BOOST_CHECK(FindIndex(items, [](int item) { return item == 4; }) == 1);
}

BOOST_AUTO_TEST_CASE(FindIndexSetTest)
{
	set<int> items = { 5, 4, 3, 2, 1 };
	BOOST_CHECK(FindIndex(items, [](int item) { return item == 2; }) == 1);
	BOOST_CHECK(FindIndex(items, [](int item) { return false; }) == -1);
	BOOST_CHECK(FindIndex(items, [](int item) { return true; }) == 0);
	BOOST_CHECK(FindIndex(items, [](int item) { return item == 4; }) == 3);
}

BOOST_AUTO_TEST_CASE(FindIndexListTest)
{
	forward_list<int> items = { 5, 4, 3, 2, 1 };
	BOOST_CHECK(FindIndex(items, [](int item) { return item == 2; }) == 3);
	BOOST_CHECK(FindIndex(items, [](int item) { return false; }) == -1);
	BOOST_CHECK(FindIndex(items, [](int item) { return true; }) == 0);
	BOOST_CHECK(FindIndex(items, [](int item) { return item == 4; }) == 1);
}

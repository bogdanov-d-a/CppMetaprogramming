#include "stdafx.h"
#include "FindIf.h"

namespace
{

class NonCopyableNonMovable : public boost::noncopyable
{
public:
	NonCopyableNonMovable() = default;
	NonCopyableNonMovable(NonCopyableNonMovable&&) = delete;
	NonCopyableNonMovable& operator=(NonCopyableNonMovable&&) = delete;
};

class IntegerNCM : public NonCopyableNonMovable
{
public:
	IntegerNCM(int value)
		: value(value)
	{}

	IntegerNCM(IntegerNCM&&) = delete;
	IntegerNCM& operator=(IntegerNCM&&) = delete;

	int value = 0;
};

template<typename It>
class RangeNCM : public NonCopyableNonMovable
{
public:
	RangeNCM(It begin, It end)
		: m_begin(begin)
		, m_end(end)
	{}

	It begin() const
	{
		return m_begin;
	}

	It end() const
	{
		return m_end;
	}

private:
	It m_begin;
	It m_end;
};

template<typename T>
class PredNCM : public NonCopyableNonMovable
{
public:
	explicit PredNCM(std::function<bool(T const&)> const& func)
		: m_func(func)
	{}

	bool operator()(T const& item) const
	{
		return std::invoke(m_func, item);
	}

private:
	std::function<bool(T const&)> m_func;
};

}

BOOST_AUTO_TEST_CASE(FindIndexVectorTest)
{
	const std::vector<int> items = { 5, 4, 3, 2, 1 };
	BOOST_CHECK(FindIndex(items, [](int item) { return item == 2; }) == 3);
	BOOST_CHECK(FindIndex(items, [](int item) { return false; }) == -1);
	BOOST_CHECK(FindIndex(items, [](int item) { return true; }) == 0);
	BOOST_CHECK(FindIndex(items, [](int item) { return item == 4; }) == 1);
}

BOOST_AUTO_TEST_CASE(FindIndexSetTest)
{
	const std::set<int> items = { 5, 4, 3, 2, 1 };
	BOOST_CHECK(FindIndex(items, [](int item) { return item == 2; }) == 1);
	BOOST_CHECK(FindIndex(items, [](int item) { return false; }) == -1);
	BOOST_CHECK(FindIndex(items, [](int item) { return true; }) == 0);
	BOOST_CHECK(FindIndex(items, [](int item) { return item == 4; }) == 3);
}

BOOST_AUTO_TEST_CASE(FindIndexListTest)
{
	const std::forward_list<int> items = { 5, 4, 3, 2, 1 };
	BOOST_CHECK(FindIndex(items, [](int item) { return item == 2; }) == 3);
	BOOST_CHECK(FindIndex(items, [](int item) { return false; }) == -1);
	BOOST_CHECK(FindIndex(items, [](int item) { return true; }) == 0);
	BOOST_CHECK(FindIndex(items, [](int item) { return item == 4; }) == 1);
}

BOOST_AUTO_TEST_CASE(FindIndexNoncopyableNonmovableItemTest)
{
	std::forward_list<IntegerNCM> items;
	items.emplace_front(1);
	items.emplace_front(2);
	items.emplace_front(3);
	items.emplace_front(4);
	items.emplace_front(5);
	std::forward_list<IntegerNCM> const& itemsConst = items;

	const RangeNCM<decltype(itemsConst.begin())> range(itemsConst.begin(), itemsConst.end());

	const PredNCM<IntegerNCM> pred1([](IntegerNCM const& item) { return item.value == 2; });
	const PredNCM<IntegerNCM> pred2([](IntegerNCM const& item) { return false; });
	const PredNCM<IntegerNCM> pred3([](IntegerNCM const& item) { return true; });
	const PredNCM<IntegerNCM> pred4([](IntegerNCM const& item) { return item.value == 4; });

	BOOST_CHECK(FindIndex(range, pred1) == 3);
	BOOST_CHECK(FindIndex(range, pred2) == -1);
	BOOST_CHECK(FindIndex(range, pred3) == 0);
	BOOST_CHECK(FindIndex(range, pred4) == 1);
}

BOOST_AUTO_TEST_CASE(FindPointerTest)
{
	std::forward_list<IntegerNCM> items;
	items.emplace_front(1);
	items.emplace_front(2);
	items.emplace_front(3);
	items.emplace_front(4);
	items.emplace_front(5);
	std::forward_list<IntegerNCM> const& itemsConst = items;

	const RangeNCM<decltype(itemsConst.begin())> range(itemsConst.begin(), itemsConst.end());

	const PredNCM<IntegerNCM> pred1([](IntegerNCM const& item) { return item.value == 2; });
	const PredNCM<IntegerNCM> pred2([](IntegerNCM const& item) { return false; });
	const PredNCM<IntegerNCM> pred3([](IntegerNCM const& item) { return true; });
	const PredNCM<IntegerNCM> pred4([](IntegerNCM const& item) { return item.value == 4; });

	BOOST_CHECK(FindPointer(range, pred1) == &*next(items.begin(), 3));
	BOOST_CHECK(FindPointer(range, pred2) == nullptr);
	BOOST_CHECK(FindPointer(range, pred3) == &*items.begin());
	BOOST_CHECK(FindPointer(range, pred4) == &*next(items.begin(), 1));
}

BOOST_AUTO_TEST_CASE(FindOptionalTest)
{
	std::forward_list<IntegerNCM> items;
	items.emplace_front(1);
	items.emplace_front(2);
	items.emplace_front(3);
	items.emplace_front(4);
	items.emplace_front(5);
	std::forward_list<IntegerNCM> const& itemsConst = items;

	const RangeNCM<decltype(itemsConst.begin())> range(itemsConst.begin(), itemsConst.end());

	const PredNCM<IntegerNCM> pred1([](IntegerNCM const& item) { return item.value == 2; });
	const PredNCM<IntegerNCM> pred2([](IntegerNCM const& item) { return false; });
	const PredNCM<IntegerNCM> pred3([](IntegerNCM const& item) { return true; });
	const PredNCM<IntegerNCM> pred4([](IntegerNCM const& item) { return item.value == 4; });

	BOOST_CHECK(FindOptional(range, pred1)->value == 2);
	BOOST_CHECK(FindOptional(range, pred2) == boost::none);
	BOOST_CHECK(FindOptional(range, pred3)->value == 5);
	BOOST_CHECK(FindOptional(range, pred4)->value == 4);
}

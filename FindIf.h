#pragma once

template<class Range, class Predicate>
size_t FindIndex(Range&& range, Predicate&& predicate)
{
	size_t index = 0;
	for (auto const& elem : range)
	{
		if (invoke(predicate, elem))
		{
			return index;
		}
		++index;
	}
	return -1;
}

template<class Range, class Predicate>
decltype(auto) FindPointer(Range&& range, Predicate&& predicate)
{
	for (auto const& elem : range)
	{
		if (invoke(predicate, elem))
		{
			return &elem;
		}
	}
	return static_cast<decltype(&*range.begin())>(nullptr);
}

template<class Range, class Predicate>
decltype(auto) FindOptional(Range&& range, Predicate&& predicate)
{
	using Result = boost::optional<decltype(*range.begin())>;

	for (auto const& elem : range)
	{
		if (invoke(predicate, elem))
		{
			return Result(elem);
		}
	}
	return Result();
}

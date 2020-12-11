#include <fstream>
#include <iostream>
#include <utility>

#include <range/v3/all.hpp>

auto valid_sum =
[]<ranges::common_range range_t>(range_t&& range)
{
	auto previous = range | ranges::views::drop_last(1);
	int64_t value = range.back();
	return ranges::contains(ranges::views::cartesian_product(previous, previous) | ranges::views::transform([](std::tuple<int64_t, int64_t> p) { return std::get<0>(p) + std::get<1>(p); }), value);
};

auto find_consecutive_sum =
[]<ranges::forward_range range_t>(range_t&& range, int64_t value)
{
	int64_t total = 0;
	std::forward_iterator auto begin = range.begin();
	std::forward_iterator auto end   = begin;
	do
	{
		if (total > value)
		{
			total -= *(begin++);
		}
		if (total < value)
		{
			if (end == range.end())
				break;
			total += *(end++);
		}
		if (total == value)
		{
			return ranges::subrange(begin, end);
		}
	} while (begin != end);
	return ranges::subrange{ range.end(), range.end() };
};

int main()
{
	{
		std::ifstream i("test.txt");
		auto rng = ranges::istream_view<int64_t>(i) | ranges::to<std::vector>;

		int64_t bad_value = (rng | ranges::views::sliding(5 + 1) | ranges::views::filter(std::not_fn(valid_sum))).front().back();
		std::cout << bad_value << std::endl;

		ranges::common_range auto sum_range = find_consecutive_sum(rng, bad_value);
		std::cout << ranges::min(sum_range) << "+" << ranges::max(sum_range) << "=" << ranges::min(sum_range) + ranges::max(sum_range) << std::endl;
	}
	{
		std::ifstream i("input.txt");
		auto rng = ranges::istream_view<int64_t>(i) | ranges::to<std::vector>;

		int64_t bad_value = (rng | ranges::views::sliding(25 + 1) | ranges::views::filter(std::not_fn(valid_sum))).front().back();
		std::cout << bad_value << std::endl;

		ranges::common_range auto sum_range = find_consecutive_sum(rng, bad_value);
		std::cout << ranges::min(sum_range) << "+" << ranges::max(sum_range) << "=" << ranges::min(sum_range) + ranges::max(sum_range) << std::endl;
	}

}

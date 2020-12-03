#include <fstream>
#include <iostream>
#include <utility>

#include <range/v3/all.hpp>

#include <concepts>
#include <optional>
template<std::semiregular T>
struct maybe_view : ranges::view_interface<maybe_view<T>> {
	maybe_view() = default;
	maybe_view(T t) : data_(std::move(t)) {
	}
	T const* begin() const noexcept {
		return data_ ? &*data_ : nullptr;
	}
	T const* end() const noexcept {
		return data_ ? &*data_ + 1 : nullptr;
	}
private:
	std::optional<T> data_{};
};

constexpr int total = 2020;

int main()
{
	std::ifstream i("input.txt");
	auto rng = ranges::istream_view<int>(i) | ranges::to<std::vector>;

	auto found_pairs = ranges::views::transform(rng,
		[&rng](int i)
		{
			auto it = ranges::find_if(rng,
				[i](int j)
				{
					return i + j == total;
				});
			if (it != rng.end())
			{
				return maybe_view<std::pair<int, int>>(std::pair(i, *it));
			}
			return maybe_view<std::pair<int, int>>();
		}) | ranges::views::join;

	for (auto found_pair : found_pairs)
	{
		std::cout << found_pair.first << ',' << found_pair.second << ',' << found_pair.first * found_pair.second << std::endl;
	}

	auto found_triples = ranges::views::transform(rng,
		[&rng](int i)
		{
			return ranges::views::transform(rng,
				[&rng, i](int j)
				{
					auto it = ranges::find_if(rng,
						[i, j](int k)
						{
							return i + j + k == total;
						});
					if (it != rng.end())
					{
						return maybe_view<std::tuple<int, int, int>>(std::tuple(i, j, *it));
					}
					return maybe_view<std::tuple<int, int, int>>();
				}) | ranges::views::join;
		}) | ranges::views::join;

	for (auto found_triple : found_triples)
	{
		std::cout << std::get<0>(found_triple) << ',' << std::get<1>(found_triple) << ',' << std::get<2>(found_triple) << ',' << std::get<0>(found_triple) * std::get<1>(found_triple) * std::get<2>(found_triple) << std::endl;
	}
}

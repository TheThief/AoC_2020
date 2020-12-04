#include <fstream>
#include <iostream>
#include <utility>

#include <range/v3/all.hpp>

struct row
{
	std::string s;
};

std::istream& operator>>(std::istream& is, row& r)
{
	std::getline(is, r.s);
	return is;
}

size_t count_trees(ranges::any_view<row> rows, size_t x, size_t y)
{
	auto traversed = rows | ranges::views::stride(y) | ranges::views::enumerate
		| ranges::views::transform([x](std::pair<size_t, const row&> r)
			{
				return (r.second.s | ranges::views::cycle) [r.first * x];
			});
	return ranges::count(traversed, '#');
}

int main()
{
	std::ifstream i("input.txt");
	auto rng = ranges::istream_view<row>(i) | ranges::to<std::vector>;

	std::cout << count_trees(rng, 3, 1) << std::endl;

	std::cout <<
		count_trees(rng, 1, 1) *
		count_trees(rng, 3, 1) *
		count_trees(rng, 5, 1) *
		count_trees(rng, 7, 1) *
		count_trees(rng, 1, 2)
		<< std::endl;
}

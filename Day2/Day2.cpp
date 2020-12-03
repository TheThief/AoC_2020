#include <fstream>
#include <iostream>
#include <utility>

#include <range/v3/all.hpp>

#include "stream_required.h"

struct row
{
	size_t min;
	size_t max;
	char c;
	std::string s;

	bool isvalid() const
	{
		size_t count = ranges::count(s, c);
		return count >= min && count <= max;
	}

	bool isvalid2() const
	{
		return (s[min-1] == c) + (s[max - 1] == c) == 1;
	}
};

std::istream& operator>>(std::istream& is, row& r)
{
	is >> r.min >> required{ "-" } >> r.max >> r.c >> required{ ": " } >> r.s;
	return is;
}

int main()
{
	std::ifstream i("input.txt");
	auto rng = ranges::istream_view<row>(i) | ranges::to<std::vector>;
	std::cout << ranges::count_if(rng, &row::isvalid) << std::endl;
	std::cout << ranges::count_if(rng, &row::isvalid2) << std::endl;
}

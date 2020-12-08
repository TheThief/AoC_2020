#include <fstream>
#include <iostream>
#include <utility>

#include <range/v3/all.hpp>

#include <sstream>
#include <optional>
#include <map>
#include "stream_required.h"

struct bag_contents
{
	std::map<std::string, size_t> contained;
};

std::istream& operator>>(std::istream& is, bag_contents& bc)
{
	bc = {};

	char c;
	do
	{
		is >> std::ws;
		if (is.peek() == 'n')
		{
			is >> required{ "no other bags" };
		}
		else
		{
			size_t count;
			std::string color1, color2;
			is >> count >> color1 >> color2;
			if (count == 1)
			{
				is >> required{ "bag" };
			}
			else
			{
				is >> required{ "bags" };
			}
			if (is)
			{
				bc.contained.emplace(color1 + " " + color2, count);
			}
		}
		is >> c;
	} while (is && c == ',');

	return is;
}

std::istream& operator>>(std::istream& is, std::pair<std::string, bag_contents>& b)
{
	b = {};

	std::string temp;
	if (std::getline(is, temp) && temp != "")
	{
		std::stringstream ss(temp);

		std::string color1, color2;
		ss >> color1 >> color2 >> required{ "bags contain" };
		b.first = color1 + " " + color2;
		ss >> b.second;

		if (is.eof() && !is.bad())
		{
			is.clear();
		}
	}

	return is;
}

bool bag_contains(const std::map<std::string, bag_contents>& bag_map, const std::string& current_color, const std::string& find_color)
{
	const bag_contents& contents = bag_map.at(current_color);

	return contents.contained.contains(find_color) ||
		ranges::any_of(contents.contained, [&](const std::pair< std::string, size_t>& b) { return bag_contains(bag_map, b.first, find_color); });
}

size_t total_bags_inside(const std::map<std::string, bag_contents>& bag_map, const std::string& current_color)
{
	const bag_contents& contents = bag_map.at(current_color);

	return ranges::accumulate(contents.contained | ranges::views::transform([&](const std::pair< std::string, size_t>& b) { return b.second + b.second * total_bags_inside(bag_map, b.first); }), (size_t)0);
}

int main()
{
	{
		std::ifstream i("test.txt");
		auto rng = ranges::istream_view<std::pair<std::string, bag_contents>>(i) | ranges::to<std::map>;
		std::cout << ranges::accumulate(rng | ranges::views::transform([&](const std::pair<std::string, bag_contents>& b) { return bag_contains(rng, b.first, "shiny gold"); }), 0) << std::endl;
	}
	{
		std::ifstream i("input.txt");
		auto rng = ranges::istream_view<std::pair<std::string, bag_contents>>(i) | ranges::to<std::map>;
		std::cout << ranges::accumulate(rng | ranges::views::transform([&](const std::pair<std::string, bag_contents>& b) { return bag_contains(rng, b.first, "shiny gold"); }), 0) << std::endl;
	}

	{
		std::ifstream i("test.txt");
		auto rng = ranges::istream_view<std::pair<std::string, bag_contents>>(i) | ranges::to<std::map>;
		std::cout << total_bags_inside(rng, "shiny gold") << std::endl;
	}
	{
		std::ifstream i("test2.txt");
		auto rng = ranges::istream_view<std::pair<std::string, bag_contents>>(i) | ranges::to<std::map>;
		std::cout << total_bags_inside(rng, "shiny gold") << std::endl;
	}
	{
		std::ifstream i("input.txt");
		auto rng = ranges::istream_view<std::pair<std::string, bag_contents>>(i) | ranges::to<std::map>;
		std::cout << total_bags_inside(rng, "shiny gold") << std::endl;
	}
}

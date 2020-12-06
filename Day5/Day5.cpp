#include <fstream>
#include <iostream>
#include <utility>

#include <range/v3/all.hpp>
#include <set>

struct boarding_pass
{
	std::string id;

	int to_seat_id()
	{
		return ranges::accumulate(id | ranges::views::reverse | ranges::views::enumerate |
			ranges::views::filter([](std::pair<size_t, char> p)
				{
					return p.second == 'R' || p.second == 'B';
				}) |
			ranges::views::transform([](std::pair<size_t, char> p)
				{
					return 1 << p.first;
				}), 0);
	}
};

std::istream& operator>>(std::istream& is, boarding_pass& bp)
{
	std::string id;
	std::getline(is, id);
	bp.id = id;
	return is;
}

int main()
{
	std::ifstream i("input.txt");
	auto rng = ranges::istream_view<boarding_pass>(i) | ranges::to<std::vector>;
	std::set<int> ids = ranges::views::transform(rng, &boarding_pass::to_seat_id) | ranges::to<std::set>();
	int max_seat_id = ranges::max(ids);
	std::cout <<
		max_seat_id
		<< std::endl;

	auto all_ids = ranges::views::iota(0, max_seat_id) | ranges::to<std::set>();
	auto missing_ids = all_ids | ranges::views::filter([&ids](int i)
		{
			return !ids.contains(i) && ids.contains(i - 1) && ids.contains(i + 1);
		});
	for (auto missing_id : missing_ids)
	{
		std::cout << missing_id << std::endl;
	}
}

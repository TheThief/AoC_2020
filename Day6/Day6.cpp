#include <fstream>
#include <iostream>
#include <utility>

#include <range/v3/all.hpp>

#include <set>
#include <sstream>

struct customs_decl
{
	std::vector<std::set<char>> people;

	size_t count() const
	{
		std::set<char> any_answers = people[0];
		for (auto answer_set : people | ranges::views::drop(1))
		{
			ranges::copy(answer_set, std::inserter(any_answers, any_answers.end()));
		}
		return any_answers.size();
	}

	size_t count2() const
	{
		std::set<char> all_answers = people[0];
		for (auto answer_set : people | ranges::views::drop(1))
		{
			std::set<char> current_answers = std::move(all_answers);
			ranges::set_intersection(answer_set, current_answers, std::inserter(all_answers, all_answers.end()));
		}
		return all_answers.size();
	}
};

std::istream& operator>>(std::istream& is, customs_decl& cd)
{
	cd = {};

	std::string temp;
	if (std::getline(is, temp) && temp != "")
	{
		do
		{
			std::set<char>& answers = cd.people.emplace_back();

			std::stringstream ss(temp);
			char answer;
			while (ss >> std::ws >> answer)
			{
				answers.insert(answer);
			}
		} while (std::getline(is, temp) && temp != "");
		if (is.eof() && !is.bad())
		{
			is.clear();
		}
	}

	return is;
}

int main()
{
	{
		std::ifstream i("test.txt");
		auto rng = ranges::istream_view<customs_decl>(i) | ranges::to<std::vector>;

		for (auto count : rng | ranges::views::transform(&customs_decl::count))
		{
			std::cout << count << std::endl;
		}
		std::cout << std::endl;

		for (auto count : rng | ranges::views::transform(&customs_decl::count2))
		{
			std::cout << count << std::endl;
		}
		std::cout << std::endl;
	}

	{
		std::ifstream i("input.txt");
		auto rng = ranges::istream_view<customs_decl>(i) | ranges::to<std::vector>;
		{
			size_t total = ranges::accumulate(rng | ranges::views::transform(&customs_decl::count), (size_t)0);
			std::cout << total << std::endl;
		}
		{
			size_t total2 = ranges::accumulate(rng | ranges::views::transform(&customs_decl::count2), (size_t)0);
			std::cout << total2 << std::endl;
		}
	}
}

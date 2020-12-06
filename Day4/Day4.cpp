#include <fstream>
#include <iostream>
#include <utility>

#include <range/v3/all.hpp>

#include <optional>
#include <sstream>
#include <string_view>
#include "stream_required.h"

using namespace std::literals;

std::array valid_ecl = { "amb"sv, "blu"sv, "brn"sv, "gry"sv, "grn"sv, "hzl"sv, "oth"sv };

struct passport
{
	std::optional<int> byr; // Birth Year
	std::optional<int> iyr; // Issue Year
	std::optional<int> eyr; // Expiration Year
	std::optional<std::pair<int, std::string>> hgt; // Height
	std::optional<std::string> hcl; // Hair Color
	std::optional<std::string> ecl; // Eye Color
	std::optional<std::string> pid; // Passport ID
	std::optional<std::string> cid; // Country ID

	bool isvalid() const
	{
		return
			byr.has_value() &&
			iyr.has_value() &&
			eyr.has_value() &&
			hgt.has_value() &&
			hcl.has_value() &&
			ecl.has_value() &&
			pid.has_value();
	}

	bool isvalid2() const
	{
		return
			byr.has_value() && byr >= 1920 && byr <= 2002 &&
			iyr.has_value() && iyr >= 2010 && iyr <= 2020 &&
			eyr.has_value() && eyr >= 2020 && eyr <= 2030 &&
			hgt.has_value() && ((hgt->second == "cm" && hgt->first >= 150 && hgt->first <= 193) || (hgt->second == "in" && hgt->first >= 59 && hgt->first <= 76)) &&
			hcl.has_value() && (*hcl)[0] == '#' &&
			ecl.has_value() && ranges::contains(valid_ecl, std::string_view(ecl.value())) &&
			pid.has_value() && pid->length() == 9;
	}
};

std::istream& operator>>(std::istream& is, passport& p)
{
	p = {};

	std::string temp;
	if (std::getline(is, temp) && temp != "")
	{
		do
		{
			std::stringstream ss(temp);
			std::string tag;
			while (std::getline(ss >> std::ws, tag, ':'))
			{
				std::string sval = "";
				int ival = -1;
				if (tag == "byr")
				{
					ss >> ival;
					p.byr = ival;
				}
				else if (tag == "iyr")
				{
					ss >> ival;
					p.iyr = ival;
				}
				else if (tag == "eyr")
				{
					ss >> ival;
					p.eyr = ival;
				}
				else if (tag == "hgt")
				{
					ss >> sval;
					std::string unit;
					{
						std::stringstream ss2(sval);
						ss2 >> ival;
						ss2 >> unit;
					}
					p.hgt = std::pair{ ival, unit };
				}
				else if (tag == "hcl")
				{
					ss >> sval;
					p.hcl = sval;
				}
				else if (tag == "ecl")
				{
					ss >> sval;
					p.ecl = sval;
				}
				else if (tag == "pid")
				{
					ss >> sval;
					p.pid = sval;
				}
				else if (tag == "cid")
				{
					ss >> sval;
					p.cid = sval;
				}
				else
				{
					ss.setstate(std::ios::failbit);
				}
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
		auto rng = ranges::istream_view<passport>(i) | ranges::to<std::vector>;
		std::cout << ranges::count_if(rng, &passport::isvalid) << std::endl;
	}
	{
		std::ifstream i("input.txt");
		auto rng = ranges::istream_view<passport>(i) | ranges::to<std::vector>;
		std::cout << ranges::count_if(rng, &passport::isvalid) << std::endl;
	}

	{
		std::ifstream i("test2_invalid.txt");
		auto rng = ranges::istream_view<passport>(i) | ranges::to<std::vector>;
		std::cout << ranges::count_if(rng, &passport::isvalid2) << std::endl;
	}
	{
		std::ifstream i("test2_valid.txt");
		auto rng = ranges::istream_view<passport>(i) | ranges::to<std::vector>;
		std::cout << ranges::count_if(rng, &passport::isvalid2) << std::endl;
	}
	{
		std::ifstream i("input.txt");
		auto rng = ranges::istream_view<passport>(i) | ranges::to<std::vector>;
		std::cout << ranges::count_if(rng, &passport::isvalid2) << std::endl;
	}
}

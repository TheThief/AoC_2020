#include <fstream>
#include <iostream>
#include <utility>

#include <range/v3/all.hpp>

#include <set>

struct instruction
{
	std::string opcode;
	int value;
};

std::istream& operator>>(std::istream& is, instruction& i)
{
	i = {};

	is >> i.opcode >> i.value;

	return is;
}

std::pair<size_t, int> run(std::span<const instruction> program)
{
	int accumulator = 0;
	size_t instruction_pointer = 0;
	std::set<size_t> visited;

	//while (std::pair result = visited.insert(instruction_pointer); result.second)
	while (instruction_pointer < program.size() && visited.insert(instruction_pointer).second)
	{
		const instruction& current_instruction = program[instruction_pointer];
		if (current_instruction.opcode == "acc")
		{
			accumulator += current_instruction.value;
		}
		if (current_instruction.opcode == "jmp")
		{
			instruction_pointer += current_instruction.value;
		}
		else
		{
			++instruction_pointer;
		}
	}

	return { instruction_pointer, accumulator };
}

int main()
{
	{
		std::ifstream i("test.txt");
		auto rng = ranges::istream_view<instruction>(i) | ranges::to<std::vector>;
		std::cout << run(rng).second << std::endl;
	}
	{
		std::ifstream i("input.txt");
		auto rng = ranges::istream_view<instruction>(i) | ranges::to<std::vector>;
		std::cout << run(rng).second << std::endl;

		for (std::pair<size_t, int> result :
			rng | ranges::views::enumerate | ranges::views::filter([](std::pair<size_t, const instruction&> p) { return p.second.opcode != "acc"; }) |
			ranges::views::transform(
				[&rng](std::pair<size_t, const instruction&> p)
				{
					std::vector<instruction> patched_program = rng;
					instruction& patch_instruction = patched_program[p.first];
					if (patch_instruction.opcode == "jmp")
					{
						patch_instruction.opcode = "nop";
					}
					else if (patch_instruction.opcode == "nop")
					{
						patch_instruction.opcode = "jmp";
					}
					return run(patched_program);
				}) |
			ranges::views::filter([&rng](std::pair<size_t, int> p) { return p.first >= rng.size(); }))
		{
			std::cout << result.second << std::endl;
		}
	}

}

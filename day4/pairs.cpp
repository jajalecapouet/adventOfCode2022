#include <iostream>
#include <fstream>
#include <utility>

bool	pairInPair(const std::string &line)
{
	std::pair<std::pair<int, int>, std::pair<int, int> > elves;

	std::string::size_type	idx1 = line.find('-') + 1;
	std::string::size_type	idx2 = line.find(',') + 1;
	std::string::size_type	idx3 = line.find('-', idx2) + 1;
	elves = std::make_pair(std::make_pair(std::stoi(line), std::stoi(line.substr(idx1))),\
							std::make_pair(std::stoi(line.substr(idx2)), std::stoi(line.substr(idx3))));
	if (elves.first.first < elves.second.first)
		return (elves.first.second >= elves.second.second);
	if (elves.second.first < elves.first.first)
		return (elves.second.second >= elves.first.second);
	return (elves.first.first == elves.second.first || elves.first.second == elves.second.second);
}

int main(int ac, char **av)
{
	if (ac != 2)		{return 1;}
	std::ifstream input(av[1]);
	if (input.fail())	{return 1;}
	std::string line;
	int	sum = 0;
	while (!input.eof())
	{
		line.clear();
		getline(input, line);
		sum += pairInPair(line);
	}
	std::cout << sum << '\n';
}
#include <iostream>
#include <string>
#include <set>
#include <fstream>

bool	getFile(char *av, std::string &input)
{
	std::ifstream	ifs;
	char			buff;

	input.clear();
	ifs.open(av, std::ifstream::in);
	if (ifs.fail())
		return (1);
	ifs.get(buff);
	while (ifs.good())
	{
		input.append(&buff, 1);
		ifs.get(buff);
	}
	ifs.close();
	return (0);
}

int main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cerr << "input error (" << ac << ")\n";
		return (1);
	}
	std::set<int>	elvesInventory;
	std::string input;
	if (getFile(av[1], input))
	{
		std::cerr << "coulnd't read file\n";
		return (1);
	}
	std::string::size_type	idx = 0;
	std::string::size_type	nextIdx;
	while (idx != std::string::npos)
	{
		int	calories = 0;
		nextIdx = input.find("\n\n", idx);
		calories += stoi(input.substr(idx));
		idx = input.find('\n', idx);
		while (idx != nextIdx)
		{
			++idx;
			calories += stoi(input.substr(idx));
			idx = input.find('\n', idx);
		}
		if (idx != std::string::npos)
			idx += 2;
		elvesInventory.insert(calories);
	}
	int result = 0;
	std::set<int>::reverse_iterator maxIt = elvesInventory.rbegin();
	for(int i = 0; i < 3; ++i)
		result += *maxIt++;
	std::cout << result;
	return (0);
}
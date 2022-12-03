#include <fstream>
#include <iostream>
#include <string>

int getPriority(const std::string &line)
{
	std::string::size_type	idx = 0;
	std::string::size_type	half = line.size() / 2;
	int result = 0;
	while (idx < half)
	{
		if (line.find(line[idx], half) != std::string::npos)
		{
			result = line[idx];
			if (islower(result))
				result += 1 - 'a';
			else
				result += 27 - 'A';
			return (result);
		}
		++idx;
	}
	return (result);
}

int main(int ac, char **av)
{
	if (ac != 2)
		return 1;
	
	std::ifstream input(av[1]);
	if (input.fail())
		return 1;
	
	int	sum = 0;
	while (!input.eof())
	{
		std::string line;
		getline(input, line);
		sum += getPriority(line);
	}
	input.close();
	std::cout << sum << '\n';
	return 0;
}
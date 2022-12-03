#include <fstream>
#include <iostream>
#include <string>

int	foundBadge(const std::string &line1, const std::string &line2, const std::string &line3)
{
	int	result = 0;
	for (int idx = 0; idx < line1.size(); ++idx)
	{
		if (line2.find(line1[idx]) != std::string::npos && line3.find(line1[idx]) != std::string::npos)
		{
			result = line1[idx];
			if (islower(result))
				result += 1 - 'a';
			else
				result += 27 - 'A';
			return (result);
		}
	}
	return result;
}

int getPriority(const std::string &line1, const std::string &line2, const std::string &line3)
{
	if (line1.size() <= line2.size() && line1.size() <= line3.size())
		return (foundBadge(line1, line2, line3));
	else if (line2.size() <= line1.size() && line2.size() <= line3.size())
		return (foundBadge(line2, line1, line3));
	else
		return (foundBadge(line3, line2, line1));
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
		std::string line1;
		getline(input, line1);
		std::string line2;
		getline(input, line2);
		std::string line3;
		getline(input, line3);
		sum += getPriority(line1, line2, line3);
	}
	input.close();
	std::cout << sum << '\n';
	return 0;
}
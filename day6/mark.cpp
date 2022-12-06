#include <iostream>
#include <fstream>

bool	isOk(const std::string &str)
{
	if (str.size() < 4)
		return false;
	
	for (int i = 0; i < 3; ++i)
	{
		for (int j = i + 1; j < 4; ++j)
		{
			if (*(str.rbegin() + i) == *(str.rbegin() + j))
				return false;
		}
	}
	return true;
}

int	getChars(char *av, std::string &input)
{
	std::ifstream	ifs;
	char			buff;
	int				count = 0;

	input.clear();
	ifs.open(av, std::ifstream::in);
	if (ifs.fail())
		return (1);
	ifs.get(buff);
	while (ifs.good())
	{
		count++;
		input.append(&buff, 1);
		if (isOk(input))
		{
			ifs.close();
			return (count);
		}
		ifs.get(buff);
	}
	ifs.close();
	return (0);
}

int main(int ac, char **av)
{
	if (ac != 2)
		return 1;
	
	std::string	tool;
	int result = getChars(av[1], tool);

	std::cout << result << '\n';
	return 0;
}
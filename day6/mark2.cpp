#include <iostream>
#include <fstream>

bool	checkLastChar(const std::string &str, int &count)
{
	for (int i = 1; i <= count; ++i)
	{
		if (*(str.rbegin()) == *(str.rbegin() + i))
		{
			count = i;
			return false;
		}
	}
	++count;
	if (count < 14)
		return false;
	return true;
}

int	getChars(char *av)
{
	std::ifstream	ifs;
	char			buff;
	int				count = 0;

	std::string	tool;
	ifs.open(av, std::ifstream::in);
	if (ifs.fail())
		return (1);
	ifs.get(buff);
	while (ifs.good())
	{
		tool.append(&buff, 1);
		if (checkLastChar(tool, count))
		{
			ifs.close();
			return (tool.size());
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
	
	int result = getChars(av[1]);
	if (result == 1)
		return 1;

	if (!result)
		std::cout << "No mark found in this array\n";
	else
		std::cout << "Mark found at position " << result << ".\n";
	return 0;
}
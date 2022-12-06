#include <fstream>
#include <iostream>

int	getLoose(int n)
{
	switch (n)
	{
		case 1 : return (3);
		case 2 : return (1);
		default : return (2);
	}
}

int	getWin(int n)
{
	switch (n)
	{
		case 1 : return (2);
		case 2 : return (3);
		default : return (1);
	}
}

int	scoreCalculator(char *str)
{
	int	score = 1 + str[2] - 'X';
	int	other = 1 + str[0] - 'A';
	
	switch (score)
	{
		case 1 : return (getLoose(other));
		case 2 : return (other + 3);
		default : return (getWin(other) + 6);
	}
}

int main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cerr << "input error\n";
		return (1);
	}

	std::ifstream input(av[1]);
	if (input.fail())
	{
		std::cerr << "couldn't read file\n";
		return (1);
	}

	int	sum = 0;
	while (!input.eof())
	{
		char	line[4];
		input.getline(line, 5);
		sum += scoreCalculator(line);
	}
	input.close();
	std::cout << sum << '\n';
	return (0);
}
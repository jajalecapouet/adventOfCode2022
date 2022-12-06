#include <fstream>
#include <iostream>

int	scoreCalculator(char *str)
{
	int	score = 1 + str[2] - 'X';
	int	other = 1 + str[0] - 'A';
	
	if (score == other)
		return (score + 3);
	switch (score)
	{
		case 1 :
			if (other == 2)
				return (score);
			return (score + 6);
		case 2 :
			if (other == 1)
				return (score + 6);
			return (score);
		default :
			if (other == 1)
				return score;
			return (score + 6);	
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
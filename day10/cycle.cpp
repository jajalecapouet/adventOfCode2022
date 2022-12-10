#include <iostream>
#include <fstream>
#include <string>

int main(int ac, char **av)
{
	if (ac != 2) return 1;
	std::ifstream input(av[1]);
	if (input.fail()) return 1;

	std::string line;
	int		endLoop = 20;
	int		currentCycle = 0;
	int		xRegister = 1;
	int		sum = 0;
	bool	addrInstructionTime = 0;
	while (!input.eof() && currentCycle <= 220)
	{
		if (!endLoop)
		{
			endLoop = 40;
			sum += currentCycle * xRegister;
		}
		--endLoop;
		if (!addrInstructionTime && line[0] != 'n')
			xRegister += atoi(&line[5]);
		if (addrInstructionTime)
			addrInstructionTime = false;
		else
		{
			getline(input, line);
			if (line.back() == 13) line.erase(--line.end());
			if (line[0] == 'a')
				addrInstructionTime = true;
		}
		++currentCycle;
	}
	if (input.eof() && currentCycle < 220)
		std::cout << "file ended before encounter all the wanted instructions.\n";
	std::cout << sum << '\n';
	return 0;
}
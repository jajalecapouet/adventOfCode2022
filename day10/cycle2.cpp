#include <iostream>
#include <fstream>
#include <string>
#include <vector>

void	displayCRT(const std::vector<std::vector<char> >	&CRT)
{
	std::vector<std::vector<char> >::const_iterator	endR = CRT.end();
	for (std::vector<std::vector<char> >::const_iterator itR = CRT.begin(); itR != endR; ++itR)
	{
		std::vector<char>::const_iterator	endC = itR->end();
		for (std::vector<char>::const_iterator	itC = itR->begin(); itC != endC; ++itC)
			std::cout << *itC;
		std::cout << '\n';
	}
}

bool	isBetween(int n, int a)
{
	int	spriteBegin = a - 1;
	int	spriteEnd = a + 1;
	return (n >= spriteBegin && n <= spriteEnd);
}

int main(int ac, char **av)
{
	if (ac != 2) return 1;
	std::ifstream input(av[1]);
	if (input.fail()) return 1;

	std::string line;
	std::vector<std::vector<char> >	CRT(6, std::vector<char>(40, '.'));
	int		endLine = 0;
	int		lineCRT = 0;
	int		currentCycle = 0;
	int		xRegister = 1;
	bool	addrInstructionTime = 0;
	while (!input.eof() && ++currentCycle <= 240)
	{
		if (endLine >= 40)
		{
			endLine = 0;
			++lineCRT;
		}
		if (!addrInstructionTime && line[0] != 'n')
			xRegister += atoi(&line[5]);
		if (isBetween(endLine, xRegister))
			CRT[lineCRT][endLine] = '#';
		if (addrInstructionTime)
			addrInstructionTime = false;
		else
		{
			getline(input, line);
			if (line.back() == 13) line.erase(--line.end());
			if (line[0] == 'a')
				addrInstructionTime = true;
		}
		++endLine;
	}
	if (input.eof() && currentCycle < 240)
		std::cout << "file ended before encounter all the wanted instructions.\n";
	displayCRT(CRT);
	return 0;
}
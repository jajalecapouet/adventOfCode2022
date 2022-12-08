#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <utility>

int	countFromTop(std::vector<std::vector<std::pair<short, bool> > > &grid)
{
	int																count = 0;
	std::vector<std::vector<std::pair<short, bool> > >::iterator	endR(grid.end());

	for (int idx = 0; idx < grid[0].size(); ++idx) {
		short	taller = -1;
		for (std::vector<std::vector<std::pair<short, bool> > >::iterator itR(grid.begin()); itR != endR; ++itR)
		{
			if (taller == 9)
				break;
			if (itR->at(idx).first > taller)
			{
				taller = itR->at(idx).first;
				if (!itR->at(idx).second) {
					++count;
					itR->at(idx).second = true;
				}
			}
		}
	}
	return	count;
}

int	countFromBottom(std::vector<std::vector<std::pair<short, bool> > > &grid)
{
	int																count = 0;
	std::vector<std::vector<std::pair<short, bool> > >::reverse_iterator	endR(grid.rend());

	for (int idx = 0; idx < grid[0].size(); ++idx) {
		short	taller = -1;
		for (std::vector<std::vector<std::pair<short, bool> > >::reverse_iterator itR(grid.rbegin()); itR != endR; ++itR)
		{
			if (taller == 9)
				break;
			if (itR->at(idx).first > taller)
			{
				taller = itR->at(idx).first;
				if (!itR->at(idx).second) {
					++count;
					itR->at(idx).second = true;
				}
			}
		}
	}
	return	count;
}

int	countFromLeft(std::vector<std::vector<std::pair<short, bool> > > &grid)
{
	int																count = 0;
	std::vector<std::vector<std::pair<short, bool> > >::iterator	endR(grid.end());

	for (std::vector<std::vector<std::pair<short, bool> > >::iterator itR(grid.begin()); itR != endR; ++itR)
	{
		short	taller = -1;
		for (int idx = 0; idx < grid[0].size(); ++idx) {
			if (taller == 9)
				break;
			if (itR->at(idx).first > taller)
			{
				taller = itR->at(idx).first;
				if (!itR->at(idx).second) {
					++count;
					itR->at(idx).second = true;
				}
			}
		}
	}
	return	count;
}

int	countFromRight(std::vector<std::vector<std::pair<short, bool> > > &grid)
{
	int																count = 0;
	std::vector<std::vector<std::pair<short, bool> > >::iterator	endR(grid.end());

	for (std::vector<std::vector<std::pair<short, bool> > >::iterator itR(grid.begin()); itR != endR; ++itR)
	{
		short	taller = -1;
		for (int idx = grid[0].size() - 1; idx >= 0; --idx) {
			if (taller == 9)
				break;
			if (itR->at(idx).first > taller)
			{
				taller = itR->at(idx).first;
				if (!itR->at(idx).second) {
					++count;
					itR->at(idx).second = true;
				}
			}
		}
	}
	return	count;
}

int	countVisibleTrees(std::vector<std::vector<std::pair<short, bool> > > &grid)
{
	int count = 0;

	count += countFromTop(grid);
	count += countFromRight(grid);
	count += countFromBottom(grid);
	count += countFromLeft(grid);
	return (count);
}

int main(int ac, char **av)
{
	if (ac != 2)
		return 1;
	std::ifstream	input(av[1]);
	if (input.fail())
		return 1;

	std::vector<std::vector<std::pair<short, bool> > >	grid;
	std::string							line;
	while (!input.eof())
	{
		std::vector<std::pair<short, bool > >	row;
		getline(input, line);
		if (line.back() == 13) // carriage return deletion if it exists
			line.erase(--line.end());
		std::string::iterator end(line.end());
		for (std::string::iterator it = line.begin(); it != end; ++it)
			row.push_back(std::make_pair((*it) - '0', false));
		grid.push_back(row);
	}
	std::cout << countVisibleTrees(grid) << '\n';
	return (0);
}
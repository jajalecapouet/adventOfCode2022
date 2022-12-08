#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

int	upRange(std::vector<std::vector<short> >::const_iterator itR, int idx, const std::vector<std::vector<short> > &grid)
{
	int	count = 0;
	int	compareTo = itR->at(idx);
	for (std::vector<std::vector<short> >::const_reverse_iterator it(itR); it != grid.rend(); ++it)
	{
		++count;
		if (it->at(idx) >= compareTo)
			return (count);
	}
	return (count);
}

int	downRange(std::vector<std::vector<short> >::const_iterator itR, int idx, const std::vector<std::vector<short> > &grid)
{
	int	count = 0;
	int	compareTo = itR->at(idx);
	for (std::vector<std::vector<short> >::const_iterator it(++itR); it != grid.end(); ++it)
	{
		++count;
		if (it->at(idx) >= compareTo)
			return (count);
	}
	return (count);
}

int	rightRange(std::vector<std::vector<short> >::const_iterator itR, int idx, const std::vector<std::vector<short> > &grid)
{
	int	count = 0;
	int	compareTo = itR->at(idx);
	for (int it = ++idx; it < grid[0].size(); ++it)
	{
		++count;
		if (itR->at(it) >= compareTo)
			return (count);
	}
	return (count);
}

int	leftRange(std::vector<std::vector<short> >::const_iterator itR, int idx)
{
	int	count = 0;
	int	compareTo = itR->at(idx);
	for (int it = --idx; it >= 0; --it)
	{
		++count;
		if (itR->at(it) >= compareTo)
			return (count);
	}
	return (count);
}

int	calculateView(std::vector<std::vector<short> >::const_iterator itR, int idx, const std::vector<std::vector<short> > &grid)
{
	int	up = upRange(itR, idx, grid);
	if (!up)
		return 0;
	int	right = rightRange(itR, idx, grid);
	if (!right)
		return 0;
	int	down = downRange(itR, idx, grid);
	if (!down)
		return 0;
	int	left = leftRange(itR, idx);
	return (up * right * down * left);
}

int	betterView(const std::vector<std::vector<short> > &grid)
{
	int betterView = 0;

	std::vector<std::vector<short > >::const_iterator	endR(grid.end());
	for (std::vector<std::vector<short> >::const_iterator itR(grid.begin()); itR != endR; ++itR)
	{
		for (int idx = 0; idx < grid[0].size(); ++idx)
			betterView = std::max(betterView, calculateView(itR, idx, grid));
	}
	return (betterView);
}

int main(int ac, char **av)
{
	if (ac != 2)
		return 1;
	std::ifstream	input(av[1]);
	if (input.fail())
		return 1;

	std::vector<std::vector<short> >	grid;
	std::string							line;
	while (!input.eof())
	{
		std::vector<short>	row;
		getline(input, line);
		if (line.back() == 13) // carriage return deletion if it exists
			line.erase(--line.end());
		std::string::iterator end(line.end());
		for (std::string::iterator it = line.begin(); it != end; ++it)
			row.push_back((*it) - '0');
		grid.push_back(row);
	}
	std::cout << betterView(grid) << '\n';
	return (0);
}
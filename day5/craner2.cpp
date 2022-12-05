#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

void	reverseAll(std::vector<char> &stack)
{
	std::reverse(stack.begin(), stack.end());
}

bool	doTheCrates(const std::string &line, std::vector<std::vector<char> > &boat)
{
	if (line[1] == '1')
	{
		for_each(boat.begin(), boat.end(), reverseAll);
		return 1;
	}
	std::string::size_type	idx = 0;
	idx = line.find('[');
	while (idx != std::string::npos)
	{
		while ((idx + 1) / 4  + 1 > boat.size())
			boat.push_back(std::vector<char>());
		boat[(idx / 4)].push_back(line[idx + 1]);
		idx = line.find('[', idx + 1);
	}
	return 0;
}

void getTheTops(std::vector<std::vector<char> > &boat)
{
	for (int i = 0; i < boat.size(); ++i)
	{
		std::cout << boat[i].back();
	}
	std::cout << '\n';
}

void mooveCrates(const std::string &line, std::vector<std::vector<char> > &boat)
{
	if (line.empty())
		return;
	
	int	move = std::stoi(line.substr(line.find('m') + 5));
	int	from = std::stoi(line.substr(line.find('f') + 5)) - 1;
	int	to = std::stoi(line.substr(line.find('t') + 3)) - 1;

	boat[to].insert(boat[to].end(), boat[from].end() - move, boat[from].end());
	boat[from].erase(boat[from].end() - move, boat[from].end());
}

int	main(int ac, char **av)
{
	if (ac != 2) {return 1;}
	std::ifstream	input(av[1]);
	if (input.fail()) {return 1;}

	std::vector<std::vector<char> >	boat;
	std::string	line;
	bool	parsed = false;
	while (!input.eof())
	{
		line.clear();
		getline(input, line);
		if (!parsed)
			parsed = doTheCrates(line, boat);
		else
			mooveCrates(line, boat);
	}
	getTheTops(boat);
	return (0);
}
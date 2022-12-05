#include <iostream>
#include <fstream>
#include <vector>
#include <deque>

bool	doTheCrates(const std::string &line, std::vector<std::deque<char> > &boat)
{
	if (line[1] == '1')
		return 1;
	std::string::size_type	idx = 0;
	idx = line.find('[');
	while (idx != std::string::npos)
	{
		while ((idx + 1) / 4  + 1 > boat.size())
			boat.push_back(std::deque<char>());
		boat[(idx / 4)].push_front(line[idx + 1]);
		idx = line.find('[', idx + 1);
	}
	return 0;
}

void getTheTops(std::vector<std::deque<char> > &boat)
{
	for (int i = 0; i < boat.size(); ++i)
	{
		std::cout << boat[i].back();
	}
	std::cout << '\n';
}

void mooveCrates(const std::string &line, std::vector<std::deque<char> > &boat)
{
	if (line.empty())
		return;
	
	int	move = std::stoi(line.substr(line.find('m') + 5));
	int	from = std::stoi(line.substr(line.find('f') + 5)) - 1;
	int	to = std::stoi(line.substr(line.find('t') + 3)) - 1;

	while (move--)
	{
			boat[to].push_back(boat[from].back());
			boat[from].pop_back();
	}
}

int	main(int ac, char **av)
{
	if (ac != 2) {return 1;}
	std::ifstream	input(av[1]);
	if (input.fail()) {return 1;}

	std::vector<std::deque<char> >	boat;
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
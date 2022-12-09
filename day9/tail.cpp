#include <set>
#include <utility>
#include <iostream>
#include <fstream>
#include <unistd.h>

std::pair<int, int>	&operator+=(std::pair<int, int> &lhs, const std::pair<int, int> &rhs)
{
	lhs.first += rhs.first;
	lhs.second += rhs.second;
	return (lhs);
}

void	setMoove(const std::string &line, std::pair<std::pair<int, int>, int> &move)
{
	move.first.first = 0;
	move.first.second = 0;
	if (!line.size())
		return ;
	move.second = std::stoi(line.substr(2));
	switch (line.front())
	{
		case 'U' : 
			move.first.second = 1;
			break;
		case 'R' :
			move.first.first = 1;
			break;
		case 'D' :
			move.first.second = -1;
			break;
		default :
			move.first.first = -1;
	}
}

std::pair<int, int> stickOnHead(const std::pair<int, int> &head, const std::pair<int, int> &tail)
{
	std::pair<int, int>	distance = std::make_pair(head.first - tail.first, head.second - tail.second);
	if (distance.first == 2)
		return (std::make_pair(1, distance.second));
	if (distance.first == -2)
		return (std::make_pair(-1, distance.second));
	if (distance.second == 2)
		return (std::make_pair(distance.first, 1));
	if (distance.second == -2)
		return (std::make_pair(distance.first, -1));
	return (std::make_pair(0, 0));
}

void	executeMove(std::pair<int, int> &head, std::pair<int, int> &tail, std::pair<std::pair<int, int>, int> &move, std::set<std::pair<int, int> > &pathing)
{
	while (move.second--)
	{
		head += move.first;
		tail += stickOnHead(head, tail);
		pathing.insert(tail);
	}
}

int main(int ac, char **av)
{
	if (ac != 2) return 1;
	std::ifstream input(av[1]);
	if (input.fail()) return 1;

	std::string							line;
	std::set<std::pair<int, int> >		tailPathing;
	std::pair<int, int>					lastTailPosition = std::make_pair(0, 0);
	std::pair<int, int>					lastHeadPosition = std::make_pair(0, 0);
	std::pair<std::pair<int, int>, int> move;

	tailPathing.insert(lastTailPosition);
	while (!input.eof())
	{
		getline(input, line);
		if (line.back() == 13) line.erase(--line.end());
		setMoove(line, move);
		executeMove(lastHeadPosition, lastTailPosition, move, tailPathing);
	}
	std::cout << tailPathing.size() << '\n';
}
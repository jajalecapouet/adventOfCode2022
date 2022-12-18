#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <set>
#include <map>
#include <cmath>

class Coord
{
public:
	int x;
	int	y;
	int z;

	Coord() : x(0), y(0), z(0) {}
	Coord(const Coord &o) : x(o.x), y(o.y), z(o.z) {}
	Coord(int x_, int y_, int z_) : x(x_), y(y_), z(z_) {}
	~Coord() {}
	Coord &operator=(const Coord &o)
	{
		x = o.x;
		y = o.y;
		z = o.z;
		return *this;
	}

	Coord	&operator+=(const Coord &o)
	{
		x += o.x;
		y += o.y;
		z += o.z;
		return *this;
	}

	Coord	&operator-=(const Coord &o)
	{
		x -= o.x;
		y -= o.y;
		z -= o.z;
		return *this;
	}

	Coord	operator+(const Coord &o)	const
	{
		return (Coord(*this) += o);
	}

	Coord	operator-(const Coord &o)	const
	{
		return (Coord(*this) -= o);
	}

	bool operator<(const Coord &o)	const
	{
		if (z == o.z)
		{
			if (y == o.y)
			{
				return (x < o.x);
			}
			return (y < o.y);
		}
		return (z < o.z);
	}

	int ManhattanDistance(const Coord &o)
	{
		return (abs(x - o.x) + abs(y - o.y) + abs(z - o.z));
	}

};

std::ostream &operator<<(std::ostream &o, const Coord &c)
{
	o << c.x << '|' << c.y << '|' << c.z;
	return o;
}

void	parseLine(const std::string &line, Coord &drop)
{
	static int i = 1;
	std::string::size_type idx1 = line.find(',');
	std::string::size_type idx2 = line.find(',', idx1 + 1);

	if (idx1 == std::string::npos || idx2 == std::string::npos) {
		std::string errMsg = "incorrect input at line ";
		errMsg += std::to_string(i);
		errMsg += ".\n";
		throw (std::invalid_argument(errMsg));
	}
	drop.x = atoi(&line[0]);
	drop.y = atoi(&line[idx1 + 1]);
	drop.z = atoi(&line[idx2 + 1]);
	++i;
}

void insertDrop(Coord &drop, std::map<Coord, int> &drops)
{
	int faces = 6;
	std::map<Coord, int>::iterator	found = drops.find(drop);
	if (found != drops.end())
		throw (std::invalid_argument("input has doubles: abort\n"));
	found = drops.find(drop - Coord(1, 0, 0));
	if (found != drops.end())
	{
		--faces;
		--(found->second);
	}
	found = drops.find(drop + Coord(1, 0, 0));
	if (found != drops.end())
	{
		--faces;
		--(found->second);
	}
	found = drops.find(drop - Coord(0, 1, 0));
	if (found != drops.end())
	{
		--faces;
		--(found->second);
	}
	found = drops.find(drop + Coord(0, 1, 0));
	if (found != drops.end())
	{
		--faces;
		--(found->second);
	}
	found = drops.find(drop - Coord(0, 0, 1));
	if (found != drops.end())
	{
		--faces;
		--(found->second);
	}
	found = drops.find(drop + Coord(0, 0, 1));
	if (found != drops.end())
	{
		--faces;
		--(found->second);
	}
	drops.insert(std::make_pair(drop, faces));
}

int main(int ac, char **av)
{
	if (ac != 2) return 1;
	std::ifstream input(av[1]);
	if (input.fail())
	{
		std::cerr << "input fail\n";
		return 1;
	}

	std::map<Coord, int>	drops;
	std::string line;
	while (!input.eof())
	{
		getline(input, line);
		if (line.back() == 13) line.erase(--line.end());
		Coord	drop;
		parseLine(line, drop);
		insertDrop(drop, drops);
	}
	int result = 0;
	for (std::map<Coord, int>::const_iterator cit = drops.begin(); cit != drops.end(); ++cit)
		result += cit->second;
	std::cout << result << " is result\n";
	return 0;
}
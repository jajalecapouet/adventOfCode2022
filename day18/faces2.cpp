#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <set>
#include <map>
#include <cmath>
#include <algorithm>

#define	TOP 0 // if touched with    0/-1/0
#define RIGHT 1 // if touched with  -1/0/0
#define BOTTOM 2 // if touched with 0/1/0
#define	LEFT 3 // if touched with   1/0/0
#define	BEHIND 4 // if touched with 0/0/1
#define AHEAD 5 // if touched with  0/0/-1

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

	int ManhattanDistance(const Coord &o)	const
	{
		return (abs(x - o.x) + abs(y - o.y) + abs(z - o.z));
	}

	int norm()	const
	{
		return ManhattanDistance(Coord(0, 0, 0));
	}

	bool isPositive()	const
	{
		return (x >= -1 && y >= -1 && z >= -1);
	}
};

typedef std::set<std::pair<Coord, int> > Faces;

std::ostream &operator<<(std::ostream &o, const Coord &c)
{
	o << c.x << '|' << c.y << '|' << c.z;
	return o;
}

void	parseLine(const std::string &line, Coord &drop, int &maxSize)
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
	int x = atoi(&line[0]);
	maxSize = std::max(maxSize, x);
	int y = atoi(&line[idx1 + 1]);
	maxSize = std::max(maxSize, y);
	int z = atoi(&line[idx2 + 1]);
	maxSize = std::max(maxSize, z);
	drop.x = x;
	drop.y = y;
	drop.z = z;
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

void expandSteam(const Coord &c, const std::map<Coord, int> &map, std::set<Coord> &visited, Faces &touched, int sizeMax)
{

	Coord	pX(c + Coord(1, 0, 0)); //plus X
	Coord	mX(c - Coord(1, 0, 0)); //minus X
	Coord	pY(c + Coord(0, 1, 0)); //plus Y
	Coord	mY(c - Coord(0, 1, 0)); //minus Y
	Coord	pZ(c + Coord(0, 0, 1)); //plus Z
	Coord	mZ(c - Coord(0, 0, 1)); //minus Z
	std::pair<std::set<Coord>::iterator, bool>	insertResult;

	if (pX.x < sizeMax)
	{
		if (map.find(pX) != map.end()) {
			touched.insert(std::make_pair(pX, LEFT));
		}
		else
		{
			insertResult = visited.insert(pX);
			if (insertResult.second)
				expandSteam(pX, map, visited, touched, sizeMax);
		}
	}

	if (mX.isPositive())
	{
		if (map.find(mX) != map.end())
			touched.insert(std::make_pair(mX, RIGHT));
		else
		{
			insertResult = visited.insert(mX);
			if (insertResult.second)
				expandSteam(mX, map, visited, touched, sizeMax);
		}
	}

	if (pY.y < sizeMax)
	{
		if (map.find(pY) != map.end())
			touched.insert(std::make_pair(pY, BOTTOM));
		else
		{
			insertResult = visited.insert(pY);
			if (insertResult.second)
				expandSteam(pY, map, visited, touched, sizeMax);
		}
	}

	if (mY.isPositive())
	{
		if (map.find(mY) != map.end())
			touched.insert(std::make_pair(mY, TOP));
		else
		{
			insertResult = visited.insert(mY);
			if (insertResult.second)
				expandSteam(mY, map, visited, touched, sizeMax);
		}
	}

	if (pZ.z < sizeMax)
	{
		if (map.find(pZ) != map.end())
			touched.insert(std::make_pair(pZ, BEHIND));
		else
		{
			insertResult = visited.insert(pZ);
			if (insertResult.second)
				expandSteam(pZ, map, visited, touched, sizeMax);
		}
	}

	if (mZ.isPositive())
	{
		if (map.find(mZ) != map.end())
			touched.insert(std::make_pair(mZ, AHEAD));
		else
		{
			insertResult = visited.insert(mZ);
			if (insertResult.second)
				expandSteam(mZ, map, visited, touched, sizeMax);
		}
	}
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
	int maxSize = 0;
	while (!input.eof())
	{
		getline(input, line);
		if (line.back() == 13) line.erase(--line.end());
		Coord	drop;
		parseLine(line, drop, maxSize);
		insertDrop(drop, drops);
	}
	Faces	touched;
	std::set<Coord>	visited;
	Coord	start(0, 0, 0);
	visited.insert(start);
	expandSteam(start, drops, visited, touched, maxSize + 2);
	std::cout << touched.size() << " is result " << visited.size() << " is units of steam spread\n";
	return 0;
}
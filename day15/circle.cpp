#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <utility>
#include <cmath>
#include <exception>
#include <algorithm>
#include <set>

typedef std::vector<std::vector<char> > grid;
typedef std::pair<int, int>	coord;

std::ostream &operator<<(std::ostream &o, const coord &c)
{
	o << c.first << '|' << c.second;
	return o;
}

std::ostream &operator<<(std::ostream &o, const grid &g)
{
	for (grid::const_iterator cit = g.begin(); cit != g.end(); ++cit)
	{
		for (std::vector<char>::const_iterator cit2 = cit->begin(); cit2 != cit->end(); ++cit2)
			o << *cit2;
		o << 'n';
	}
	return o;
}

int manhattanNorm(const coord &a, const coord &b)
{
	return (abs(a.first - b.first) + abs(a.second - b.second));
}

class Circle
{
public :
	coord	_coord;
	int 	_size;

	//Coplien constructors
	Circle() : _coord(coord()), _size(0) {}
	Circle(const Circle &other) : _coord(other._coord), _size(other._size) {}
	~Circle() {}
	Circle &operator=(const Circle &other)
	{
		_coord = other._coord;
		_size = other._size;
		return *this;
	}

	//Other constructors
	Circle(const coord &a, const coord &b) : _coord(a),
			_size(manhattanNorm(a, b))
	{}

	bool coordInCircle(const coord &c)	const	{return (_size >= (manhattanNorm(_coord, c)));}

	// coord c MUST be in circle
	int giveRightBorderX(const coord &c)	const	{return (xMax() - abs(c.second - _coord.second));}

	// bool is false if left border is not reachable or has already been reached
	// bool is true otherwise
	std::pair<bool, int>	giveLeftBorderX(const coord &c)	const
	{
		if (c.second > yMax() || c.second < yMin() || c.first >= xMin() + abs(c.second - _coord.second))
			return (std::make_pair(false, 0));
		else
			return (std::make_pair(true, xMin() + abs(c.second - _coord.second)));
	}

	int xMax()	const	{return (_coord.first + _size);}
	int	xMin()	const	{return (_coord.first - _size);}
	int yMax()	const	{return (_coord.second + _size);}
	int	yMin()	const	{return (_coord.second - _size);}

};

std::ostream &operator<<(std::ostream &o, const Circle &c)
{
	o << c._coord << '=' << c._size;
	return o;
}

std::ostream &operator<<(std::ostream &o, const std::vector<Circle> &v)
{
	for (std::vector<Circle>::const_iterator cit = v.begin(); cit != v.end(); ++cit)
		o << *cit << '\n';
	return o;
}

Circle	parseLine(const std::string &line, std::set<coord> &beacons)
{
	std::string::size_type idx1 = line.find("x=");
	std::string::size_type idx2 = line.find("y=");
	std::string::size_type idx3 = line.find("x=", idx2);
	std::string::size_type idx4 = line.find("y=", idx3);

	if (idx1 == std::string::npos || idx2 == std::string::npos ||
		idx3 == std::string::npos || idx4 == std::string::npos)
		throw (std::invalid_argument("file is incorrect\n"));

	coord	a(atoi(&line[idx1 + 2]), atoi(&line[idx2 + 2]));
	coord	b(atoi(&line[idx3 + 2]), atoi(&line[idx4 + 2]));
	beacons.insert(b);
	return (Circle(a, b));
}

int getMinX(const std::vector<Circle> &v)
{
	if (v.begin() == v.end())
		return 0;
	int min = v.begin()->xMin();
	for (std::vector<Circle>::const_iterator cit = ++v.begin(); cit != v.end(); ++cit)
		min = std::min(min, cit->xMin());
	return min;
}

int getMaxX(const std::vector<Circle> &v)
{
	if (v.begin() == v.end())
		return 0;
	int max = v.begin()->xMax();
	for (std::vector<Circle>::const_iterator cit = ++v.begin(); cit != v.end(); ++cit)
		max = std::max(max, cit->xMax());
	return max;
}

void	fillYSortedBeacons(std::set<coord> &YSorted, const std::set<coord> &XSorted)
{
	for (std::set<coord>::const_iterator cit = XSorted.begin(); cit != XSorted.end(); ++cit)
		YSorted.insert(coord(cit->second, cit->first));
}

int beaconsInRange(const coord &xy, int range, const std::set<coord> &ySorted)
{
	int xMin = xy.first;
	int xMax = xy.first + range;
	int count = 0;

	for (std::set<coord>::const_iterator cit = ySorted.begin(); cit != ySorted.end(); ++cit)
	{
		if (cit->first > xy.second)
			break;
		if (cit->first == xy.second)
		{
			if (cit->second > xMax)
				break;
			if (cit->second >= xMin)
				++count;
		}
	}
	return count;
}

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << "need 1 argument only\n";
		return 1;
	}
	std::ifstream input(av[1]);
	if (input.fail())
	{
		std::cerr << "couldn't open file\n";
		return 1;
	}
	std::string line;

	std::filebuf fb;
	fb.open("output.log", std::ios::out);
	std::ostream out(&fb);

	std::vector<Circle>	circles;
	std::set<coord>		beacons;
	while (!input.eof())
	{
		getline(input, line);
		if (line.back() == 13) line.erase(--line.end());
		circles.push_back(parseLine(line, beacons));
	}
	out << circles;
	std::set<coord>	ySortedBeacons;
	fillYSortedBeacons(ySortedBeacons, beacons);
	int xMin = getMinX(circles);
	int xMax = getMaxX(circles);
	std::cout << "range [" << xMin << ',' << xMax << "]\n";
	int count = 0;
	int y = atoi(av[2]);

	for (int x = xMin; x <= xMax; ++x)
	{
		std::set<int>	leftBorders;
		bool found = false;
		for (std::vector<Circle>::const_iterator cit = circles.begin(); cit != circles.end(); ++cit)
		{
			coord	xy(x, y);
			if (cit->coordInCircle(xy))
			{
				int nextX = cit->giveRightBorderX(xy);
				count += (nextX - x + 1);
				count -= beaconsInRange(xy, nextX - x, ySortedBeacons);
				x = nextX;
				found = true;
				break;
			}
			std::pair<bool, int>	result = cit->giveLeftBorderX(xy);
			if (result.first)
				leftBorders.insert(result.second);
		}
		if (!found && leftBorders.empty())
			break;
		if (!found)
			x = *leftBorders.begin() - 1;
	}

	std::cout << count << '\n';
	return 0;
}
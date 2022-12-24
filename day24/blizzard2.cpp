#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <cmath>
#include <map>
#include <set>

#define EMPTY 0
#define WALL 16
#define R 1
#define D 2
#define L 4
#define U 8
#define CURRENT 32

typedef std::vector<short> row;
typedef std::vector<row> grid;
typedef std::pair<int, int> coord;
typedef std::map<coord, std::set<unsigned int> >	visited;

std::ostream &operator<<(std::ostream &o, const coord &c)
{
	o << c.first << '|' << c.second;
	return o;
}

std::ostream	&operator<<(std::ostream &o, const std::set<unsigned int> &s)
{
	for (std::set<unsigned int>::const_iterator cit = s.begin(); cit != s.end(); ++cit)
		o << *cit << ' ';
	return o;
}

std::ostream	&operator<<(std::ostream &o, const visited &v)
{
	for (visited::const_iterator cit = v.begin(); cit != v.end(); ++cit)
		o << cit->first << " : " << cit->second << '\n';
	return o;
}

unsigned int ManhattanDist(const coord &a, const coord &b)
{
	return (abs(a.second - b.second) + abs(a.first - b.first));
}

char	toDisplay(short n)
{
	switch (n){
		case EMPTY:
			return '.';
		case WALL:
			return '#';
		case CURRENT:
			return 'E';
		default:
		{
			char direction;
			int nb = 0;
			if (n & R)
			{
				direction = '>';
				++nb;
			}
			if (n & D)
			{
				direction = 'v';
				++nb;
			}
			if (n & L)
			{
				direction = '<';
				++nb;
			}
			if (n & U)
			{
				direction = '^';
				++nb;
			}
			if (nb == 1)
				return direction;
			return ('0' + nb);
		}
	}
}


struct coordCmp : std::binary_function<std::pair<int, int>, std::pair<int, int>, bool>
{
	bool operator() (const std::pair<int, int>& x, const std::pair<int, int>& y) const {
		if (x.second == y.second)
			return x.first < y.first;
		return x.second < y.second;
	}
};

coord	&operator+=(coord &lhs, const coord &rhs)
{
	lhs.first += rhs.first;
	lhs.second += rhs.second;
	return lhs;
}

coord	operator+(const coord &lhs, const coord &rhs)
{
	coord	toReturn(lhs);
	return coord(toReturn += rhs);
}

coord	&operator-=(coord &lhs, const coord &rhs)
{
	lhs.first -= rhs.first;
	lhs.second -= rhs.second;
	return lhs;
}

coord	operator-(const coord &lhs, const coord &rhs)
{
	coord	toReturn(lhs);
	return coord(toReturn -= rhs);
}

std::ostream &operator<<(std::ostream &o, const row &r)
{
	for (row::const_iterator cit = r.begin(); cit != r.end(); ++cit)
		o << toDisplay(*cit);
	return o;
}

std::ostream &operator<<(std::ostream &o, const grid &g)
{
	for (grid::const_iterator cit = g.begin(); cit != g.end(); ++cit)
		o << *cit << '\n';
	return o;
}

void	parseLine(const std::string &line, grid &g)
{
	row	toPushToG;
	for (std::string::size_type idx = 0; idx < line.size(); ++idx)
	{
		switch (line[idx])
		{
			case '#':
				toPushToG.push_back(WALL);
				break;
			case '>':
				toPushToG.push_back(R);
				break;
			case 'v':
				toPushToG.push_back(D);
				break;
			case '<':
				toPushToG.push_back(L);
				break;
			case '^':
				toPushToG.push_back(U);
				break;
			case '.':
				toPushToG.push_back(EMPTY);
				break;
			case 'E':
				toPushToG.push_back(CURRENT);
				break;
			default:
				throw (std::invalid_argument("Incorrect file format.\n"));
		}
	}
	g.push_back(toPushToG);
}

class Sim
{

	Sim() {}
	coord	_Gilberte;
	coord	_startPoint;
	coord 	_endPoint;
	unsigned int 	_turn;

	void	_timeUp(const grid &prevG)
	{
		++_turn;
		for (grid::size_type y = 1; y < _g.size() - 1; ++y)
		{
			for (row::size_type x = 1; x < _g[y].size() - 1; ++x)
			{
				short	s = prevG[y][x];
				if (s != EMPTY)
				{
					if (s & 1)
					{
						if (x < _g[y].size() - 2)
							_g[y][x + 1] |= 1;
						else
							_g[y][1] |= 1;
					}
					if (s & 2)
					{
						if (y < _g.size() - 2)
							_g[y + 1][x] |= 2;
						else
							_g[1][x] |= 2;
					}
					if (s & 4)
					{
						if (x > 1)
							_g[y][x - 1] |= 4;
						else
							_g[y][_g[y].size() - 2] |= 4;
					}
					if (s & 8)
					{
						if (y > 1)
							_g[y - 1][x] |= 8;
						else
							_g[_g.size() - 2][x] |= 8;
					}
				}
			}
		}
	}

	void	_initStarts()
	{
		bool	GilberteFound = false;
		bool	endFound = false;
		for (row::size_type idx = 0; idx < _g[0].size(); ++idx)
		{
			if (_g[0][idx] == EMPTY || _g[0][idx] == CURRENT)
			{
				GilberteFound = true;
				_Gilberte = (coord (idx, 0));
				_startPoint = _Gilberte;
				_g[0][idx] = EMPTY;
				if (endFound)
					return ;
			}
			if (_g.back()[idx] == EMPTY)
			{
				endFound = true;
				_endPoint = (coord (idx, --_g.end() - _g.begin()));
				if (GilberteFound)
					return ;
			}
		}
		throw (std::invalid_argument("No end found.\n"));
	}

	int _checkAround()	const
	{
		if (_Gilberte == coord(_endPoint.first, _endPoint.second - 1))
			return 2;
		if (_Gilberte == coord(_endPoint.first, _endPoint.second + 1))
			return 8;
		int toReturn = 0;
		if (_g[_Gilberte.second][_Gilberte.first] == EMPTY)
			toReturn |= 16;
		if (_g[_Gilberte.second][_Gilberte.first + 1] == EMPTY)
			toReturn |= 1;
		if (_g[_Gilberte.second][_Gilberte.first - 1] == EMPTY)
			toReturn |= 4;
		if (_Gilberte.second != (_g.size() - 1) && _g[_Gilberte.second + 1][_Gilberte.first] == EMPTY)
			toReturn |= 2;
		if (_Gilberte.second != 0 && _g[_Gilberte.second - 1][_Gilberte.first] == EMPTY)
			toReturn |= 8;
		return toReturn;
	}

	bool	_isKnown(visited &v)	const
	{
		std::pair<visited::iterator, bool>	insertResult = v.insert(std::make_pair(_Gilberte, std::set<unsigned int>()));
		if (insertResult.second)
		{
			insertResult.first->second.insert(_turn);
			return false;
		}
		else
		{
			std::pair<std::set<unsigned int>::iterator, bool>	setInsertResult = insertResult.first->second.insert(_turn);
			if (setInsertResult.second)
				return false;
		}
		return true;
	}

public :

	grid	_g;

	Sim(const grid &g) : _turn(0), _g(g) {
		_initStarts();
	}
	Sim(const Sim &o) : _Gilberte(o._Gilberte), _startPoint(o._startPoint), _endPoint(o._endPoint), _turn(o._turn),
	_g(grid(o._g.size(), row(o._g[0].size(), EMPTY)))
	{
		for (grid::size_type y = 0; y != _g.size(); ++y)
		{
			_g[y][0] = WALL;
			_g[y][_g[0].size() - 1] = WALL;
		}
		for (row::size_type x = 0; x != _g[0].size(); ++x)
		{
			_g[0][x] = WALL;
			_g[_g.size() - 1][x] = WALL;
		}
		_g[_startPoint.second][_startPoint.first] = EMPTY;
		_g[_endPoint.second][_endPoint.first] = EMPTY;
	}
	~Sim() {}
	Sim &operator=(const Sim &o)
	{
		_Gilberte = o._Gilberte;
		_startPoint = o._endPoint;
		_endPoint = o._startPoint;
		_turn = 0;
		_g = o._g;
		return *this;
	}

	void	dnf(unsigned int &result, visited &v, Sim &newStart) const
	{
		if (_isKnown(v))
			return;
		if (_turn + ManhattanDist(_Gilberte, _endPoint) >= result)
			return ;
		if (_Gilberte == _endPoint)
		{
			result = _turn;
			std::cout << result << '\n';
			newStart = *this;
			return ;
		}

		Sim cpy(*this);
		cpy._timeUp(_g);
		int possibilities = cpy._checkAround();
		if (!possibilities)
			return;
		if (possibilities & 1)
		{
			cpy._Gilberte += coord(1, 0);
			cpy.dnf(result, v, newStart);
			cpy._Gilberte -= coord(1, 0);
		}
		if (possibilities & 2)
		{
			cpy._Gilberte += coord(0, 1);
			cpy.dnf(result, v, newStart);
			cpy._Gilberte -= coord(0, 1);
		}
		if (possibilities & 4)
		{
			cpy._Gilberte += coord(-1, 0);
			cpy.dnf(result, v, newStart);
			cpy._Gilberte -= coord(-1, 0);
		}
		if (possibilities & 8)
		{
			cpy._Gilberte += coord(0, -1);
			cpy.dnf(result, v, newStart);
			cpy._Gilberte -= coord(0, -1);
		}
		if (possibilities & 16)
			cpy.dnf(result, v, newStart);
	}

	unsigned int exe()	const
	{
		unsigned int	result = 3000;
		unsigned int	totalResult = 0;
		visited v;
		Sim	newStart;
		dnf(result, v, newStart);
		totalResult += result;
		std::cout << "totalResult = " << totalResult << " start and end : " << newStart._startPoint << ' ' << newStart._endPoint << '\n';
		result = 3000;
		v.clear();
		Sim newStart2;
		newStart.dnf(result, v, newStart2);
		totalResult += result;
		std::cout << "totalResult = " << totalResult << '\n';
		result = 3000;
		v.clear();
		newStart2.dnf(result, v, newStart);
		totalResult += result;
		std::cout << "totalResult = " << totalResult << '\n';
		return totalResult;
	}

	void displayGrid()
	{
		_g[_Gilberte.second][_Gilberte.first] = CURRENT;
		std::cout << _g << '\n';
		_g[_Gilberte.second][_Gilberte.first] = EMPTY;
	}

};

int main(int ac, char **av)
{
	if (ac != 2) return 1;
	std::ifstream input(av[1]);
	if (input.fail()) return 2;

	grid valley;
	std::string line;
	while (!input.eof())
	{
		getline(input, line);
		if (line.back() == 13) line.erase(--line.end());
		parseLine(line, valley);
	}
	Sim sim(valley);
	unsigned int result = sim.exe();
	std::cout << result << " is result.\n";
	return 0;
}
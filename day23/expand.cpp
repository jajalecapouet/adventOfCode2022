#include <iostream>
#include <map>
#include <fstream>
#include <vector>

#define NORTH 0
#define SOUTH 1
#define WEST 2
#define EAST 3
#define DTC 4

typedef std::pair<int, int>	coord;
typedef std::vector<char>	row;
typedef std::vector<row>	grid;

struct triple
{
	coord	one;
	coord	two;
	coord	three;
};

struct coordCmp : std::binary_function<std::pair<int, int>, std::pair<int, int>, bool>
{
	bool operator() (const std::pair<int, int>& x, const std::pair<int, int>& y) const {
		if (x.second == y.second)
			return x.first < y.first;
		return x.second < y.second;
	}
};

std::ostream &operator<<(std::ostream &o, const coord &c)
{
	o << c.first << '|' << c.second;
	return o;
}

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
		o << *cit;
	return o;
}

std::ostream &operator<<(std::ostream &o, const grid &g)
{
	for (grid::const_iterator cit = g.begin(); cit != g.end(); ++cit)
		o << *cit << '\n';
	return o;
}

class Elf {

	Elf() {}

public :

	int _idx;
	coord _pos;
	coord _posWanted;
	bool _posChosen;

	Elf(int idx, const coord &c) : _idx(idx), _pos(c), _posChosen(false) {}
	Elf(const Elf &o) : _idx(o._idx), _pos(o._pos), _posWanted(o._posWanted), _posChosen(o._posChosen) {}
	~Elf() {}
	Elf &operator=(const Elf &o)
	{
		_idx = o._idx;
		_pos = o._pos;
		_posWanted = o._posWanted;
		_posChosen = o._posChosen;
		return *this;
	}
};

typedef std::map<coord, Elf *, coordCmp>	Visited;

class Sim
{

	std::map<coord, int>	_wished;
	int _priority;

	bool _empty(const coord &c) const
	{
		Visited::const_iterator found = _map.find(c);
		return (found == _map.end() || !found->second);
	}

	triple _look(int side) const
	{
		triple	toReturn;
		switch (side % DTC)
		{
			case NORTH:
				toReturn.one = coord(-1, -1);
				toReturn.two = coord(0, -1);
				toReturn.three = coord(1, -1);
				break;
			case SOUTH:
				toReturn.one = coord(-1, 1);
				toReturn.two = coord(0, 1);
				toReturn.three = coord(1, 1);
				break;
			case WEST:
				toReturn.one = coord(-1, -1);
				toReturn.two = coord(-1, 0);
				toReturn.three = coord(-1, 1);
				break;
			case EAST:
				toReturn.one = coord(1, -1);
				toReturn.two = coord(1, 0);
				toReturn.three = coord(1, 1);
				break;
			default:
				throw (std::invalid_argument("Direction doesn't exists.\n"));
		}
		return toReturn;
	}

	bool _someoneAround(Elf *elf) const
	{
		int xMax = elf->_pos.first + 1;
		int xMin = xMax - 2;
		int yMax = elf->_pos.second + 1;
		int yMin = yMax - 2;
		for (int y = yMin; y <= yMax; ++y)
		{
			for (int x = xMin; x <= xMax; ++x)
			{
				if (!(x == elf->_pos.first && y == elf->_pos.second))
				{
					Visited::const_iterator found = _map.find(coord(x, y));
					if (found != _map.end() && found->second)
						return true;
				}
			}
		}
		return false;
	}

	void _doWishes()
	{
		for (std::vector<Elf *>::iterator it = _elves.begin(); it != _elves.end(); ++it)
		{
			(*it)->_posChosen = false;
			if (_someoneAround(*it))
			{
				triple	check;
				for (int i = 0; i < 4; ++i)
				{
					check = _look(_priority + i);
					if (_empty((*it)->_pos + check.one) && _empty((*it)->_pos + check.two) && _empty((*it)->_pos + check.three))
					{
						(*it)->_posChosen = true;
						(*it)->_posWanted = (*it)->_pos + check.two;
						std::pair<std::map<coord, int>::iterator, bool>	insertResult = _wished.insert(std::make_pair((*it)->_posWanted, 1));
						if (!insertResult.second)
							insertResult.first->second = 0;
						break;
					}
				}
			}
		}
	}

	void _applyWishes()
	{
		for (std::vector<Elf *>::iterator it = _elves.begin(); it != _elves.end(); ++it)
		{
			if ((*it)->_posChosen)
			{
				if (_wished[(*it)->_posWanted])
				{
					_map[(*it)->_pos] = 0;
					_map[(*it)->_posWanted] = *it;
					(*it)->_pos = (*it)->_posWanted;
				}
			}
		}
	}

	void _doTurn()
	{
		_doWishes();
		_applyWishes();
		_wished.clear();
		++_priority %= 4;
	}

public :

	Visited	_map;
	std::vector<Elf *>	_elves;

	Sim() : _priority(0) {}
	Sim(const Sim &o) : _priority(o._priority), _wished(o._wished), _map(o._map), _elves(o._elves) {}
	~Sim()
	{
		for (std::vector<Elf *>::iterator it = _elves.begin(); it != _elves.end(); ++it)
			delete *it;
	}
	Sim	&operator=(const Sim &o)
	{
		_wished = o._wished;
		_priority = o._priority;
		_map = o._map;
		_elves = o._elves;
		return *this;
	}

	void	exe()
	{
		for (int i = 0; i < 10; ++i)
			_doTurn();
	}

	void	fit()
	{
		Visited::iterator it = _map.begin();
		while (it != _map.end())
		{
			Visited::iterator next = it;
			++next;
			if (!it->second)
				_map.erase(it);
			it = next;
		}
	}

	int result()	const
	{
		int	yMax = (--_map.end())->first.second;
		int yMin = _map.begin()->first.second;
		int y = yMax - yMin;
		int xMin = _map.begin()->first.first;
		int xMax = (--_map.end())->first.first;
		int little = -0xffff;
		int idx = yMin;
		while (idx <= yMax)
		{
			Visited::const_iterator cit = _map.upper_bound(coord(little, idx));
			if (cit->first.first < xMin)
				xMin = cit->first.first;
			if (cit != _map.begin())
			{
				--cit;
				if (cit->first.first > xMax)
					xMax = cit->first.first;
			}
			++idx;
		}
		int x = xMax - xMin;
		return ((x + 1) * (y + 1)) - _map.size();
	}

};

std::ostream &operator<<(std::ostream &o, const std::vector<Elf *> &v)
{
	for (std::vector<Elf *>::const_iterator cit = v.begin(); cit != v.end(); ++cit)
	{
		o << "Elf " << (*cit)->_idx << " in pos: " << (*cit)->_pos << '\n';
	}
	return o;
}

std::ostream &operator<<(std::ostream &o, const Visited &v)
{
	if (v.empty())
		return o;
	int	yMax = (--v.end())->first.second;
	int yMin = v.begin()->first.second;
	int y = yMax - yMin;
	int xMin = v.begin()->first.first;
	int xMax = (--v.end())->first.first;
	int little = -0xffff;
	int idx = yMin;
	while (idx <= yMax)
	{
		Visited::const_iterator cit = v.upper_bound(coord(little, idx));
		if (cit->first.first < xMin)
			xMin = cit->first.first;
		if (cit != v.begin())
		{
			--cit;
			if (cit->first.first > xMax)
				xMax = cit->first.first;
		}
		++idx;
	}
	int x = xMax - xMin;

	grid	displayable(y + 1, row(x + 1, '.'));
	for (Visited::const_iterator cit = v.begin(); cit != v.end(); ++cit)
	{
		if (cit->second)
			displayable[cit->first.second - yMin][cit->first.first - xMin] = '#';
	}
	return o << displayable;
}

std::ostream &operator<<(std::ostream &o, const Sim &s)
{
	o << "elves are : \n" << s._elves << '\n';
	o << "map actually is :\n\n" << s._map << '\n';
	return o;
}

void	parseLine(const std::string &line, Sim &sim, int y)
{
	for (std::string::size_type idx = 0; idx < line.size(); ++idx)
	{
		if (line[idx] == '#')
		{
			sim._elves.push_back(new Elf(sim._elves.size() + 1, coord(idx, y)));
			sim._map.insert(std::make_pair(coord(idx, y), sim._elves.back()));
		}
	}
}

int main(int ac, char **av)
{
	if (ac != 2) return 1;
	std::ifstream input(av[1]);
	if (input.fail()) return 2;

	std::string line;
	Sim sim;
	int y = 0;
	while (!input.eof())
	{
		getline(input, line);
		if (line.back() == 13) line.erase(--line.end());
		parseLine(line, sim, y);
		++y;
	}
	sim.exe();
	sim.fit();
	std::cout << sim << '\n';
	std::cout << sim.result() << " is result.\n";


	return 0;
}
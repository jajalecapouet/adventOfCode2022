#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <utility>
#include <string>
#include <cmath>
#include <deque>
#include <ostream>

typedef std::vector<std::vector<int> >	grid;
typedef std::pair<int, int>				coord;


std::ostream	&operator<<(std::ostream &o, const std::pair<int, int> &p)
{
	o << p.first << " | " << p.second;
	return o;
}

std::ostream	&operator<<(std::ostream &o, const std::vector<int> &v)
{
	for (int i = 0; i < v.size(); ++i)
	{
		o << v[i];
	}
	return o;
}

std::ostream	&operator<<(std::ostream &o, const grid &grid)
{
	for (int i = 0; i < grid.size(); ++i)
	{
		o << grid[i] << '\n';
	}
	return o;
}

coord	&operator+=(coord &lhs, const coord &rhs)
{
	lhs.first += rhs.first;
	lhs.second += rhs.second;
	return(lhs);
}

coord	operator+(const coord &lhs, const coord &rhs)
{
	return std::make_pair(lhs.first + rhs.first, lhs.second + rhs.second);
}

class HeuristicManhattan
{
	friend class AStar; //To allows AStar to compile for the default constructor

	HeuristicManhattan() {}
	grid	_heuristicGrid;

public :

	HeuristicManhattan(const HeuristicManhattan &other) : _heuristicGrid(other._heuristicGrid)
	{}
	HeuristicManhattan(const std::vector<std::string> &grid, const std::pair<int, int> &end)
	{
		for (int idxV = 0; idxV < grid.size(); ++idxV)
		{
			std::vector<int> row;
			for (int idxS = 0; idxS < grid[idxV].size(); ++idxS)
				row.push_back(abs(idxS - end.first) + abs(idxV - end.second));
			_heuristicGrid.push_back(row);
		}
	}
	HeuristicManhattan(const grid &grid, const std::pair<int, int> &end)
	{
		for (int idxV = 0; idxV < grid.size(); ++idxV)
		{
			std::vector<int> row;
			for (int idxS = 0; idxS < grid[idxV].size(); ++idxS)
				row.push_back(abs(idxS - end.first) + abs(idxV - end.second));
			_heuristicGrid.push_back(row);
		}
	}
	~HeuristicManhattan() {}
	HeuristicManhattan	&operator=(const HeuristicManhattan &other) {
		_heuristicGrid = other._heuristicGrid;
		return (*this);
	}

	int getHeuristicValue(const std::pair<int, int> &pos)	const
	{
		if (pos.second < 0 || pos.second >= _heuristicGrid.size() ||
			pos.first < 0 || pos.first >= _heuristicGrid[pos.second].size())
		return (-1); // Out of border case
		return (_heuristicGrid[pos.second][pos.first]);
	}

	const grid	&getHeuristic()	const
	{
		return _heuristicGrid;
	}
};

class Node
{

		Node() : _value(0), _cost(0), _parent(0)
		{}

	public :

		std::pair<int, int>	_pos;
		int					_value;
		int 				_cost;
		Node				*_parent;

		Node(coord pos, Node *parent, const HeuristicManhattan &grid) : _pos(pos), _parent(parent)
		{
			if (parent)
				_cost = parent->_cost + 1;
			else
				_cost = 0;
			_value = _cost + grid.getHeuristicValue(_pos);
		}
		Node(coord pos, Node *parent, int heuristicValue) : _pos(pos), _parent(parent)
		{
			if (parent)
				_cost = parent->_cost + 1;
			else
				_cost = 0;
			_value = _cost + heuristicValue;
		}
		Node(const Node &other) : _pos(other._pos), _value(other._value), _cost(other._cost), _parent(other._parent)
		{}
		~Node() {}
		Node &operator=(const Node &other)
		{
			_pos = other._pos;
			_value = other._value;
			_cost = other._cost;
			_parent = other._parent;
			return (*this);
		}
};

typedef std::multimap<int, Node*>				openType;
typedef std::map<std::pair<int, int>, Node*>	closeType;

std::ostream &operator<<(std::ostream &o, const openType &open)
{
	int i = 0;
	for (openType::const_iterator it = open.begin(); it != open.end(); ++it)
	{
		++i;
		o << "key = " << it->first << " ";
		o << "noeud " << i << " : ";
		o << "pos " << it->second->_pos << " ";
		o << "cost " << it->second->_cost << " ";
		o << "val " << it->second->_value << " ";
		o << "parent " << it->second->_parent << '\n';
	}
	return o;
}

std::ostream &operator<<(std::ostream &o, const closeType &close)
{
	int i = 0;
	for (closeType::const_iterator it = close.begin(); it != close.end(); ++it)
	{
		++i;
		o << "key = " << it->first << " ";
		o << "noeud " << i << " : ";
		o << "pos " << it->second->_pos << " ";
		o << "cost " << it->second->_cost << " ";
		o << "val " << it->second->_value << " ";
		o << "parent " << it->second->_parent << '\n';
	}
	return o;
}

class AStar
{
	//Coplien not to use
	AStar() {}
	AStar(const AStar &other) {}
	AStar &operator&(const AStar &other) {return *this;}
	//End of Coplien not to use

	//Member's data
	openType								_open;
	closeType 								_close;
	std::set<coord>							_secureFreeTool;
	coord									_start;
	coord									_end;
	grid									_grid;
	HeuristicManhattan						_heuristic;
	bool									_endReached;
	Node									*_current;

	//Private tool functions

	bool _heightOk(coord a, coord b)
	{
		return (_grid[a.second][a.first] - _grid[b.second][b.first] <= 1);
	}

	void _checkSide(const coord &move)
	{
		coord	newCoord = _current->_pos + move;
		int newVal = _heuristic.getHeuristicValue(newCoord);
		if (newVal != -1 && _heightOk(newCoord, _current->_pos)) // check if I'm out of border + if the height is too big
		{
		//	std::cout << newVal << '\n';
			closeType::iterator	found = _close.find(_current->_pos);
			if (found != _close.end())
			{
				if (found->second->_value > newVal + 1 + _current->_cost)
				{
					Node	*newNode = new Node(newCoord, _current, newVal);
					_open.insert(std::make_pair(newNode->_value, newNode));
					_close.erase(found);
				}
			}
			else
			{
				Node	*newNode = new Node(_current->_pos + move, _current, newVal);
				_open.insert(std::make_pair(newNode->_value, newNode));
			}
		}
	}

	public :

	AStar(const grid &grid, const coord &start, const coord &end) : _start(start), _end(end), _grid(grid), _heuristic(_grid, _end),
																	_endReached(false)
	{
		std::cout << _grid;
		std::cout << _heuristic._heuristicGrid;
		Node *newNode = new Node(_start, NULL, _heuristic);
		_open.insert(std::make_pair(newNode->_value, newNode));
		_current = _open.begin()->second;
		//std::cout << "start is at " << _current->_pos << '\n';
		while (!_open.empty() && !_endReached)
		{
		//	std::cout << "current pos is at " << _current->_pos << '\n';
			_checkSide(std::make_pair(-1, 0));	//LEFT
			_checkSide(std::make_pair(0, -1));	//UP
			_checkSide(std::make_pair(0, 1));	//DOWN
			_checkSide(std::make_pair(1, 0));	//RIGHT
		//	std::cout << "open content\n" << _open << '\n';
		//	std::cout << "close content\n" << _close << '\n';
			std::pair<closeType::iterator, bool> res = _close.insert(std::make_pair(_open.begin()->second->_pos, _open.begin()->second));
			if (!res.second)
			{
				/*if (res.first->second->_value > _open.begin()->second->_value)
					res.first->second = _open.begin()->second;*/
				std::cerr << "error\n";
			}
			_open.erase(_open.begin());
			if (!_heuristic.getHeuristicValue(_current->_pos)) {
				std::cout << "end reached at " << _current->_pos << '\n';
				_endReached = true;
			}
			if (!_open.empty() && !_endReached)
				_current = _open.begin()->second;
		}
	}

	~AStar ()
	{
		for (openType::iterator it = _open.begin(); it != _open.end(); ++it)
		{
			_secureFreeTool.insert(it->second->_pos);
			delete (it->second);
		}
		for (closeType::iterator it = _close.begin(); it != _close.end(); ++it)
		{
			if (_secureFreeTool.find(it->first) == _secureFreeTool.end())
				delete (it->second);
		}
	}

	int stepCount()	const
	{
		int 	stepCount = 0;
		Node	*idx = _current;
		if (!_endReached)
			return (-1);
		while (idx->_parent)
		{
			std::cout << "chemin : " << idx->_pos << " is char " << (_grid[idx->_pos.second][idx->_pos.first]) << '\n';
			idx = idx->_parent;
			++stepCount;
		}
		std::cout << "chemin : " << idx->_pos << " is char " << (_grid[idx->_pos.second][idx->_pos.first]) << '\n';
		return (stepCount);
	}

};

std::vector<int> parseStrToInt(const std::string &line)
{
	std::vector<int>	parsed;
	for (std::string::const_iterator it = line.begin(); it != line.end(); ++it) {
		if (*it == 'S')
			parsed.push_back(-100);
		else if (*it == 'E')
			parsed.push_back(100);
		else
			parsed.push_back(*it - 'a' + 1);
	}
	return parsed;
}

std::pair<coord, coord> findGoals(grid &grid)
{
	coord start;
	coord end;
	for (int y = 0; y < grid.size(); ++y)
	{
		for (int x = 0; x < grid[y].size(); ++x)
		{
			if (grid[y][x] == -100)
			{
				grid[y][x] = 1;
				start = std::make_pair(x, y);
			}
			if (grid[y][x] == 100) {
				grid[y][x] = 27;
				end = std::make_pair(x, y);
			}
		}
	}
	return (std::make_pair(start, end));
}

int main(int ac, char **av)
{
	if (ac != 2) return 1;
	std::ifstream	input(av[1]);
	if (input.fail()) return 1;

	grid	grid;
	std::string	line;
	while (!input.eof())
	{
		getline(input, line);
		if (line.back() == 13) line.erase(--line.end());
		grid.push_back(parseStrToInt(line));
	}
	std::pair<coord, coord>	goals = findGoals(grid);
	AStar	result(grid, goals.first, goals.second);
	std::cout << "begin = " << goals.first << " | end = " << goals.second << '\n';
	std::cout << result.stepCount() << '\n';
	std::cout << 455 << " is too low\n";
	return 0;
}
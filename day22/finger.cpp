#include <iostream>
#include <fstream>
#include <vector>
#include <list>

#define RIGHT 0
#define DOWN 1
#define LEFT 2
#define UP 3

typedef std::vector<char>	row;
typedef std::vector<row>	grid;

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

class Command
{
	char	_turn;
	int		_dist;

public:

	Command() : _turn(0), _dist(0) {}
	Command(char t) : _turn(t), _dist(0) {}
	Command(int d) : _turn(0), _dist(d) {}
	Command(const Command &o) : _turn(o._turn), _dist(o._dist) {}
	~Command() {}
	Command &operator=(const Command &o)
	{
		_turn = o._turn;
		_dist = o._dist;
		return *this;
	}

	int dist()	const {return _dist;}
	char turn()	const {return _turn;}
	void dist(int d) {
		_dist = d;
		_turn = 0;
	}

	void turn(char t) {
		_turn = t;
		_dist = 0;
	}
};

class Finger
{

	void	_setFingerStart()
	{
		for (grid::const_iterator cit = _g.begin(); cit != _g.end(); ++cit)
		{
			for (row::const_iterator citR = cit->begin(); citR != cit->end(); ++citR)
			{
				if (*citR == '.')
				{
					_y = cit - _g.begin();
					_x = citR - cit->begin();
					return;
				}
			}
		}
		throw (std::invalid_argument("undefined map.\n"));
	}

	void	_setCpyIdx(const Finger &o)
	{
		std::list<Command>::const_iterator	idxO = o._idx;
		_idx = _cmd.begin();
		while (idxO != o._cmd.begin())
		{
			--idxO;
			++_idx;
		}
	}

	void	_turn(char c)
	{
		if (c == 'R') {
			switch (_dir) {
				case RIGHT:
					_dir = DOWN;
					break;
				case DOWN:
					_dir = LEFT;
					break;
				case LEFT:
					_dir = UP;
					break;
				default:
					_dir = RIGHT;
			}
		}
		else
		{
			switch (_dir) {
				case RIGHT:
					_dir = UP;
					break;
				case UP:
					_dir = LEFT;
					break;
				case LEFT:
					_dir = DOWN;
					break;
				default:
					_dir = RIGHT;
			}
		}
	}

	int _oppositeFromRight()	const
	{
		int i = 0;
		while (_g[_y][i] == ' ')
			++i;
		return i;
	}

	int _oppositeFromLeft()	const
	{
		int i = _g[_y].size() - 1;
		while (_g[_y][i] == ' ')
			--i;
		return i;
	}

	int _oppositeFromDown()	const
	{
		int i = 0;
		while (_g[i][_x] == ' ')
			++i;
		return i;
	}

	int _oppositeFromUp()	const
	{
		int i = _g.size() - 1;
		while (_g[i][_x] == ' ')
			--i;
		return i;
	}

	void	_goForwardRight(int dist)
	{
		while (dist--)
		{
			if (_x + 1 == _g[_y].size() || _g[_y][_x + 1] == ' ')
			{
				int opposite = _oppositeFromRight();
				if (_g[_y][opposite] == '#')
					break;
				else
					_x = opposite;
			}
			else
			{
				if (_g[_y][_x + 1] == '#')
					break;
				else
					_x += 1;
			}
		}
	}

	void	_goForwardLeft(int dist)
	{
		while (dist--)
		{
			if (_x == 0 || _g[_y][_x - 1] == ' ')
			{
				int opposite = _oppositeFromLeft();
				if (_g[_y][opposite] == '#')
					break;
				else
					_x = opposite;
			}
			else
			{
				if (_g[_y][_x - 1] == '#')
					break;
				else
					_x -= 1;
			}
		}
	}

	void	_goForwardDown(int dist)
	{
		while (dist--)
		{
			if (_y + 1 == _g.size() || _g[_y + 1][_x] == ' ')
			{
				int opposite = _oppositeFromDown();
				if (_g[opposite][_x] == '#')
					break;
				else
					_y = opposite;
			}
			else
			{
				if (_g[_y + 1][_x] == '#')
					break;
				else
					_y += 1;
			}
		}
	}

	void	_goForwardUp(int dist)
	{
		while (dist--)
		{
			if (_y == 0 || _g[_y - 1][_x] == ' ')
			{
				int opposite = _oppositeFromUp();
				if (_g[opposite][_x] == '#')
					break;
				else
					_y = opposite;
			}
			else
			{
				if (_g[_y - 1][_x] == '#')
					break;
				else
					_y -= 1;
			}
		}
	}

	void	_goForward(int dist)
	{
		switch (_dir)
		{
			case 0:
				_goForwardRight(dist);
				break;
			case 1:
				_goForwardDown(dist);
				break;
			case 2:
				_goForwardLeft(dist);
				break;
			default:
				_goForwardUp(dist);
		}
	}

public:
	grid	_g;
	int 	_x;
	int		_y;
	int 	_dir;
	std::list<Command>	_cmd;
	std::list<Command>::iterator	_idx;

	Finger() : _x(0), _y(0), _dir(RIGHT) {}
	Finger(const grid &g, const std::list<Command> &cmd) : _g(g), _dir(RIGHT), _cmd(cmd), _idx(_cmd.begin())
	{
		_setFingerStart();
	}
	Finger(const Finger &o) : _g(o._g), _x(o._x), _y(o._y), _dir(o._dir), _cmd(o._cmd)
	{
		_setCpyIdx(o);
	}
	Finger &operator=(const Finger &o)
	{
		_g = o._g;
		_x = o._x;
		_y = o._y;
		_dir = o._dir;
		_cmd = o._cmd;
		_setCpyIdx(o);
		return *this;
	}

	void exe()
	{
		for (std::list<Command>::const_iterator cit = _cmd.begin(); cit != _cmd.end(); ++cit)
		{
			if (cit->turn())
				_turn(cit->turn());
			else
				_goForward(cit->dist());
		}
	}

	int returnResult()	const
	{
		return (1000 * (_y + 1) + 4 * (_x + 1) + _dir);
	}
};

//DEBUG TOOLS

std::ostream &operator<<(std::ostream &o, const Command &c)
{
	if (c.dist())
		o << " -> " << c.dist();
	else switch(c.turn())
		{
			case 'R':
				o << " R";
				break;
			case 'L':
				o << " L";
				break;
			default:
				o << "parsing fail or incorrect map\n";
		}
	return o;
}

std::ostream &operator<<(std::ostream &o, std::list<Command> &l)
{
	for (std::list<Command>::const_iterator cit = l.begin(); cit != l.end(); ++cit)
	{
		o << *cit;
	}
	return o;
}

//PARSING TOOLS
bool isCharGrid(char abia)
{
	if (abia == ' ' || abia == '.' || abia == '#')
		return true;
	return false;
}

void parseLineGrid(const std::string &line, grid &g)
{
	g.push_back(row());
	for (std::string::const_iterator cit = line.begin(); cit != line.end(); ++cit)
		g.back().push_back(*cit);
}

void	equalize(grid &g) {
	int max = 0;
	for (grid::iterator it = g.begin(); it != g.end(); ++it)
	{
		if (it->size() > max)
			max = it->size();
	}
	for (grid::iterator it = g.begin(); it != g.end(); ++it)
	{
		if (it->size() < max)
			it->resize(max, ' ');
	}
}

void parseLineCmd(const std::string &line, std::list<Command> &cmd)
{
	std::string::size_type	idx = 0;
	std::string::size_type	idx2 = 0;
	while (idx < line.size())
	{
		if (line[idx] == 'R')
		{
			if (idx - idx2 > 0)
				cmd.push_back(Command(atoi(&line[idx2])));
			cmd.push_back(Command('R'));
			idx2 = idx + 1;
		}
		else if (line[idx] == 'L')
		{
			if (idx - idx2 > 0)
				cmd.push_back(Command(atoi(&line[idx2])));
			cmd.push_back(Command('L'));
			idx2 = idx + 1;
		}
		++idx;
	}
	if (idx - idx2 > 0)
		cmd.push_back(Command(atoi(&line[idx2])));
}

int main(int ac, char **av)
{
	if (ac != 2) return 1;
	std::ifstream input(av[1]);
	if (input.fail()) return 1;

	std::string line;
	grid	g;
	std::list<Command>	cmd;
	while (!input.eof())
	{
		getline(input, line);
		if (line.back() == 13) line.erase(--line.end());
		if (!line.empty())
		{
			if (isCharGrid(line.front()))
				parseLineGrid(line, g);
			else
				parseLineCmd(line, cmd);
		}
	}
	equalize(g);
	Finger	douah(g, cmd);
	douah.exe();
	std::cout << douah.returnResult() << " is result.\n";
	return 0;
}
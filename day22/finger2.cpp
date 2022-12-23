#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <map>
#include <utility>

#define RIGHT 0
#define DOWN 1
#define LEFT 2
#define UP 3

typedef std::vector<char>	row;
typedef std::vector<row>	grid;
typedef std::pair<int, int>	coord;


template< class T >
struct coordCmp : std::binary_function<T, T, bool>
{
	bool operator() (const T& x, const T& y) const {
		if (x.second == y.second)
			return x.first < y.first;
		return x.second < y.second;
	}
};

bool operator<(const coord &lhs, const coord &rhs)
{
	if (lhs.second == rhs.second)
		return lhs.first < rhs.first;
	return (lhs.second < rhs.second);
}

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

class Face
{
public :
	const grid	_g;
	//next are where I am when I'm out of _g's border (Face *), how I should turn
	//when I reach the new face (int).
	std::pair<Face *, int>	_right;
	std::pair<Face *, int>	_down;
	std::pair<Face *, int>	_left;
	std::pair<Face *, int>	_up;

	Face(const grid &g) : _g(g) {}
};

typedef std::map<coord, Face *, coordCmp<coord> >	Loc;

class Finger
{

	void	_setFingerStart()
	{
		for (grid::const_iterator cit = _face->_g.begin(); cit != _face->_g.end(); ++cit)
		{
			for (row::const_iterator citR = cit->begin(); citR != cit->end(); ++citR)
			{
				if (*citR == '.')
				{
					_y = cit - _face->_g.begin();
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
		if (c == 'R') {// && !_inverted) || (c == 'L' && _inverted)) {
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

	void	_goForwardRight(int dist)
	{
		while (dist--)
		{
			if (_x + 1 == _face->_g[_y].size())
			{
				Face	*right = _face->_right.first;
				switch (_face->_right.second)
				{
					case RIGHT:
						if (right->_g[_y][0] == '#')
							return ;
						_face = right;
						_x = 0;
						break;
					case UP:
						if (right->_g[right->_g.size() - 1][_y] == '#')
							return ;
						_face = right;
						_x = _y;
						_y = right->_g.size() - 1;
						_dir = UP;
						_goForward(dist);
						return ;
					case LEFT:
						if (right->_g[right->_g.size() - (1 + _y)][right->_g[0].size() - 1] == '#')
							return ;
						_face = right;
						_x = right->_g[0].size() - 1;
						_y = right->_g.size() - (1 + _y);
						_dir = LEFT;
						if (_inverted)
							_inverted = false;
						else
							_inverted = true;
						_goForward(dist);
						return;
					default:
						throw(std::invalid_argument("faces -> right parsing code error\n"));
				}
			}
			else
			{
				if (_face->_g[_y][_x + 1] == '#')
					return ;
				_x += 1;
			}
		}
	}

	void	_goForwardLeft(int dist)
	{
		while (dist--)
		{
			if (_x == 0)
			{
				Face	*left = _face->_left.first;
				switch (_face->_left.second)
				{
					case LEFT:
						if (left->_g[_y][left->_g[0].size() - 1] == '#')
							return ;
						_face = left;
						_x = left->_g[0].size() - 1;
						break;
					case DOWN:
						if (left->_g[0][_y] == '#')
							return ;
						_face = left;
						_x = _y;
						_y = 0;
						_dir = DOWN;
						_goForward(dist);
						return ;
					case RIGHT:
						if (left->_g[left->_g.size() - (1 + _y)][0] == '#')
							return ;
						_face = left;
						_x = 0;
						_y = left->_g.size() - (1 + _y);
						_dir = RIGHT;
						if (_inverted)
							_inverted = false;
						else
							_inverted = true;
						_goForward(dist);
						return ;
					default:
						throw(std::invalid_argument("faces -> left parsing code error\n"));
				}
			}
			else
			{
				if (_face->_g[_y][_x - 1] == '#')
					return ;
				_x -= 1;
			}
		}
	}

	void	_goForwardDown(int dist)
	{
		while (dist--)
		{
			if (_y + 1 == _face->_g.size())
			{
				Face	*down = _face->_down.first;
				switch (_face->_down.second)
				{
					case DOWN:
						if (down->_g[0][_x] == '#')
							return ;
						_face = down;
						_y = 0;
						break;
					case LEFT:
						if (down->_g[_x][down->_g[0].size() - 1] == '#')
							return ;
						_face = down;
						_y = _x;
						_x = down->_g[0].size() - 1;
						_dir = LEFT;
						_goForward(dist);
						return ;
					default:
						throw(std::invalid_argument("faces -> down parsing code error\n"));
				}
			}
			else
			{
				if (_face->_g[_y + 1][_x] == '#')
					return ;
				_y += 1;
			}
		}
	}

	void	_goForwardUp(int dist)
	{
		while (dist--)
		{
			if (_y == 0)
			{
				Face	*up = _face->_up.first;
				switch (_face->_up.second)
				{
					case UP:
						if (up->_g[up->_g.size() - 1][_x] == '#')
							return ;
						_y = up->_g.size() - 1;
						_face = up;
						break;
					case RIGHT:
						if (up->_g[_x][0] == '#')
							return ;
						_face = up;
						_y = _x;
						_x = 0;
						_dir = RIGHT;
						_goForward(dist);
						return ;
					default:
						throw(std::invalid_argument("faces -> up parsing code error\n"));
				}
			}
			else
			{
				if (_face->_g[_y - 1][_x] == '#')
					return ;
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

	Finger() : _x(0), _y(0), _dir(RIGHT) {}

public:

	Loc 	_l;
	int 	_x;
	int		_y;
	int 	_dir;
	Face	*_face;
	bool 	_inverted; // I did this bool in case if the commands are inverted when I invert my direction
	std::list<Command>	_cmd;
	std::list<Command>::iterator	_idx;

	Finger(const Loc &l, const std::list<Command> &cmd) : _l(l), _dir(RIGHT), _face(l.begin()->second), _inverted(false), _cmd(cmd), _idx(_cmd.begin())
	{
		_setFingerStart();
	}
	Finger(const Finger &o) : _l(o._l), _x(o._x), _y(o._y), _dir(o._dir), _face(o._face), _inverted(o._inverted), _cmd(o._cmd)
	{
		_setCpyIdx(o);
	}
	Finger &operator=(const Finger &o)
	{
		_l = o._l;
		_x = o._x;
		_y = o._y;
		_dir = o._dir;
		_face = o._face;
		_inverted = o._inverted;
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
		Loc::const_iterator found;
		for (Loc::const_iterator cit = _l.begin(); cit != _l.end(); ++cit)
		{
			if (cit->second == _face)
			{
				found = cit;
				break ;
			}
		}
		coord	c = found->first;
		int column = _x + 1 + (c.first * _face->_g[0].size());
		int row = _y + 1 + (c.second * _face->_g.size());
		return (1000 * row + 4 * column + _dir);
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

void searchRight(Loc::iterator &face, Loc &loc)
{
	if (face->first == coord(1, 0))
	{
		face->second->_right.first = loc[coord(2, 0)];
		face->second->_right.second = RIGHT;
		return;
	}
	if (face->first == coord(2, 0))
	{
		face->second->_right.first = loc[coord(1, 2)];
		face->second->_right.second = LEFT;
		return;
	}
	if (face->first == coord(1, 1))
	{
		face->second->_right.first = loc[coord(2, 0)];
		face->second->_right.second = UP;
		return;
	}
	if (face->first == coord(1, 2))
	{
		face->second->_right.first = loc[coord(2, 0)];
		face->second->_right.second = LEFT;
		return;
	}
	if (face->first == coord(0, 2))
	{
		face->second->_right.first = loc[coord(1, 2)];
		face->second->_right.second = RIGHT;
		return;
	}
	if (face->first == coord(0, 3))
	{
		face->second->_right.first = loc[coord(1, 2)];
		face->second->_right.second = UP;
		return;
	}
	throw (std::invalid_argument("bug searchRight\n"));
}

void searchLeft(Loc::iterator &face, Loc &loc)
{
	if (face->first == coord(1, 0))
	{
		face->second->_left.first = loc[coord(0, 2)];
		face->second->_left.second = RIGHT;
		return;
	}
	if (face->first == coord(2, 0))
	{
		face->second->_left.first = loc[coord(1, 0)];
		face->second->_left.second = LEFT;
		return;
	}
	if (face->first == coord(1, 1))
	{
		face->second->_left.first = loc[coord(0, 2)];
		face->second->_left.second = DOWN;
		return;
	}
	if (face->first == coord(1, 2))
	{
		face->second->_left.first = loc[coord(0, 2)];
		face->second->_left.second = LEFT;
		return;
	}
	if (face->first == coord(0, 2))
	{
		face->second->_left.first = loc[coord(1, 0)];
		face->second->_left.second = RIGHT;
		return;
	}
	if (face->first == coord(0, 3))
	{
		face->second->_left.first = loc[coord(1, 0)];
		face->second->_left.second = DOWN;
		return;
	}
	throw (std::invalid_argument("bug searchLeft\n"));
}

void searchUp(Loc::iterator &face, Loc &loc)
{
	if (face->first == coord(1, 0))
	{
		face->second->_up.first = loc[coord(0, 3)];
		face->second->_up.second = RIGHT;
		return;
	}
	if (face->first == coord(2, 0))
	{
		face->second->_up.first = loc[coord(0, 3)];
		face->second->_up.second = UP;
		return;
	}
	if (face->first == coord(1, 1))
	{
		face->second->_up.first = loc[coord(1, 0)];
		face->second->_up.second = UP;
		return;
	}
	if (face->first == coord(1, 2))
	{
		face->second->_up.first = loc[coord(1, 1)];
		face->second->_up.second = UP;
		return;
	}
	if (face->first == coord(0, 2))
	{
		face->second->_up.first = loc[coord(1, 1)];
		face->second->_up.second = RIGHT;
		return;
	}
	if (face->first == coord(0, 3))
	{
		face->second->_up.first = loc[coord(0, 2)];
		face->second->_up.second = UP;
		return;
	}
	throw (std::invalid_argument("bug searchUp\n"));
}

void searchDown(Loc::iterator &face, Loc &loc)
{
	if (face->first == coord(1, 0))
	{
		face->second->_down.first = loc[coord(1, 1)];
		face->second->_down.second = DOWN;
		return;
	}
	if (face->first == coord(2, 0))
	{
		face->second->_down.first = loc[coord(1, 1)];
		face->second->_down.second = LEFT;
		return;
	}
	if (face->first == coord(1, 1))
	{
		face->second->_down.first = loc[coord(1, 2)];
		face->second->_down.second = DOWN;
		return;
	}
	if (face->first == coord(1, 2))
	{
		face->second->_down.first = loc[coord(0, 3)];
		face->second->_down.second = LEFT;
		return;
	}
	if (face->first == coord(0, 2))
	{
		face->second->_down.first = loc[coord(0, 3)];
		face->second->_down.second = DOWN;
		return;
	}
	if (face->first == coord(0, 3))
	{
		face->second->_down.first = loc[coord(2, 0)];
		face->second->_down.second = DOWN;
		return;
	}
	throw (std::invalid_argument("bug searchDown\n"));
}

void doLinkFaces(Loc &loc)
{
	for (Loc::iterator it = loc.begin(); it != loc.end(); ++it)
	{
		searchRight(it, loc);
		searchDown(it, loc);
		searchLeft(it, loc);
		searchUp(it, loc);
	}
}

grid	subGrid(const grid &g, int _x, int limX, int _y, int limY)
{
	grid toReturn;
	for (int y = _y; y < limY; ++y)
	{
		row toPushTotoReturn;
		for (int x = _x; x < limX; ++x)
			toPushTotoReturn.push_back(g[y][x]);
		toReturn.push_back(toPushTotoReturn);
	}
	return toReturn;
}

void	parseFaces(const grid &g, Loc &facesLocation)
{
	int sizeSquare;
	int width;
	int high;
	if (g.size() > g[0].size())
	{
		if (2 * g.size() == 5 * g[0].size())
		{
			sizeSquare = g.size() / 5;
			high = 5;
			width = 2;
		}
		else {
			sizeSquare = g.size() / 4;
			high = 4;
			width = 3;
		}
	}
	else
	{
		if (5 * g[0].size() == 2 * g.size())
		{
			sizeSquare = g[0].size() / 5;
			high = 2;
			width = 5;
		}
		else
		{
			sizeSquare = g[0].size() / 4;
			high = 3;
			width = 4;
		}
	}
	for (int y = 0; y < high; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			int trueX = x * sizeSquare;
			int	trueY = y * sizeSquare;
			if (g[trueY][trueX] != ' ')
			{
				int limitX = trueX + sizeSquare;
				int limitY = trueY + sizeSquare;
				Face	*newFace = new Face(subGrid(g, trueX, limitX, trueY, limitY));
				facesLocation.insert(std::make_pair(coord(x, y), newFace));
			}
		}
	}
	doLinkFaces(facesLocation);
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
	Loc facesLocation;
	parseFaces(g, facesLocation);
	Finger	douah(facesLocation, cmd);
	douah.exe();
	std::cout << douah.returnResult() << " is result.\n";
	for (Loc::iterator it = facesLocation.begin(); it != facesLocation.end(); ++it)
		delete it->second;
	return 0;
}
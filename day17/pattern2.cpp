#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>

typedef std::vector<std::vector<char> >	grid;
typedef std::pair<int, int>	coord;

class PatternDetector
{

public:

	std::vector<coord>	stackA;
	std::vector<coord>	stackB;
	std::vector<coord>::iterator idxPattern;

	int patternSize;
	int	patternStart;
	int highPatternStart;
	int idxLine;
	bool lookingPattern;
	bool patternFound;

	PatternDetector() : patternSize(0), patternStart(0), highPatternStart(0), idxLine(0), lookingPattern(false), patternFound(false) {}
	PatternDetector(const PatternDetector &o) :	stackA(o.stackA), stackB(o.stackB),
												patternSize(o.patternSize), patternStart(o.patternStart),
												highPatternStart(o.highPatternStart), idxLine(o.idxLine), lookingPattern(o.lookingPattern),
												patternFound(o.patternFound)
												{
		idxPattern = stackA.begin() + (o.idxPattern - o.stackA.begin());
												}
	~PatternDetector() {}
	PatternDetector &operator=(const PatternDetector &o)
	{
		stackA = o.stackA;
		stackB = o.stackB;
		idxPattern = stackA.begin() + (o.idxPattern - o.stackA.begin());
		patternSize = o.patternSize;
		patternStart = o.patternStart;
		highPatternStart = o.highPatternStart;
		idxLine = o.idxLine;
		lookingPattern = o.lookingPattern;
		patternFound = o.patternFound;
		return *this;
	}

	bool add(int idx, int form)
	{
		if (patternFound)
			return true;

		++patternStart;
		coord	c(idx, form);
		if (!stackB.empty() && c == *stackB.begin()) {
			idxLine = idx;
			patternSize = stackB.size();
			patternFound = true;
			patternStart -= patternSize;
			return true;
		}
		std::vector<coord>::iterator it = stackA.end();
		while (it != stackA.begin() && c != *it)
			--it;
		if (it != stackA.begin())
		{
			if (!lookingPattern)
			{
				lookingPattern = true;
				idxPattern = it;
				stackB.push_back(c);
			}
			else
			{
				if (++idxPattern == it)
					stackB.push_back(c);
				else
				{
					stackB.clear();
					lookingPattern = false;
				}
			}
		}
		else
		{
			lookingPattern = false;
			stackB.clear();
		}
		stackA.push_back(c);
		return false;
	}
};

std::ostream &operator<<(std::ostream &o, const std::vector<char> &v)
{
	for (std::vector<char>::const_iterator cit = v.begin(); cit != v.end(); ++cit)
		o << *cit;
	return o;
}

std::ostream &operator<<(std::ostream &o, const grid &g)
{
	int i = g.size() - 1;
	for (grid::const_reverse_iterator crit = g.rbegin(); crit != g.rend(); ++crit)
		o << *crit << ' ' << i-- << '\n';
	return o;
}

coord	&operator+=(coord &lhs, const coord &rhs)
{
	lhs.first += rhs.first;
	lhs.second += rhs.second;
	return lhs;
}

std::vector<coord>	&operator+=(std::vector<coord> &lhs, const coord &rhs)
{
	for (std::vector<coord>::iterator it = lhs.begin(); it != lhs.end(); ++it)
		*it += rhs;
	return lhs;
}

grid	&operator+=(grid &g, const std::vector<coord> &v)
{
	for (std::vector<coord>::const_iterator cit = v.begin(); cit != v.end(); ++cit)
		g[cit->second][cit->first] = '#';
	return g;
}

std::vector<coord>	operator+(const std::vector<coord> &lhs, const coord &rhs)
{
	std::vector<coord>	newVec = lhs;
	return (newVec += rhs);
}

bool moveOk(const grid &g, const std::vector<coord> &v)
{
	for (std::vector<coord>::const_iterator cit = v.begin(); cit != v.end(); ++cit)
	{
		if (g[cit->second][cit->first] != '.')
			return false;
	}
	return true;
}

std::vector<char>	newLineGrid()
{
	std::vector<char>	newLine(9, '.');

	newLine[0] = '|';
	newLine[8] = '|';
	return newLine;
}

int newLRH(const grid &tetris)
{
	for (int idx = tetris.size() - 1; idx >= 0; --idx)
	{
		for (int i = 1; i < 8; i++)
		{
			if (tetris[idx][i] == '#')
			{
				return idx;
			}
		}
	}
	return 0;
}

void flatRock(grid &tetris, int &LRH, const std::string &pattern, std::string::size_type &idx)
{
	std::vector<coord>	flat;
	int startHeight = LRH + 4;
	for (int i = 3; i < 7; ++i)
		flat.push_back(coord(i, startHeight));
	bool canFall = true;
	while (canFall)
	{
		if (idx == pattern.size())
			idx = 0;
		coord dxdy;
		if (pattern[idx] == '<')
			dxdy = coord(-1, 0);
		else
			dxdy = coord(1, 0);
		if (moveOk(tetris, flat + dxdy))
			flat += dxdy;
		++idx;

		if (moveOk(tetris, flat + coord(0, -1)))
			flat += coord(0, -1);
		else
			canFall = false;
	}
	tetris += flat;
	LRH = newLRH(tetris);
	for (int i = tetris.size(); i < LRH + 8; ++i)
		tetris.push_back(newLineGrid());
}

void crossRock(grid &tetris, int &LRH, const std::string &pattern, std::string::size_type &idx)
{
	std::vector<coord>	cross;
	int startHeight = LRH + 4;
	cross.push_back(coord(4, startHeight));
	for (int i = 0; i < 3; ++i)
		cross.push_back(coord(i + 3, startHeight + 1));
	cross.push_back(coord(4, startHeight + 2));
	bool canFall = true;
	while (canFall)
	{
		if (idx == pattern.size())
			idx = 0;
		coord dxdy;
		if (pattern[idx] == '<')
			dxdy = coord(-1, 0);
		else
			dxdy = coord(1, 0);
		if (moveOk(tetris, cross + dxdy))
			cross += dxdy;
		++idx;

		if (moveOk(tetris, cross + coord(0, -1)))
			cross += coord(0, -1);
		else
			canFall = false;
	}
	tetris += cross;
	LRH = newLRH(tetris);
	for (int i = tetris.size(); i < LRH + 8; ++i)
		tetris.push_back(newLineGrid());
}

void angleRock(grid &tetris, int &LRH, const std::string &pattern, std::string::size_type &idx)
{
	std::vector<coord>	angle;
	int startHeight = LRH + 4;
	for (int i = 0; i < 3; ++i)
		angle.push_back(coord(i + 3, startHeight));
	for (int i = 1; i < 3; ++i)
		angle.push_back(coord(5, startHeight + i));
	bool canFall = true;
	while (canFall)
	{
		if (idx == pattern.size())
			idx = 0;
		coord dxdy;
		if (pattern[idx] == '<')
			dxdy = coord(-1, 0);
		else
			dxdy = coord(1, 0);
		if (moveOk(tetris, angle + dxdy))
			angle += dxdy;
		++idx;

		if (moveOk(tetris, angle + coord(0, -1)))
			angle += coord(0, -1);
		else
			canFall = false;
	}
	tetris += angle;
	LRH = newLRH(tetris);
	for (int i = tetris.size(); i < LRH + 8; ++i)
		tetris.push_back(newLineGrid());
}

void lineRock(grid &tetris, int &LRH, const std::string &pattern, std::string::size_type &idx)
{
	std::vector<coord>	line;
	int startHeight = LRH + 4;
	for (int i = 0; i < 4; ++i)
		line.push_back(coord(3, startHeight + i));
	bool canFall = true;
	while (canFall)
	{
		if (idx == pattern.size())
			idx = 0;
		coord dxdy;
		if (pattern[idx] == '<')
			dxdy = coord(-1, 0);
		else
			dxdy = coord(1, 0);
		if (moveOk(tetris, line + dxdy))
			line += dxdy;
		++idx;

		if (moveOk(tetris, line + coord(0, -1)))
			line += coord(0, -1);
		else
			canFall = false;
	}
	tetris += line;
	LRH = newLRH(tetris);
	for (int i = tetris.size(); i < LRH + 8; ++i)
		tetris.push_back(newLineGrid());
}

void squareRock(grid &tetris, int &LRH, const std::string &pattern, std::string::size_type &idx)
{
	std::vector<coord>	square;
	int startHeight = LRH + 4;
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2; ++j)
			square.push_back(coord(j + 3, startHeight + i));
	}
	bool canFall = true;
	while (canFall)
	{
		if (idx == pattern.size())
			idx = 0;
		coord dxdy;
		if (pattern[idx] == '<')
			dxdy = coord(-1, 0);
		else
			dxdy = coord(1, 0);
		if (moveOk(tetris, square + dxdy))
			square += dxdy;
		++idx;

		if (moveOk(tetris, square + coord(0, -1)))
			square += coord(0, -1);
		else
			canFall = false;
	}
	tetris += square;
	LRH = newLRH(tetris);
	for (int i = tetris.size(); i < LRH + 8; ++i)
		tetris.push_back(newLineGrid());
}

int tetrisSim3(int end, int patternStart, const std::string &pattern)
{
	grid	tetris(10, newLineGrid());
	int turn = 1;
	int LRH = 0; // = lastRockHeight
	std::string::size_type	idx = 0;
	int h1;
	int h2;

	for (int i = 1; i < 9 ; ++i)
		tetris[0][i] = '-';
	tetris[0][0] = '+';
	tetris[0][8] = '+';

	while (turn < end + 1) {
		if (patternStart == turn)
			h1 = LRH;
		if (end == turn)
			h2 = LRH;
		if (idx == pattern.size())
			idx = 0;
		switch (turn % 5) {
			case 1:
				flatRock(tetris, LRH, pattern, idx);
				break;
			case 2:
				crossRock(tetris, LRH, pattern, idx);
				break;
			case 3:
				angleRock(tetris, LRH, pattern, idx);
				break;
			case 4:
				lineRock(tetris, LRH, pattern, idx);
				break;
			default:
				squareRock(tetris, LRH, pattern, idx);
		}
		++turn;
	}
	std::cout << "h1 = " << h1 << '\n';
	std::cout << "the high of the rest is " << h2 - h1 << '\n';
	return (h2 - h1);
}

coord tetrisSim2(const std::string &pattern, const PatternDetector &tool, std::ostream &out, std::ostream &out2)
{
	grid	tetris(10, newLineGrid());
	int turn = 1;
	int LRH = 0; // = lastRockHeight
	std::string::size_type	idx = 0;
	int h1;
	int h2;

	for (int i = 1; i < 9 ; ++i)
		tetris[0][i] = '-';
	tetris[0][0] = '+';
	tetris[0][8] = '+';

	while (turn < tool.patternSize + tool.patternStart + 1) {
		if (tool.patternStart == turn)
			h1 = LRH;
		if (tool.patternSize + tool.patternStart == turn)
			h2 = LRH;
		if (idx == pattern.size())
			idx = 0;
		switch (turn % 5) {
			case 1:
				flatRock(tetris, LRH, pattern, idx);
				break;
			case 2:
				crossRock(tetris, LRH, pattern, idx);
				break;
			case 3:
				angleRock(tetris, LRH, pattern, idx);
				break;
			case 4:
				lineRock(tetris, LRH, pattern, idx);
				break;
			default:
				squareRock(tetris, LRH, pattern, idx);
		}
		++turn;
	}
	out2 << tetris;
	std::cout << "h1 = " << h1 << '\n';
	std::cout << "the high of a pattern is " << h2 - h1 << '\n';
	return (coord(h1, h2 - h1));
}

PatternDetector tetrisSim(const std::string &pattern, std::ostream &out, std::ostream &out2)
{
	PatternDetector tool;
	grid	tetris(10, newLineGrid());
	int turn = 1;
	int LRH = 0; // = lastRockHeight
	std::string::size_type	idx = 0;

	for (int i = 1; i < 9 ; ++i)
		tetris[0][i] = '-';
	tetris[0][0] = '+';
	tetris[0][8] = '+';

	while (!tool.patternFound) {
		if (idx == pattern.size())
			idx = 0;
		switch (turn % 5) {
			case 1:
				tool.add(idx, 1);
				flatRock(tetris, LRH, pattern, idx);
				break;
			case 2:
				tool.add(idx, 2);
				crossRock(tetris, LRH, pattern, idx);
				break;
			case 3:
				tool.add(idx, 3);
				angleRock(tetris, LRH, pattern, idx);
				break;
			case 4:
				tool.add(idx, 4);
				lineRock(tetris, LRH, pattern, idx);
				break;
			default:
				tool.add(idx, 5);
				squareRock(tetris, LRH, pattern, idx);
		}
		++turn;
	}
	std::cout << "pattern size = " << tool.patternSize << '\n';
	std::cout << "pattern start at turn " << tool.patternStart << '\n';
	std::cout << "second pattern start at high " << LRH + 1 << '\n';
	out << tetris;
	return (tool);
}

int main(int ac, char **av)
{
	if (ac != 2) return 1;
	std::ifstream input(av[1]);
	if (input.fail()) return 1;

	std::filebuf fb;
	fb.open("output.log", std::ios::out);
	std::ostream out(&fb);

	std::filebuf fb2;
	fb2.open("output2.log", std::ios::out);
	std::ostream out2(&fb2);

	std::string	line;
	getline(input, line);
	if (line.back() == 13) line.erase(--line.end());

	PatternDetector tool = tetrisSim(line, out, out2);
	coord patternHigh = tetrisSim2(line, tool, out, out2);
	long long int elephant = 1000000000000;
	long long int elePatt = (elephant - tool.patternStart) / tool.patternSize;
	int	rest = (elephant - tool.patternStart) % tool.patternSize;
	std::cout << elephant << '\n';
	std::cout << elePatt << '\n';
	std::cout << rest << '\n';
	std::cout << patternHigh.first + (elePatt * patternHigh.second) << '\n';
	int restHigh = tetrisSim3(tool.patternStart + rest + 1, tool.patternStart, line);
	std::cout << restHigh << '\n';
	std::cout << patternHigh.first + (elePatt * patternHigh.second) + restHigh << " is result\n";
	return 0;
}
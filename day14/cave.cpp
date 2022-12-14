#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <exception>

typedef std::vector<std::vector<char> >	grid;
typedef std::pair<int, int>	coord;

coord	&operator+=(coord &lhs, const coord &rhs)
{
	lhs.second += rhs.second;
	lhs.first += rhs.first;
	return lhs;
}

std::ostream &operator<<(std::ostream &o, const coord &c)
{
	o << c.first << "|" << c.second;
	return o;
}

std::ostream &operator<<(std::ostream &o, const std::vector<coord> &v)
{
	for (std::vector<coord>::size_type i = 0; i < v.size(); ++i)
		o << v[i] << "--";
	return o;
}

std::ostream &operator<<(std::ostream &o, const std::vector<char> &v)
{
	for (std::vector<char>::size_type i = 0; i < v.size(); ++i)
		o << v[i];
	return o;
}

std::ostream &operator<<(std::ostream &o, const grid &g)
{
	for (grid::size_type i = 0; i < g.size(); ++i)
		o << g[i] << '\n';
	return o;
}

void parseLine(const std::string &l, std::vector<coord> &v)
{
	std::vector<std::string::size_type>	indexes;
	std::string::size_type	idx = l.find("->");
	while (idx != std::string::npos)
	{
		indexes.push_back(idx);
		idx = l.find("->", idx + 1);
	}
	idx = 0;
	for (std::vector<std::string::size_type>::size_type i = 0; i != indexes.size(); ++i)
	{
		std::string bout(l.substr(idx, indexes[i] - idx));
		v.push_back(std::make_pair(atoi(&bout[0]) - 450,
								   atoi(&bout[bout.find(',') + 1])));
		idx = indexes[i] + 2;
	}
	std::string bout(l.substr(idx));
	v.push_back(std::make_pair(atoi(&bout[0]) - 450,
							   atoi(&bout[bout.find(',') + 1])));
}

void	insertParsedLine(const std::vector<coord> &v, grid &g)
{
	if (!v.size())
		return;
	g[v[0].second][v[0].first] = '#';
	if (v.size() == 1)
		return;
	for (std::vector<coord>::size_type i = 0; i != v.size() - 1; ++i)
	{
		coord	a(v[i]);
		coord	b(v[i + 1]);
		coord	dxdy(0, 0);
		int dx = a.first - b.first;
		int	dy = a.second - b.second;
		if (dx && dy)
			throw std::invalid_argument("rocks must be aligned\n");
		if (dx < 0)
			dxdy = coord(1, 0);
		else if (dx > 0)
			dxdy = coord(-1, 0);
		else if (dy < 0)
			dxdy = coord(0, 1);
		else if (dy > 0)
			dxdy = coord(0, -1);
		while (a != b)
		{
			a += dxdy;
			g[a.second][a.first] = '#';
		}
	}
}

void pourTheSand(grid &c)
{
	bool	bottomReached = false;
	int 	count = 0;
	while (!bottomReached)
	{
		coord	sand(50, 0);
		bool	blocked = false;
		while (!blocked)
		{
			if (sand.second == 199)
			{
				bottomReached = true;
				break;
			}
			if (c[sand.second + 1][sand.first] == '.')
				sand += coord(0, 1);
			else if (c[sand.second + 1][sand.first - 1] == '.')
				sand += coord(-1, 1);
			else if (c[sand.second + 1][sand.first + 1] == '.')
				sand += coord(1, 1);
			else
			{
				blocked = true;
				c[sand.second][sand.first] = 'o';
			}
		}
		++count;
	}
	std::cout << "units of sand blocked : " << count - 1 << '\n';
}

int main(int ac, char **av)
{
	if (ac != 2)	return 1;
	std::ifstream input(av[1]);
	if (input.fail())	return 1;
	std::string line;

	std::filebuf	fb;
	fb.open("output_empty", std::ios::out);
	std::ostream	outputEmpty(&fb);

	std::filebuf	fb2;
	fb2.open("output_full", std::ios::out);
	std::ostream	outputFull(&fb2);

	grid	cave(200, std::vector<char>(100, '.'));
	cave[0][50] = '+';
	while (!input.eof())
	{
		getline(input, line);
		if (line.back() == 13) line.erase(--line.end());
		std::vector<coord>	parsedLine;
		parseLine(line, parsedLine);
		insertParsedLine(parsedLine, cave);
	}
	outputEmpty << cave;
	pourTheSand(cave);
	outputFull << cave;
	return 0;
}
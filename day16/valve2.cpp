#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <exception>
#include <algorithm>
#include <cmath>

class Node
{

public :

	typedef std::map<Node *, int>	children;
	typedef std::set<std::string>	toolToParse;

	std::string	_name;
	children	_children;
	toolToParse	_tool;
	int _valve;
	int _time;
	Node() : _valve(0), _time(0) {};
	Node(const std::string name, int valve) : _name(name), _valve(valve), _time(0) {}
	Node(const Node &o) : _name(o._name), _children(o._children), _valve(o._valve), _time(o._time) {}
	~Node() {}
	Node &operator=(const Node &o)
	{
		_name = o._name;
		_children = o._children;
		_valve = o._valve;
		_time = o._time;
		return *this;
	}

	bool operator<(const Node &rhs)	const	{return (_name < rhs._name);}
};

std::ostream &operator<<(std::ostream &o, const Node &n)
{
	o << "Valve name : " << n._name << " with pressure " << n._valve << '\n';
	o << "status : ";
	if (n._time)
		o << "opened at time " << n._time << '\n';
	else
		o << "not open\n";
	o << "Different path|time : ";
	Node::children::const_iterator cit = n._children.begin();
	while (cit != n._children.end())
	{
		o << cit->first->_name << '|' << cit->second;
		if (++cit != n._children.end())
			o << " or ";
		else
			o << '\n';
	}
	return o;
}

typedef std::map<std::string, Node *>	maze;
typedef std::map<std::string, Node *>::iterator	iterator;
typedef std::map<std::string, Node *>::const_iterator	const_iterator;

void	parseLine(const std::string &line, std::map<std::string, Node *> &maze)
{
	bool onlyOne = false;
	std::string::size_type idx1 = line.find("Valve ");
	std::string::size_type idx2 = line.find('=', idx1);
	std::string::size_type idx3 = line.find("valves ", idx2);
	if (idx3 == std::string::npos) {
		idx3 = line.find("valve ", idx2);
		onlyOne = true;
	}
	if (idx1 == std::string::npos || idx2 == std::string::npos || idx3 == std::string::npos)
		throw (std::invalid_argument("1 bad file format\n"));

	idx1 += 6;
	idx2 += 1;
	Node	*newNode = new Node(line.substr(idx1, 2), atoi(&line[idx2]));

	idx3 += 7;
	if (onlyOne)
		--idx3;

	if (idx3 > line.size())
		throw (std::invalid_argument("2 bad file format\n"));
	newNode->_tool.insert(line.substr(idx3, 2));

	for (std::string::size_type idx = line.find(',', idx3); idx != std::string::npos; idx = line.find(',', idx)){
		idx += 2;
		if (idx > line.size())
			throw (std::invalid_argument("3 bad file format\n"));
		newNode->_tool.insert(line.substr(idx, 2));
	}
	maze.insert(std::make_pair(newNode->_name, newNode));
}

void doTheLink(Node	*n)
{
	Node::children::iterator it = n->_children.begin();
	Node	*lhs = it->first;
	int 	lhsDist = it->second;
	++it;
	Node	*rhs = it->first;
	int		dist = it->second + lhsDist;

	std::pair<Node *, int>	childLhs = std::make_pair(lhs, dist);
	std::pair<Node *, int>	childRhs = std::make_pair(rhs, dist);

	rhs->_children.erase(n);
	std::pair<Node::children::iterator, bool> result = rhs->_children.insert(childLhs);
	if (!result.second)
		result.first->second = std::min(result.first->second, dist);

	lhs->_children.erase(n);
	std::pair<Node::children::iterator, bool> result2 = lhs->_children.insert(childRhs);
	if (!result2.second)
		result2.first->second = std::min(result2.first->second, dist);
}

void deleteUselessPipes(std::map<std::string, Node *> &maze)
{
	bool	changeOccured = true;
	while (changeOccured) {
		changeOccured = false;
		iterator itM = maze.begin();
		while(itM != maze.end()) {
			Node::children::iterator	found = itM->second->_children.find(itM->second);
			if (found != itM->second->_children.end())
			{
				changeOccured = true;
				itM->second->_children.erase(found);
			}
			if (itM != maze.begin() && itM->second->_valve == 0 && itM->second->_children.size() <= 2)
			{
				changeOccured = true;
				iterator itNext = itM;
				++itNext;
				switch (itM->second->_children.size())
				{
					case 1:
						itM->second->_children.begin()->first->_children.erase(itM->second);
						break;
					case 2:
						doTheLink(itM->second);
						break;
					default:
						throw (std::invalid_argument("I did sh*t\n"));
				}
				delete (itM->second);
				maze.erase(itM);
				itM = itNext;
			}
			else
				++itM;
		}
	}
}

void openUselessValves(std::map<std::string, Node *> &maze)
{
	for (iterator it = maze.begin(); it != maze.end(); ++it)
	{
		if (!(it->second->_valve))
			it->second->_time = 1;
	}
}

void simplifyMaze(std::map<std::string, Node *> &maze)
{
	for (iterator it = maze.begin(); it != maze.end(); ++it)
	{
		for (std::set<std::string>::iterator itTool = it->second->_tool.begin(); itTool != it->second->_tool.end(); ++itTool)
		{
			iterator found = maze.find(*itTool);
			if (found == maze.end())
				throw (std::invalid_argument("non sens maze input\n"));
			it->second->_children.insert(std::make_pair(found->second, 1));
		}
		it->second->_tool.clear();
	}
	deleteUselessPipes(maze);
	openUselessValves(maze);
}

std::ostream &operator<<(std::ostream &o, const std::map<std::string, Node *> &maze)
{
	for (const_iterator it = maze.begin(); it != maze.end(); ++it)
		o << *(it->second) << '\n';
	return o;
}

int calcPressure(const maze &maze)
{
	int	pressure = 0;
	for (const_iterator cit = maze.begin(); cit != maze.end(); ++cit)
		pressure += cit->second->_valve * cit->second->_time;
	return pressure;
}

bool canDoBetter(const maze &maze, int best, int time)
{
	int better = calcPressure(maze);
	std::multiset<int>	valves;
	for (const_iterator it = maze.begin(); it != maze.end(); ++it)
	{
		if (it->second->_time == 0)
			valves.insert(it->second->_valve);
	}
	std::multiset<int>::const_reverse_iterator crit = valves.rbegin();
	while (time >= 0 && crit != valves.rend())
	{
		better += time * *crit;
		++crit;
		better += time * *crit;
		++crit;
		time -= 3;
	}
	return (better > best);
}

void doTheSims(maze &maze, Node *pos1, Node *pos2, int time1, int time2, int time, int &simReport)
{
	if (time <= 0) {
		int result = calcPressure(maze);
		if (result > simReport) {
			std::cerr << result << '\n';
			simReport = result;
		}
		//simReport = std::max(simReport, calcPressure(maze));
	}
	else if (time1 && time2)
	{
		int	timeSpend = std::min(time1, time2);
		doTheSims(maze, pos1, pos2, time1 - timeSpend, time2 - timeSpend, time - timeSpend, simReport);
	}
	else if (!time1 && time2)
	{
		if (pos1->_time == 0)
		{
			pos1->_time = time - 1;
			doTheSims(maze, pos1, pos2, 0, time2 - 1, time - 1, simReport);
			pos1->_time = 0;
		}
		for (Node::children::iterator it = pos1->_children.begin(); it != pos1->_children.end(); ++it)
		{
			if (canDoBetter(maze, simReport, time - 1))
				doTheSims(maze, it->first, pos2, it->second - 1, time2 - 1, time - 1, simReport);
		}
	}
	else if (time1 && !time2)
	{
		if (pos2->_time == 0)
		{
			pos2->_time = time - 1;
			doTheSims(maze, pos1, pos2, time1 - 1, 0, time - 1, simReport);
			pos2->_time = 0;
		}
		for (Node::children::iterator it = pos2->_children.begin(); it != pos2->_children.end(); ++it)
		{
			if (canDoBetter(maze, simReport, time - 1))
				doTheSims(maze, pos1, it->first, time1 - 1, it->second - 1, time - 1, simReport);
		}
	}
	else {
		if (pos1->_time == 0 && pos2->_time == 0 && pos1 != pos2) {
			pos1->_time = time - 1;
			pos2->_time = time - 1;
			doTheSims(maze, pos1, pos2, 0, 0, time - 1, simReport);
			pos1->_time = 0;
			pos2->_time = 0;
		}
		if (pos1->_time == 0 && pos1 != pos2) {
			pos1->_time = time - 1;
			for (Node::children::iterator it = pos2->_children.begin(); it != pos2->_children.end(); ++it) {
				if (canDoBetter(maze, simReport, time - 1))
					doTheSims(maze, pos1, it->first, 0, it->second - 1, time - 1, simReport);
			}
			pos1->_time = 0;
		}
		if (pos2->_time == 0) {
			pos2->_time = time - 1;
			for (Node::children::iterator it = pos1->_children.begin(); it != pos1->_children.end(); ++it) {
				if (canDoBetter(maze, simReport, time - 1))
					doTheSims(maze, it->first, pos2, it->second - 1, 0, time - 1, simReport);
			}
			pos2->_time = 0;
		}
		for (Node::children::iterator it1 = pos1->_children.begin(); it1 != pos1->_children.end(); ++it1)
		{
			for (Node::children::iterator it2 = pos2->_children.begin(); it2 != pos2->_children.end(); ++it2)
			{
				if (canDoBetter(maze, simReport, time - std::min(it1->second, it2->second)))
					doTheSims(maze, it1->first, it2->first, it1->second - 1, it2->second - 1, time - 1, simReport);
			}
		}
	}
}

int	main(int ac, char **av)
{
	if (ac != 2) return 1;
	std::ifstream input(av[1]);
	if (input.fail()) return 1;

	std::filebuf fb;
	fb.open("output.log", std::ios::out);
	std::ostream out(&fb);

	std::map<std::string, Node *>	maze;
	std::string line;
	while (!input.eof())
	{
		getline(input, line);
		if (line.back() == 13) line.erase(--line.end());
		parseLine(line, maze);
	}
	simplifyMaze(maze);
	out << "simplified maze : \n" << maze;
	fb.close();
	int	simReport = 0;
	doTheSims(maze, maze.begin()->second, maze.begin()->second, 0, 0, 9, simReport);
	std::cout << simReport << '\n';
	for (iterator it = maze.begin(); it != maze.end(); ++it)
		delete it->second;
	return 0;
}
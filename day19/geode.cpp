#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

class Compo
{
public:
	int ore;
	int	clay;
	int obsidian;

	Compo() : ore(0), clay(0), obsidian(0) {}
	Compo(const Compo &o) : ore(o.ore), clay(o.clay), obsidian(o.obsidian) {}
	Compo(int ore_, int clay_, int obsidian_) : ore(ore_), clay(clay_), obsidian(obsidian_) {}
	~Compo() {}
	Compo &operator=(const Compo &o)
	{
		ore = o.ore;
		clay = o.clay;
		obsidian = o.obsidian;
		return *this;
	}

	Compo	&operator+=(const Compo &o)
	{
		ore += o.ore;
		clay += o.clay;
		obsidian += o.obsidian;
		return *this;
	}

	Compo	&operator-=(const Compo &o)
	{
		ore -= o.ore;
		clay -= o.clay;
		obsidian -= o.obsidian;
		return *this;
	}

	Compo	operator+(const Compo &o)	const
	{
		return (Compo(*this) += o);
	}

	Compo	operator-(const Compo &o)	const
	{
		return (Compo(*this) -= o);
	}

	bool operator>=(const Compo &o)	const
	{
		if (ore < o.ore)
			return false;
		if (clay < o.clay)
			return false;
		if (obsidian < o.obsidian)
			return false;
		return true;
	}
};

std::ostream &operator<<(std::ostream &o, const Compo &c)
{
	o << c.ore << '|' << c.clay << '|' << c.obsidian;
	return o;
}

class Blueprint
{
public :
	Compo	oreRobotCompo;
	Compo	clayRobotCompo;
	Compo	obsidianRobotCompo;
	Compo	geodeRobotCompo;

	Blueprint() :	oreRobotCompo(0, 0, 0), clayRobotCompo(0, 0, 0),
					obsidianRobotCompo(0, 0, 0), geodeRobotCompo(0, 0, 0) {}
	Blueprint(const Blueprint &o) :	oreRobotCompo(o.oreRobotCompo), clayRobotCompo(o.clayRobotCompo),
									obsidianRobotCompo(o.obsidianRobotCompo), geodeRobotCompo(o.geodeRobotCompo) {}
	Blueprint(Compo ore, Compo clay, Compo obsidian, Compo geode)
		:	oreRobotCompo(ore), clayRobotCompo(clay), obsidianRobotCompo(obsidian), geodeRobotCompo(geode)
	{}
	~Blueprint() {}
	Blueprint &operator=(const Blueprint &o)
	{
		oreRobotCompo = o.oreRobotCompo;
		clayRobotCompo = o.clayRobotCompo;
		obsidianRobotCompo = o.obsidianRobotCompo;
		geodeRobotCompo = o.geodeRobotCompo;
		return *this;
	}

	int BestAmountOfGeodeInTime(int time)	const;
};

class Sim
{
public :
	Blueprint _b;
	int	_t;
	int _ore;
	int _clay;
	int _obs;
	int _robotOre;
	int _robotClay;
	int _robotObs;
	int _robotGeo;
	int	_geo;

	Sim() : _t(0), _ore(0), _clay(0), _obs(0), _robotOre(1), _robotClay(0), _robotObs(0), _robotGeo(0), _geo(0) {}
	Sim(const Blueprint &b, int t) : _b(b), _t(t), _ore(0), _clay(0), _obs(0), _robotOre(1), _robotClay(0), _robotObs(0), _robotGeo(0), _geo(0) {}
	Sim(const Sim &o) : _b(o._b), _t(o._t), _ore(o._ore), _clay(o._clay), _obs(o._obs), _robotOre(o._robotOre), _robotClay(o._robotClay),
	_robotObs(o._robotObs), _robotGeo(o._robotGeo), _geo(o._geo) {}
	~Sim() {}
	Sim &operator=(const Sim &o)
	{
		_b = o._b;
		_t = o._t;
		_ore = o._ore;
		_clay = o._clay;
		_obs = o._obs;
		_robotOre = o._robotOre;
		_robotClay = o._robotClay;
		_robotObs = o._robotObs;
		_robotGeo = o._robotGeo;
		_geo = o._geo;
		return *this;
	}

	void passTime(int time)
	{
		_t -= time;
		_ore += time * _robotOre;
		_clay += time * _robotClay;
		_obs += time * _robotObs;
		_geo += time * _robotGeo;
	}

	void buildRobot(int robot)
	{
		switch (robot)
		{
			case 1:
				_robotOre += 1;
				break;
			case 2:
				_robotClay += 1;
				break;
			case 3:
				_robotObs += 1;
				break;
			default :
				_robotGeo += 1;
		}
	}

	void destroyRobot(int robot)
	{
		switch (robot)
		{
			case 1:
				_robotOre -= 1;
				break;
			case 2:
				_robotClay -= 1;
				break;
			case 3:
				_robotObs -= 1;
				break;
			default :
				_robotGeo -= 1;
		}
	}

	int canDoOre()	const
	{
		if (_t == 1) return (-1);
		/*if ((_robotOre >= _b.clayRobotCompo.ore || _robotClay >= _b.obsidianRobotCompo.clay || _robotObs >= _b.geodeRobotCompo.obsidian) &&
			(_robotOre >= _b.obsidianRobotCompo.ore || _robotObs >= _b.geodeRobotCompo.obsidian) &&
			_robotOre >= _b.geodeRobotCompo.ore)
			return (-1);*/
		if ((_robotObs >= _b.geodeRobotCompo.obsidian || ((_robotOre >= _b.clayRobotCompo.ore || _robotClay >= _b.obsidianRobotCompo.clay &&
			_robotOre >= _b.obsidianRobotCompo.ore))) && _robotOre >= _b.geodeRobotCompo.ore)
			return (-1);
		if (Compo(_ore, 0, 0) >= _b.oreRobotCompo) return (0);
		int timeToGet = ((_b.oreRobotCompo.ore - _ore) / _robotOre);
		//std::cout << "robotOre : ttgOre = " << timeToGet << '\n';
		if ((_b.oreRobotCompo.ore - _ore) % _robotOre)
			++timeToGet;
		if (timeToGet >= _t)	return (-1);
		return timeToGet;
	}

	int canDoClay()	const
	{
		if (_t == 1) return (-1);
		if (_robotClay >= _b.obsidianRobotCompo.clay || _robotObs >= _b.geodeRobotCompo.obsidian)
			return (-1);
		if (Compo(_ore, 0, 0) >= _b.clayRobotCompo) return (0);
		int timeToGet = ((_b.clayRobotCompo.ore - _ore) / _robotOre);
		//std::cout << "robotClay : ttgOre = " << timeToGet << '\n';
		if ((_b.clayRobotCompo.ore - _ore) % _robotOre)
			++timeToGet;
		if (timeToGet >= _t)	return (-1);
		return timeToGet;
	}

	int	canDoObs()	const
	{
		if (_t == 1) return (-1);
		if (_robotObs >= _b.geodeRobotCompo.obsidian || !_robotClay)
			return (-1);
		if (Compo(_ore, _clay, 0) >= _b.obsidianRobotCompo) return (0);
		int timeToGetOre = ((_b.obsidianRobotCompo.ore - _ore) / _robotOre);
		int timeToGetClay = ((_b.obsidianRobotCompo.clay - _clay) / _robotClay);
		//std::cout << "robotObs : _t = " << _t << " ttgOre = " << timeToGetOre << " ttgClay = " << timeToGetClay << '\n';
		if (timeToGetClay > timeToGetOre)
		{
			if ((_b.obsidianRobotCompo.clay - _clay) % _robotClay)
				++timeToGetClay;
			if (timeToGetClay >= _t) return (-1);
			return timeToGetClay;
		}
		if ((_b.obsidianRobotCompo.ore - _ore) % _robotOre)
			++timeToGetOre;
		if (timeToGetOre >= _t)	return (-1);
		return timeToGetOre;
	}

	int canDoGeo()	const
	{
		if (_t == 1) return (-1);
		if (!_robotObs)
			return (-1);
		if (Compo(_ore, 0, _obs) >= _b.geodeRobotCompo) return (0);
		int timeToGetOre = ((_b.geodeRobotCompo.ore - _ore) / _robotOre);
		int timeToGetObs = ((_b.geodeRobotCompo.obsidian - _obs) / _robotObs);
		//std::cout << "robotGeo : _t = " << _t << " Ore = " << _ore << " ttgOre = " << timeToGetOre << " ttgObs = " << timeToGetObs << '\n';
		if (timeToGetObs > timeToGetOre)
		{
			if ((_b.geodeRobotCompo.obsidian - _obs) % _robotObs)
				++timeToGetObs;
			if (timeToGetObs >= _t) return (-1);
			return timeToGetObs;
		}
		if ((_b.geodeRobotCompo.ore - _ore) % _robotOre)
			++timeToGetOre;
		if (timeToGetOre >= _t)	return (-1);
		return timeToGetOre;
	}

};

void stupidForce(Sim &sim, int &result);
void recursive(Sim &sim, int &result, int time, int robot)
{
	sim.passTime(time);
	sim.buildRobot(robot);
	stupidForce(sim, result);
	sim.passTime(- (time));
	sim.destroyRobot(robot);
}

void stupidForce(Sim &sim, int &result) {
	if (sim._t < 0)
	{
		std::cout << "erreur\n";
		return ;
	}
	if (sim._t == 0)
	{
		if (sim._geo > result)
		{
			result = sim._geo;
			std::cout << result << '\n';
		}
		return ;
	}
	int doOre = sim.canDoOre();
	int doClay = sim.canDoClay();
	int	doObs = sim.canDoObs();
	int doGeo = sim.canDoGeo();
	if (sim._t == 24)
	{
		std::cout << "doOre = " << doOre << '\n';
		std::cout << "doClay = " << doClay << '\n';
		std::cout << "doObs = " << doObs << '\n';
		std::cout << "doGeo = " << doGeo << '\n';
	}
	if (doOre != -1)
		recursive(sim, result, doOre, 1);
	if (doClay != -1)
		recursive(sim, result, doClay, 2);
	if (doObs != -1)
		recursive(sim, result, doObs, 3);
	if (doGeo != -1)
		recursive(sim, result, doObs, 4);
	if (doOre == -1 && doClay == -1 && doObs == -1 && doGeo == -1)
	{
		sim.passTime(sim._t);
		stupidForce(sim, result);
	}
}

std::ostream &operator<<(std::ostream &o, const Blueprint &b)
{
	o << "\tOre robot compo : " << b.oreRobotCompo;
	o << "\n\tClay robot compo : " << b.clayRobotCompo;
	o << "\n\tObsidian robot compo : " << b.obsidianRobotCompo;
	o << "\n\tGeode robot compo : " << b.geodeRobotCompo;
	return o;
}

int Blueprint::BestAmountOfGeodeInTime(int time) const {
	Sim	sim(*this, time);
	int result = 0;
	stupidForce(sim, result);
	return result;
}

std::ostream &operator<<(std::ostream &o, const std::vector<Blueprint> &v)
{
	int i = 1;
	o << "\t\tore|clay|obsidian";
	for (std::vector<Blueprint>::const_iterator cit = v.begin(); cit != v.end(); ++cit)
		o << "\n\nBlueprint " << i++ << " :\n" << *cit;
	return o;
}

void parseLines(const std::vector<std::string> &lines, std::vector<Blueprint> &book)
{
	if (lines.empty())
		return ;
	int	idx = 0;
	int	foundCount = 0;
	std::string::size_type	found = 0;
	Blueprint	blue;
	while (idx < lines.size() && foundCount < 4)
	{
		while (found != std::string::npos && foundCount < 4)
		{
			found = lines[idx].find("costs", found);
			if (found != std::string::npos)
			{
				++foundCount;
				found += 5;
				switch(foundCount)
				{
					case 1:
						blue.oreRobotCompo = Compo(atoi(&(lines[idx][found])), 0, 0);
						break;
					case 2:
						blue.clayRobotCompo = Compo(atoi(&(lines[idx][found])), 0, 0);
						break;
					case 3:
						blue.obsidianRobotCompo = Compo(atoi(&(lines[idx][found])), atoi(&(lines[idx][lines[idx].find("and", found) + 3])), 0);
						break;
					default:
						blue.geodeRobotCompo = Compo(atoi(&(lines[idx][found])), 0, atoi(&(lines[idx][lines[idx].find("and", found) + 3])));
				}
			}
			else
			{
				++idx;
				found = 0;
			}
		}
	}
	book.push_back(blue);
}

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << "./exe + <file> + time\n";
		return 1;
	}
	std::ifstream input(av[1]);
	if (input.fail())
	{
		std::cerr << "intput fail.\n";
		return 1;
	}
	int	time = atoi(av[2]);
	if (time <= 0)
	{
		std::cerr << "time error.\n";
		return 1;
	}

	std::string line;
	std::vector<Blueprint>	book;
	while (!input.eof())
	{
		std::vector<std::string>	lines;
		if (!line.empty() && line.front() == 'B')
			lines.push_back(line);
		while (getline(input, line) && line.front() != 'B')
		{
			if (line.back() == 13) line.erase(--line.end());
			lines.push_back(line);
		}
		parseLines(lines, book);
	}
	if (!line.empty() && line.front() == 'B')
		parseLines(std::vector<std::string>(1, line), book);
	int	result = 0;
	int idx = 1;
//	std::cout << book << "\n\n";
	for (std::vector<Blueprint>::const_iterator cit = book.begin(); cit != book.end(); ++cit)
	{
		result += (idx * cit->BestAmountOfGeodeInTime(time));
		++idx;
	}
	std::cout << "\n\n" << result << " is result\n";
	return 0;
}
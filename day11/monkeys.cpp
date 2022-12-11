#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <string>

class Monkey
{

		Monkey() {}
		Monkey &operator=(const Monkey &other) {return (*this);}

	public :

		typedef std::vector<int>::iterator	ite;
		typedef std::vector<int>::reverse_iterator	rev_ite;

		std::vector<Monkey>	*link;
		std::vector<int>	items;
		int					add;
		int					mult;
		int					divTest;
		int					trueMonkeyIdx;
		int					falseMonkeyIdx;
		int					busyCount;

		Monkey(const Monkey &other) :	link(other.link), items(other.items), add(other.add), mult(other.mult),
										divTest(other.divTest), trueMonkeyIdx(other.trueMonkeyIdx),
										falseMonkeyIdx(other.falseMonkeyIdx), busyCount(other.busyCount)
		{}

		Monkey(std::vector<Monkey> *linkk, const std::vector<int> &startingItems, int addd, int multt, int divTestt, int trueMonkeyIdxx, int falseMonkeyIdxx)
			: link(linkk), items(startingItems), add(addd), mult(multt), divTest(divTestt), trueMonkeyIdx(trueMonkeyIdxx), falseMonkeyIdx(falseMonkeyIdxx), busyCount(0)
		{}

		void	doTurn()
		{
			for (rev_ite it = items.rbegin(); it != items.rend(); ++it)
			{
				++busyCount;
				*it += add;
				if (mult == 0)
					*it *= *it;
				else
					*it *= mult;
				*it /= 3;
				if (*it % divTest)
					passItToBuddy(it, link->at(falseMonkeyIdx));
				else
					passItToBuddy(it, link->at(trueMonkeyIdx));
			}
		}

		void	passItToBuddy(rev_ite it, Monkey &buddy)
		{
			buddy.items.push_back(*it);
			items.erase(--it.base());
		}
};


std::ostream	&operator<<(std::ostream &o, const std::vector<int> &v)
{
	for (std::vector<int>::const_iterator it = v.begin(); it != v.end(); ++it)
		o << " " << *it;
	return o;
}

std::ostream	&operator<<(std::ostream &o, const Monkey &m)
{
	o << "start : " << m.items << '\n';
	o << "op : add = " << m.add << " | mult = " << m.mult << '\n';
	o << "divTest = " << m.divTest << '\n';
	o << "T | F monkeys : " << m.trueMonkeyIdx << " | " << m.falseMonkeyIdx << "\n\n"; 
	return o;
}

std::ostream	&operator<<(std::ostream &o, const std::vector<Monkey> &v)
{
	int	i = 0;
	for (std::vector<Monkey>::const_iterator it = v.begin(); it != v.end(); ++it)
		o << "----------Monkey " << i++ << "----------\n" << *it;
	return o;
}

void	doTheGame(std::vector<Monkey> &monkeys)
{
	for (int i = 0; i < 20; ++i)
	{
		for (std::vector<Monkey>::iterator it = monkeys.begin(); it != monkeys.end(); ++it)
			it->doTurn();
		std::cout << monkeys;
	}
	std::set<int>	busySet;
	for (std::vector<Monkey>::iterator it = monkeys.begin(); it != monkeys.end(); ++it)
			busySet.insert(it->busyCount);
	std::cout << "Monkeys business is : " << *(busySet.rbegin()) * *(++busySet.rbegin()) <<'\n';
}

int main(int ac, char **av)
{
	if (ac != 2) return 1;
	std::ifstream	input(av[1]);
	if (input.fail()) return 1;

	std::vector<Monkey>	monkeys;
	//BEGINNING OF PARSING
	std::string	line;
	while (!input.eof())
	{
		std::vector<std::string>	parse;
		for (int i = 0; i < 6; ++i)
		{
			getline(input, line);
			if (line.back() == 13) line.erase(--line.end());
			if (line.empty())
				getline(input, line);
			parse.push_back(line);
		}
		std::vector<int>		items;
		std::string::size_type	idx = parse[1].find(": ");
		while (idx != std::string::npos)
		{
			items.push_back(atoi(&(parse[1][idx + 1])));
			idx = parse[1].find(',', idx + 1);
		}

		int	mult = 1;
		int	add = 0;
		int	divTest;
		int	trueM;
		int	falseM;
		if (parse[2][23] == '*')
			mult = atoi(&(parse[2][24]));
		else
			add = atoi(&(parse[2][24]));
		divTest = atoi(&(parse[3][20]));
		trueM = atoi(&(parse[4][28]));
		falseM = atoi(&(parse[5][29]));
		monkeys.push_back(Monkey(&monkeys, items, add, mult, divTest, trueM, falseM));
	}
	//END OF PARSING
	std::cout << monkeys;
	doTheGame(monkeys);
	return 0;
}
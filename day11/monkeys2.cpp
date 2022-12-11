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

		typedef std::vector<long long>::iterator	ite;
		typedef std::vector<long long>::reverse_iterator	rev_ite;

		std::vector<Monkey>	*link;
		std::vector<long long>	items;
		int					add;
		int					mult;
		int					divTest;
		int					trueMonkeyIdx;
		int					falseMonkeyIdx;
		long				busyCount;

		Monkey(const Monkey &other) :	link(other.link), items(other.items), add(other.add), mult(other.mult),
										divTest(other.divTest), trueMonkeyIdx(other.trueMonkeyIdx),
										falseMonkeyIdx(other.falseMonkeyIdx), busyCount(other.busyCount)
		{}

		Monkey(std::vector<Monkey> *linkk, const std::vector<long long> &startingItems, int addd, int multt, int divTestt, int trueMonkeyIdxx, int falseMonkeyIdxx)
			: link(linkk), items(startingItems), add(addd), mult(multt), divTest(divTestt), trueMonkeyIdx(trueMonkeyIdxx), falseMonkeyIdx(falseMonkeyIdxx), busyCount(0)
		{}

		void	doTurn()
		{
			for (rev_ite it = items.rbegin(); it != items.rend(); ++it)
			{
				*it %= 9699690; // to not let the number be ridiculuously big, I keep the rest of it by a division by 1 * 2 * 3 * 5 * 7 * 11 * 13 * 17 * 19.
				++busyCount;
				*it += add;
				if (mult == 0)
					*it *= *it;
				else
					*it *= mult;
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

void	doTheGame(std::vector<Monkey> &monkeys)
{
	for (int i = 0; i < 10000; ++i)
	{
		for (std::vector<Monkey>::iterator it = monkeys.begin(); it != monkeys.end(); ++it)
			it->doTurn();
	}
	std::set<long>	busySet;
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
	//BEGINNIG OF PARSING
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
		std::vector<long long>		items;
		std::string::size_type	idx = parse[1].find(": ");
		while (idx != std::string::npos)
		{
			items.push_back(atol(&(parse[1][idx + 1])));
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
	doTheGame(monkeys);

	return 0;
}
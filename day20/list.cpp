#include <list>
#include <iostream>
#include <fstream>
#include <set>

typedef std::list<int>::iterator it;

int main(int ac, char **av)
{
	if (ac != 2) return 1;
	std::ifstream input(av[1]);
	if (input.fail()) return 1;

	std::string line;
	std::list<int>	lst;
	while (!input.eof())
	{
		getline(input, line);
		if (line.back() == 13) line.erase(--line.end());
		lst.push_back(stoi(line));
	}
	it current = lst.begin();
	std::set<int *>	placed;
	placed.insert(&*current);
	int	i = 0;
	int	idx = 0;
	int	size = lst.size();
	while (i < size)
	{
		int jump = *current;
		if (jump < 0)
		{
			jump *= -1;
			it next = current;
			int rect = 0;
			++rect;
			++next;
			++idx;
			while (placed.find(&*next) != placed.end())
			{
				++rect;
				++idx;
				++next;
			}
			if (idx - rect <= jump)
			{
				jump -= idx - rect;
				it here = lst.end();
				while (jump)
				{
					--here;
					--jump;
				}
				lst.insert(here, *current);
				--here;
				placed.insert(&*here);
				lst.erase(current);
			}
			else
			{
				it here = current;
				while (jump)
				{
					--here;
					--jump;
				}
				lst.insert(here, *current);
				--here;
				placed.insert(&*here);
				lst.erase(current);
			}
			current = next;
		}
		else if (jump > 0) {
			it next = current;
			int	rect = 0;
			++rect;
			++next;
			++idx;
			while (placed.find(&*next) != placed.end())
			{
				++rect;
				++idx;
				++next;
			}
			int	depassage = size + idx - rect;
			if (idx - rect <= jump)
			{
				jump -= idx - rect;

			}
			else
			{

			}
			current = next;
		}
		else
		{
			++current;
			while (placed.find(&*current) != placed.end())
				++current;
		}
		i++;
	}
}
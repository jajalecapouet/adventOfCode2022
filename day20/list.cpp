#include <list>
#include <iostream>
#include <fstream>
#include <set>

typedef std::list<int>::iterator it;
typedef std::list<int>::const_iterator cit;

bool operator<(const it &lhs, const it &rhs)
{
	return (&*lhs < &*rhs);
}

std::ostream &operator<<(std::ostream &o, const std::list<int> &lst)
{
	for (cit cit = lst.begin(); cit != lst.end(); ++cit)
		o << *cit << " | ";
	return o;
}

template< class T >
struct cmpItList : std::binary_function<T, T, bool> // I don't know why std::less can't do the work, but whatever...
{
	bool operator() (const T& x, const T& y) const {return x<y;}
};

it placeLeft(std::list<int> &lst, it &toPlace, int jump)
{
	int nb = -jump;
	jump %= (lst.size() - 1);
	it newPlace = toPlace;
	++newPlace;
	lst.erase(toPlace);
	while (jump && newPlace != lst.begin())
	{
		--jump;
		--newPlace;
	}
	if (jump)
	{
		newPlace = lst.end();
		while (jump)
		{
			--jump;
			--newPlace;
		}
	}
	lst.insert(newPlace, nb);
	return (--newPlace);
}

it placeRight(std::list<int> &lst, it &toPlace, int jump)
{
	int nb = jump;
	jump %= (lst.size() - 1);
	it newPlace = toPlace;
	++newPlace;
	lst.erase(toPlace);
	while (jump && newPlace != lst.end())
	{
		--jump;
		++newPlace;
	}
	if (jump)
	{
		newPlace = lst.begin();
		while (jump)
		{
			--jump;
			++newPlace;
		}
	}
	lst.insert(newPlace, nb);
	return (--newPlace);
}

it place(std::list<int> &lst, it &toPlace, int jump)
{
	if (jump < 0)
		return placeLeft(lst, toPlace, -jump);
	if (jump > 0)
		return placeRight(lst, toPlace, jump);
	return toPlace;
}

void	reOrder(std::list<int> &lst, it &current, std::set<it, cmpItList<it> > &placed)
{
	if (current == lst.end())
		return ;
	if (placed.find(current) == placed.end())
	{
		it toPlace = current++;
		toPlace = place(lst, toPlace, *toPlace);
		placed.insert(toPlace);
	}
	else
		++current;
	reOrder(lst, current, placed);
}

void findResult(const std::list<int> &lst)
{
	cit current = lst.begin();
	int result = 0;
	while (*current != 0)
		++current;
	for (int k = 0; k < 3; ++k)
	{
		for (int i = 0; i < 1000; ++i)
		{
			++current;
			if (current == lst.end())
				current = lst.begin();
		}
		result += *current;
		std::cout << (k + 1) * 1000 << "th element : " << *current << '\n';
	}
	std::cout << result << " is result.\n";
}

int main(int ac, char **av)
{
	if (ac != 2) return 1;
	std::ifstream input(av[1]);
	if (input.fail()) return 1;

	std::string line;
	std::list<int>	lst;
	int max = 0;
	int min = 0;
	while (!input.eof())
	{
		getline(input, line);
		if (line.back() == 13) line.erase(--line.end());
		int read = stoi(line);
		if (read > max)
			max = read;
		else if (read < min)
			min = read;
		lst.push_back(read);
	}
	it current = lst.begin();
	std::set<it, cmpItList<it> >	placed;
	reOrder(lst, current, placed);
	if (lst.size() < 20)
		std::cout << lst << '\n';
	findResult(lst);
	return 0;
}
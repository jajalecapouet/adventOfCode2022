#include <list>
#include <iostream>
#include <fstream>

typedef std::list<long long *>::iterator it;
typedef std::list<long long *>::const_iterator cit;

bool operator<(const it &lhs, const it &rhs)
{
	return (&*lhs < &*rhs);
}

std::ostream &operator<<(std::ostream &o, const std::list<long long *> &lst)
{
	for (cit cit = lst.begin(); cit != lst.end(); ++cit)
		o << *cit << ": " << **cit << " | ";
	return o;
}

it placeLeft(std::list<long long *> &lst, it &toPlace, long long jump)
{
	long long *nb = *toPlace;
	jump %= lst.size() - 1;
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

it placeRight(std::list<long long *> &lst, it &toPlace, long long jump)
{
	long long *nb = *toPlace;
	jump %= lst.size() - 1;
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

it place(std::list<long long *> &lst, it &toPlace, long long jump)
{
	if (jump < 0)
		return placeLeft(lst, toPlace, -jump);
	if (jump > 0)
		return placeRight(lst, toPlace, jump);
	return toPlace;
}

void	reOrder(std::list<long long *> &lst, const std::list<long long *> &order)
{
	it current = lst.begin();
	for (cit cit = order.begin(); cit != order.end(); ++cit)
	{
		while (*current != *cit)
		{
			if (current == lst.end())
				current = lst.begin();
			else
				++current;
		}
		it toPlace = current++;
		place(lst, toPlace, **toPlace);
	}
}

void findResult(const std::list<long long *> &lst)
{
	cit current = lst.begin();
	long long result = 0;
	while (**current != 0)
		++current;
	for (int k = 0; k < 3; ++k)
	{
		for (int i = 0; i < 1000; ++i)
		{
			++current;
			if (current == lst.end())
				current = lst.begin();
		}
		result += **current;
		std::cout << (k + 1) * 1000 << "th element : " << **current << '\n';
	}
	std::cout << result << " is result.\n";
}

int main(int ac, char **av)
{
	if (ac != 2) return 1;
	std::ifstream input(av[1]);
	if (input.fail()) return 1;

	std::string line;
	std::list<long long *>	lst;
	long long max = 0;
	long long min = 0;
	while (!input.eof())
	{
		getline(input, line);
		if (line.back() == 13) line.erase(--line.end());
		long long *read = new long long(stoll(line) * 811589153);
		if (*read > max)
			max = *read;
		else if (*read < min)
			min = *read;
		lst.push_back(read);
	}
	std::cout << "range [" << min << ';' << max << "]\n";
	if (lst.size() < 20)
		std::cout << lst << '\n';
	std::list<long long *>	order = lst;
	reOrder(lst, order);
	reOrder(lst, order);
	reOrder(lst, order);
	reOrder(lst, order);
	reOrder(lst, order);
	reOrder(lst, order);
	reOrder(lst, order);
	reOrder(lst, order);
	reOrder(lst, order);
	reOrder(lst, order);
	if (lst.size() < 20)
		std::cout << lst << '\n';
	findResult(lst);
	return 0;
}
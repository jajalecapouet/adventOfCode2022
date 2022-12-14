#include <iostream>
#include <fstream>
#include <set>
#include <utility>
#include <tuple>
#include <string>
#include <vector>

//Class which can contains T type or Vector of instances of this own class
template<class T>
class MyTuple {
public :

	bool					isTuple;
	T						val;
	std::vector<MyTuple>	hug;

	typedef T	value_type;
	typedef typename std::vector<MyTuple>::iterator					ite;
	typedef typename std::vector<MyTuple>::const_iterator			cite;
	typedef typename std::vector<MyTuple>::reverse_iterator			rite;
	typedef typename std::vector<MyTuple>::const_reverse_iterator	crite;

	MyTuple() : isTuple(1), val(T()) {}
	MyTuple(const MyTuple &o) : isTuple(o.isTuple), val(o.val), hug(o.hug) {}
	MyTuple(T i) : isTuple(0), val(i) {}
	MyTuple(const std::vector<MyTuple> &vec) : isTuple(1), val(T()), hug(vec) {}
	MyTuple &operator=(const MyTuple &o)
	{
		isTuple = o.isTuple;
		val = o.val;
		hug = o.hug;
		return o;
	}

	void setVal(value_type &valSet)
	{
		isTuple = false;
		val = valSet;
	}

	void setHug(std::vector<MyTuple<T> > hugSet)
	{
		hug = hugSet;
		isTuple = true;
	}

	void addTuple(const MyTuple &other)
	{
		hug.push_back(other);
		isTuple = true;
	}

	bool isEmpty() const
	{
		return (isTuple && hug.empty());
	}

	ite		begin() {return hug.begin();}
	cite	begin() const {return hug.begin();}
	rite	rbegin() {return hug.rbegin();}
	crite	rbegin() const {return hug.rbegin();}

	ite		end() {return hug.end();}
	cite	end() const {return hug.end();}
	rite	rend() {return hug.rend();}
	crite	rend() const {return hug.rend();}

	int compareTuples(MyTuple &other)
	{
		//std::cout << *this << " | ";
		//std::cout << other << '\n';
		if (!isTuple && !other.isTuple && val < other.val)
			return 1;
		if (!isTuple && !other.isTuple && val > other.val)
			return -1;
		if (!isTuple && !other.isTuple && val == other.val)
			return 0;
		if (isEmpty() && !other.isEmpty())
			return 1;
		if (!isEmpty() && other.isEmpty())
			return -1;
		if (!isTuple && other.isTuple) {
			addTuple(*this);
			return compareTuples(other);
		}
		if (isTuple && !other.isTuple) {
			other.addTuple(other);
			return compareTuples(other);
		}
		if (isTuple && other.isTuple)
		{
			ite	it1 = begin();
			ite	it2 = other.begin();
			while (it1 != end() && it2 != other.end())
			{
				int res = it1->compareTuples(*it2);
				if (res == -1)
					return -1;
				if (res == 1)
					return 1;
				++it1;
				++it2;
			}
			if (it2 != other.end())
				return 1;
			else if (it1 != end())
				return -1;
		}
		return 0;
	}

	bool operator<=(const MyTuple &other) const
	{
		MyTuple	thisCpy(*this);
		MyTuple	otherCpy(other);
		if (thisCpy.compareTuples(otherCpy) == -1)
			return false;
		return true;
	}
};

std::ostream &operator<<(std::ostream &o, const MyTuple<int> t)
{
	if (!t.isTuple) {
		o << t.val;
		return o;
	}
	if (t.isEmpty())
		return o;
	o << '[';
	for (MyTuple<int>::cite it = t.begin(); it != t.end(); ++it)
	{

		o << *it;
		if (it + 1 != t.end())
		o << ',';
	}
	o << ']';
	return o;
}

bool noComaOutside(std::string str)
{
	int degreeOfEncapsulation = 0;
	for (std::string::size_type idx = 0; idx < str.size(); ++idx) {
		if (str[idx] == '[')
			++degreeOfEncapsulation;
		else if (str[idx] == ']')
			--degreeOfEncapsulation;
		else if (str[idx] == ',' && !degreeOfEncapsulation)
			return false;
	}
	return true;
}

MyTuple<int> parseTuple(const std::string &str) {
	if (str.empty())
		return (MyTuple<int>());

	std::string::size_type idx1 = str.find('[', 0);
	std::string::size_type idx2 = str.find(',', 0);
	if (idx1 == std::string::npos && idx2 == std::string::npos)
		return (MyTuple<int>(std::stoi(str)));

	std::vector<std::string::size_type> indexes;
	int degreeOfEncapsulation = 0;
	for (std::string::size_type idx = 0; idx < str.size(); ++idx) {
		if (str[idx] == '[')
			++degreeOfEncapsulation;
		else if (str[idx] == ']')
			--degreeOfEncapsulation;
		else if (str[idx] == ',' && !degreeOfEncapsulation)
			indexes.push_back(idx);
	}
	if (indexes.empty()) {
		std::string deeper(str.substr(1, str.size() - 2));
		if (noComaOutside(deeper))
			return (MyTuple<int>(std::vector<MyTuple<int> >(1, parseTuple(deeper))));
		else
			return (parseTuple(deeper));
	}
	else
	{
		std::vector<MyTuple<int> >	hug;
		std::string::size_type	start = 0;
		for (std::vector<std::string::size_type>::iterator it = indexes.begin(); it != indexes.end(); ++it)
		{
			hug.push_back(parseTuple(str.substr(start, (*it) - start)));
			start = (*it) + 1;
		}
		hug.push_back(parseTuple(str.substr(start)));
		return MyTuple<int>(hug);
	}
}

int main(int ac, char **av)
{
	if (ac != 2) return 1;
	std::ifstream input(av[1]);
	if (input.fail()) return 1;

	int	score2 = 1;
	int	score6 = 2; //because 6 > 2
	MyTuple<int>	divider2 = parseTuple("[[2]]");
	MyTuple<int>	divider6 = parseTuple("[[6]]");
	std::string line;
	while (!input.eof()) {
		getline(input, line);
		if (line.back() == 13) line.erase(--line.end());
		if (!line.empty()) {
			MyTuple<int> lineTuple = parseTuple(line);
			if (lineTuple <= divider2) {
				++score2;
				++score6;
			}
			else if (lineTuple <= divider6)
				++score6;
		}
	}
	std::cout << score2 * score6 << '\n';
}
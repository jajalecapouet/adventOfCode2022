#include <iostream>
#include <fstream>
#include <map>
#include <ctype.h>
#include <cstdint>

#define PLUS 0;
#define MINUS 1;
#define MUL 2;
#define DIV 3;


// the next overload operator<< wad found at https://gist.github.com/nfarring/b11dde6ada832cad4d606ff1d8ae6691
std::ostream& operator<<(std::ostream& os, const __int128 i) noexcept
{
	std::ostream::sentry s(os);
	if (s) {
		unsigned __int128 tmp = i < 0 ? -i : i;
		char buffer[128];
		char *d = std::end(buffer);
		do {
			--d;
			*d = "0123456789"[tmp % 10];
			tmp /= 10;
		} while (tmp != 0);
		if (i < 0) {
			--d;
			*d = '-';
		}
		int len = std::end(buffer) - d;
		if (os.rdbuf()->sputn(d, len) != len) {
			os.setstate(std::ios_base::badbit);
		}
	}
	return os;
}

class Tree
{
	bool _isLeaf;
	__int128 _LEAF; // leaf and left are too confusable
	Tree	*_left;
	Tree	*_right;
	int _operator;

public:
	Tree() : _isLeaf(false), _operator(0), _LEAF(0), _left(0), _right(0) {}
	Tree(int n) : _isLeaf(true), _operator(0), _LEAF(n), _left(0), _right(0) {}
	Tree(Tree *left, Tree *right, __int128 op) : _isLeaf(false), _left(left), _right(right), _operator(op) {}
	Tree(const Tree &o) : _isLeaf(o._isLeaf), _LEAF(o._LEAF), _left(o._left), _right(o._right), _operator(o._operator) {}
	Tree &operator=(const Tree &o)
	{
		_isLeaf = o._isLeaf;
		_LEAF = o._LEAF;
		_left = o._left;
		_right = o._right;
		_operator = o._operator;
		return *this;
	}

	bool getIsLeaf()	const	{return _isLeaf;}
	__int128 getLeaf()	const	{return _LEAF;}
	Tree	*getLeft()	const	{return _left;}
	Tree	*getRight()	const	{return _right;}
	char getOperator()	const
	{
		switch (_operator)
		{
			case 0:
				return '+';
			case 1:
				return '-';
			case 2:
				return '*';
			default:
				return '/';
		}
	}

	void assignNb(__int128 nb)
	{
		_isLeaf = true;
		_LEAF = nb;
	}

	void assignOp(int op)
	{
		_operator = op;
	}

	void assignLeft(Tree *l)
	{
		_left = l;
	}

	void assignRight(Tree *r)
	{
		_right = r;
	}

	void assignAll(Tree *l, Tree *r, int op)
	{
		_left = l;
		_right = r;
		_operator = op;
	}

	__int128 doOp()	const
	{
		if (_isLeaf)
			return _LEAF;
		switch (_operator)
		{
			case 0:
				return (_left->doOp() + _right->doOp());
			case 1:
				return (_left->doOp() - _right->doOp());
			case 2:
				return (_left->doOp() * _right->doOp());
			default:
				return (_left->doOp() / _right->doOp());
		}
	}
};

std::ostream &operator<<(std::ostream &o, const Tree &t)
{
	if (t.getIsLeaf())
		o << t.getLeaf();
	else
		o << t.getLeft() << ' ' << t.getOperator() << ' ' << t.getRight();
	return o;
}

typedef std::map<std::string, Tree *>	Mp;

std::ostream &operator<<(std::ostream &o, const Mp &m)
{
	for (Mp::const_iterator cit = m.begin(); cit != m.end(); ++cit)
	{
		o << cit->first << ": " << cit->second << ": " << *(cit->second) << '\n';
	}
	return o;
}

void parseLine(const std::string &str, Mp &mappedTrees) {
	std::string::size_type idx = str.find(':');
	if (idx == std::string::npos)
		throw (std::invalid_argument("bad file\n"));
	std::string name = str.substr(0, idx);
	std::string nameLeft;
	std::string nameRight;
	bool isLeaf = false;
	int nb;
	if (isdigit(str[6]))
	{
		isLeaf = true;
		nb = atoi(&str[6]);
	}
	int op;
	if (!isLeaf)
	{
		switch (str[11])
		{
			case '+':
				op = PLUS;
				break;
			case '-':
				op = MINUS;
				break;
			case '*':
				op = MUL;
				break;
			case '/':
				op = DIV;
				break;
			default:
				throw (std::invalid_argument("expected operator\n"));
		}
	}
	Mp::iterator foundName = mappedTrees.find(name);
	if (foundName != mappedTrees.end())
	{
		Tree	*exists = foundName->second;
		if (isLeaf)
			exists->assignNb(nb);
		else
		{
			Tree	*left;
			Tree	*right;
			nameLeft = str.substr(6, 4);
			nameRight = str.substr(13, 4);
			Mp::iterator foundNameLeft = mappedTrees.find(nameLeft);
			Mp::iterator foundNameRight = mappedTrees.find(nameRight);
			if (foundNameLeft != mappedTrees.end())
				left = foundNameLeft->second;
			else
			{
				left = new Tree();
				mappedTrees.insert(std::make_pair(nameLeft, left));
			}
			if (foundNameRight != mappedTrees.end())
				right = foundNameRight->second;
			else
			{
				right = new Tree();
				mappedTrees.insert(std::make_pair(nameRight, right));
			}
			exists->assignAll(left, right, op);
		}
	}
	else
	{
		Tree	*newTree;
		if (isLeaf)
			newTree = new Tree(nb);
		else
		{
			Tree	*left;
			Tree	*right;
			nameLeft = str.substr(6, 4);
			nameRight = str.substr(13, 4);
			Mp::iterator foundNameLeft = mappedTrees.find(nameLeft);
			Mp::iterator foundNameRight = mappedTrees.find(nameRight);
			if (foundNameLeft != mappedTrees.end())
				left = foundNameLeft->second;
			else
			{
				left = new Tree();
				mappedTrees.insert(std::make_pair(nameLeft, left));
			}
			if (foundNameRight != mappedTrees.end())
				right = foundNameRight->second;
			else
			{
				right = new Tree();
				mappedTrees.insert(std::make_pair(nameRight, right));
			}
			newTree = new Tree(left, right, op);
		}
		mappedTrees.insert(std::make_pair(name, newTree));
	}
}

bool humanIsHere(Tree *human, Tree *here)
{
	if (here == human)
		return true;
	else if (here->getIsLeaf())
		return false;
	else
		return (humanIsHere(human, here->getLeft()) || humanIsHere(human, here->getRight()));
}

void elevator(__int128 toFind, Tree *search, Tree *human)
{
	if (search == human)
	{
		human->assignNb(toFind);
		return;
	}
	Tree	*newSearch;
	__int128	newToFind;
	if (humanIsHere(human, search->getLeft()))
	{
		newSearch = search->getLeft();
		switch (search->getOperator())
		{
			case '+':
				newToFind = toFind - search->getRight()->doOp();
				break;
			case '-':
				newToFind = toFind + search->getRight()->doOp();
				break;
			case '*':
				newToFind = toFind / search->getRight()->doOp();
				break;
			default:
				newToFind = toFind * search->getRight()->doOp();
		}
	}
	else
	{
		newSearch = search->getRight();
		switch (search->getOperator()) {
			case '+':
				newToFind = toFind - search->getLeft()->doOp();
				break;
			case '-':
				newToFind = search->getLeft()->doOp() - toFind;
				break;
			case '*':
				newToFind = toFind / search->getLeft()->doOp();
				break;
			default:
				newToFind = search->getLeft()->doOp() / toFind;
		}
	}
	elevator(newToFind, newSearch, human);
}

int	main(int ac, char **av)
{
	if (ac != 2) return 1;
	std::ifstream input(av[1]);
	if (input.fail()) return 1;

	Mp	mappedTrees;
	std::string	line;
	while (!input.eof())
	{
		getline(input, line);
		if (line.back() == 13) line.erase(--line.end());
		parseLine(line, mappedTrees);
	}
	//std::cout << mappedTrees << '\n';
	Mp::iterator 	root = mappedTrees.find(std::string("root"));
	Mp::iterator	humanIt = mappedTrees.find(std::string("humn"));
	if (root == mappedTrees.end())
		throw (std::invalid_argument("wrong file: \"root\" not found.\n"));
	if (humanIt == mappedTrees.end())
		throw (std::invalid_argument("wrong file: \"humn\" not found.\n"));

	Tree	*human = humanIt->second;
	Tree	*left = root->second->getLeft();
	Tree	*right = root->second->getRight();
	Tree	*search;
	__int128	toFind;

	if (humanIsHere(human, left))
	{
		toFind = right->doOp();
		search = left;
	}
	else if (humanIsHere(human, right))
	{
		toFind = left->doOp();
		search = right;
	}
	else
		throw (std::invalid_argument("????????\n"));

	elevator(toFind, search, human);
	std::cout << right->doOp() << " | " << left->doOp() << '\n';
	std::cout << human->getLeaf() << " is result\n";
	return 0;
}
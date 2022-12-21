#include <iostream>
#include <fstream>
#include <map>
#include <ctype.h>

#define PLUS 0;
#define MINUS 1;
#define MUL 2;
#define DIV 3;

class Tree
{
	bool _isLeaf;
	int _LEAF; // leaf and left are too confusable
	Tree	*_left;
	Tree	*_right;
	int _operator;

public:
	Tree() : _isLeaf(false), _operator(0), _LEAF(0), _left(0), _right(0) {}
	Tree(int n) : _isLeaf(true), _operator(0), _LEAF(n), _left(0), _right(0) {}
	Tree(Tree *left, Tree *right, int op) : _isLeaf(false), _left(left), _right(right), _operator(op) {}
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
	int getLeaf()	const	{return _LEAF;}
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

	void assignNb(int nb)
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

	long long int doOp()	const
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
	Mp::const_iterator 	root = mappedTrees.find(std::string("root"));
	if (root == mappedTrees.end())
		throw (std::invalid_argument("wrong file: \"root\" not found.\n"));
	long long int result = root->second->doOp();
	std::cout << result << " is result\n";
	for (Mp::iterator it = mappedTrees.begin(); it != mappedTrees.end(); ++it)
		delete it->second;
	return 0;
}
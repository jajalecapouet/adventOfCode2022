#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <new>
#include <string>

class Node
{
		Node(const Node &other);
		Node &operator=(const Node &other);
		Node();
		void	_killChilds(std::pair<const std::string, Node *>	val)
		{
			delete (val.second);
		}

	public :

		int								size;
		int								recursiveSize;
		int								trueSize;
		std::map<std::string, Node *>	dirs;
		Node							*father;
		std::string						name;

		Node(const std::string &dirName) : size(0), recursiveSize(0), trueSize(0), father(0), name(dirName) {}
		~Node() {
			std::map<std::string, Node *>::iterator	end = dirs.end();
			for (std::map<std::string, Node *>::iterator it = dirs.begin(); it != end; ++it)
				_killChilds(*it);
		}

		void	addDir(Node *other)			{dirs.insert(std::make_pair(other->name, other));}
		void	addFileSize(int	fileSize)	{size += fileSize;}
		void	applyRecursiveSize()
		{
			trueSize = recursiveSize + size;
			if (father)
				father->recursiveSize += trueSize;
		}
};

void	fillNode(const std::string &line, Node *node)
{
	if (!line.compare(0, 1, "d"))
	{
		Node	*newNode = new Node(line.substr(4));
		node->addDir(newNode);
		newNode->father = node;
	}
	else
		node->addFileSize(std::stoi(line));
}

void	findLowerSpaceToFree(Node *node, int &result, int spaceNeeded)
{
	if (node->trueSize < result)
		result = node->trueSize;
	std::map<std::string, Node *>::iterator	end = node->dirs.end();
	for (std::map<std::string, Node *>::iterator it = node->dirs.begin(); it != end; ++it)
	{
		if (it->second->trueSize >= spaceNeeded)
			findLowerSpaceToFree(it->second, result, spaceNeeded);
	}
}

int main(int ac, char **av)
{
	if (ac != 2)		{return 1;}
	std::ifstream		input(av[1]);
	if (input.fail())	{return 1;}

	Node	*root = new Node("/");
	Node	*current = root;
	std::string	line;

	getline(input, line); // ignore "$ cd /"
	while (!input.eof())
	{
		line.clear();
		getline(input, line);
		if (line.back() == 13)
			line.erase(--line.end()); // boring subject's input behaviour correction
		if (!line.compare(0, 3, "$ c"))
		{
			std::string	cd(line.substr(5));
			if (!cd.compare(0, 2, ".."))
			{
				current->applyRecursiveSize();
				current = current->father;
			}
			else
				current = current->dirs[line.substr(5)];
		}
		else if (!line.compare(0, 3, "$ l"))
			continue;
		else
			fillNode(line, current);
	}
	while (current) //going up until root "/"
	{
		current->applyRecursiveSize();
		current = current->father;
	}
	int	result = 99999999;
	int spaceNeeded = 30000000 - (70000000 - root->trueSize);
	current = root;
	findLowerSpaceToFree(current, result, spaceNeeded);
	std::cout << result << std::endl;
	delete (root);
}
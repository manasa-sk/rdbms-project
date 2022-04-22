#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

const int order = 4;

class Node
{
private:
	int index;
	string fileName;
	Node *next;
	friend class LinkedList;
};

class LinkedList
{

public:
	LinkedList();
	void insert(int x);
	int size();
	int check(int x);
	void splitList(LinkedList &list);
	int front();
	void deleteFront();
    void searchList(int x);
	void disList();

private:
	Node *head;
	int n;
};

class TreeNode
{
public:
	TreeNode();

private:
	bool isLeaf;
	vector<TreeNode *> child;
	TreeNode *next;
	LinkedList keys;
	friend class BPlusTree;
};

class BPlusTree
{
public:
	BPlusTree();
	void insertIndex(int index, TreeNode *temp, TreeNode *parent);
	void splitTree(TreeNode *temp, TreeNode *parent);
	void deleteIndex(int index);
	void dis(TreeNode *);
	TreeNode *getRoot();

private:
	int height;
	TreeNode *root;
};

TreeNode::TreeNode()
{
	LinkedList();
}

LinkedList::LinkedList() : head(NULL), n(0) {}

void LinkedList::insert(int x) // function adds the number x such that the linked list remains sorted
{
	Node *temp = new Node;
	temp->index = x;
	++n;

	if (head == NULL || head->index > x)
	{
		temp->next = head;
		head = temp;
	}
	else
	{
		Node *ptr = head;

		while (ptr->next != NULL && ptr->next->index < x)
			ptr = ptr->next;
		temp->next = ptr->next;
		ptr->next = temp;
	}
}

int LinkedList::size()
{
	return n;
}

int LinkedList::check(int x) // function returns a number which determines which child we need to go to
{
	Node *ptr = head;
	int i;

	for (i = 0; i < n; i++)
	{
		if (x < ptr->index)
			break;
		ptr = ptr->next;
	}

	return i;
}

void LinkedList::splitList(LinkedList &list)
{
	Node *ptr = list.head;

	for (int i = 0; i < (order / 2) - 1; i++) // ptr points to last element of first list
		ptr = ptr->next;

	head = ptr->next;
	ptr->next = NULL;
	n = (order + 1) / 2;
	list.n = order / 2;
}

int LinkedList::front()
{
	return head->index;
}

void LinkedList::deleteFront()
{
	Node *ptr = head;
	head = head->next;
	delete ptr;
	--n;
}

void LinkedList::searchList(int x)
{

}

BPlusTree::BPlusTree() : root(NULL), height(0) {}

void BPlusTree::insertIndex(int index, TreeNode *temp, TreeNode *parent)
{
	if (root == NULL)
	{
		root = new TreeNode;
		root->isLeaf = true;
		root->keys.insert(index);
		root->next = NULL;
	}
	else
	{
		if (parent == NULL)
		{
			parent = new TreeNode;
			parent->isLeaf = false;
		}

		if (temp->isLeaf == false)
		{
			int i = temp->keys.check(index);
			insertIndex(index, temp->child[i], temp);
		}
		else
			temp->keys.insert(index);

		if (temp->keys.size() >= order)
		{
			splitTree(temp, parent);

			if (temp == root)
				root = parent;
		}
	}
}

void BPlusTree::splitTree(TreeNode *temp, TreeNode *parent)
{
	++height;

	TreeNode *newLeaf = new TreeNode;
	newLeaf->keys.splitList(temp->keys);
	newLeaf->isLeaf = temp->isLeaf;
	parent->keys.insert(newLeaf->keys.front());

	if (temp->isLeaf == true)
	{
		newLeaf->next = temp->next;
		temp->next = newLeaf;	// to link the leaf nodes
	}
	else
	{
		newLeaf->keys.deleteFront();
		for (int i = (order / 2) + 1; i <= order; i++)
		{
			newLeaf->child.push_back(temp->child[i]);
			temp->child.erase(temp->child.begin() + i);
		}
	}

	int i;
	i = parent->keys.check(temp->keys.front());
	parent->child.insert(parent->child.begin() + i, temp);
	i = parent->keys.check(newLeaf->keys.front());
	parent->child.insert(parent->child.begin() + i, newLeaf);
}

void BPlusTree::deleteIndex(int index)
{
	
}

void BPlusTree::dis(TreeNode *temp)
{
	if (temp != NULL)
	{
		temp->keys.disList();
		cout << "\n";
		if (temp->isLeaf == false)
		{
			for (int i = 0; i < temp->keys.size() + 1; i++)
			{
				dis(temp->child[i]);
			}
		}
	}
}

void LinkedList::disList()
{
	Node *ptr = head;

	while (ptr != NULL)
	{
		cout << ptr->index << " ";
		ptr = ptr->next;
	}
}

TreeNode *BPlusTree::getRoot()
{
	return root;
}

int main()
{
	BPlusTree b;
	b.insertIndex(1, b.getRoot(), NULL);
	b.insertIndex(4, b.getRoot(), NULL);
	b.insertIndex(7, b.getRoot(), NULL);
	b.insertIndex(10, b.getRoot(), NULL);
	b.insertIndex(17, b.getRoot(), NULL);
	b.insertIndex(21, b.getRoot(), NULL);
	b.insertIndex(31, b.getRoot(), NULL);
	b.insertIndex(25, b.getRoot(), NULL);
	b.insertIndex(19, b.getRoot(), NULL);
	b.insertIndex(20, b.getRoot(), NULL);
	b.insertIndex(28, b.getRoot(), NULL);
	b.insertIndex(42, b.getRoot(), NULL);
	b.dis(b.getRoot());
}
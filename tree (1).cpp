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
    void insert(int x);
    void splitTree(TreeNode *temp, TreeNode *ancestor[], int j);
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

BPlusTree::BPlusTree() : root(NULL), height(0) {}

void BPlusTree::insert(int index)
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
        TreeNode *temp = root;
        TreeNode *ancestor[height]; // array of pointers to ancestors of leaf node, first element is root, last is parent of leaf node
        int j = 0;                  // stores the number of ancestors

        while (temp->isLeaf == false) // goes to appropriate leaf node while storing all ancestors
        {
            ancestor[j++] = temp;
            int i = temp->keys.check(index);
            temp = temp->child[i];
        }

        temp->keys.insert(index);
        cout << index << " inserted" << endl;

        if (temp->keys.size() == order) // leaf nodes may have only order - 1 elements so if it crosses call splitTree();
            splitTree(temp, ancestor, j);
    }
}

void BPlusTree::splitTree(TreeNode *temp, TreeNode *ancestor[], int j)
{
    TreeNode *parent;
    ++height;

    if (j == 0) // if the leaf node is the root
    {
        parent = new TreeNode;
        parent->isLeaf = false;
    }
    else
        parent = ancestor[--j];

    if (temp == root)
        root = parent;

    TreeNode *newLeaf = new TreeNode();
    newLeaf->keys.splitList(temp->keys);
    newLeaf->isLeaf = false;
    parent->keys.insert(newLeaf->keys.front());

    if (temp->isLeaf == true)
    {
        newLeaf->next = temp->next;
        temp->next = newLeaf;   // to link the
        newLeaf->isLeaf = true; // leaf nodes
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

    if (parent->keys.size() == order)
        splitTree(parent, ancestor, j);
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
    b.insert(1);
    b.insert(4);
    b.insert(7);
    b.insert(10);
    b.insert(17);
    b.insert(21);
    b.insert(31);
    b.insert(25);
    b.insert(19);
    b.insert(20);
    b.insert(28);
    b.insert(42);
    b.dis(b.getRoot());
}
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

const int order = 4;

class Node
{
private:
    int index;
    string name;
    Node *next;
    friend class LinkedList;
};

class LinkedList
{

public:
    LinkedList();
    void insert(int x, bool t);
    int size();
    int check(int x); // function returns a number which determines which child we need to go to
    void splitList(LinkedList &list);
    int front();
    void deleteNode(int x);
    string searchList(int x);
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
    TreeNode *next; // used only when the node is a leafNode
    LinkedList keys;
    friend class BPlusTree;
};

class BPlusTree
{
public:
    BPlusTree();
    void insertIndex(int index, TreeNode *temp, TreeNode *parent);
    void splitTree(TreeNode *temp, TreeNode *parent);
    void searchIndex(int index);
    void dis(TreeNode *temp);
    TreeNode *getRoot();

private:
    TreeNode *root;
};

TreeNode::TreeNode()
{
    LinkedList();
}

LinkedList::LinkedList() : head(NULL), n(0) {}

void LinkedList::insert(int x, bool t) // function adds the number x such that the linked list remains sorted
{
    Node *temp = new Node;
    temp->index = x;

    if (t == true)
    {
        string name = "a";
        cout << "Enter name:";
        cin >> name;
        temp->name = name;
    }

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

void LinkedList::deleteNode(int x)
{
    Node *ptr = head;

    if (x == 0)
        head = head->next;
    else
    {
        for (int i = 1; i < x; i++) // points to the node that is before the one that is to be deleted
            ptr = ptr->next;
        Node *temp = ptr;
        ptr = ptr->next;
        temp->next = ptr->next;
    }
    delete ptr;
    --n;
}

string LinkedList::searchList(int x)
{
    Node *ptr = head;

    while (ptr != NULL)
    {
        if (x == ptr->index)
            return ptr->name;
        ptr = ptr->next;
    }

    return "NOT FOUND";
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

BPlusTree::BPlusTree() : root(NULL) {}

void BPlusTree::insertIndex(int index, TreeNode *temp, TreeNode *parent)
{
    if (root == NULL) // tree hasn't been created yet so creating the tree by initializing root node
    {
        root = new TreeNode;
        root->isLeaf = true;
        root->keys.insert(index, true);
        root->next = NULL;
    }
    else
    {
        if (temp->isLeaf == false) // recursively traverse the tree till leaf node
        {
            int i = temp->keys.check(index);
            insertIndex(index, temp->child[i], temp);
        }
        else
            temp->keys.insert(index, true);

        if (temp->keys.size() >= order)
        {
            if (temp == root) // if root node needs to be split, parent will be null so new parent must be
            {                 // created and becomes the new root node
                parent = new TreeNode;
                parent->isLeaf = false;
                splitTree(temp, parent);
                root = parent;
                return;
            }
            splitTree(temp, parent);
        }
    }
}

void BPlusTree::splitTree(TreeNode *temp, TreeNode *parent)
{
    TreeNode *newLeaf = new TreeNode; // create new tree node and insert the firts element of it into parent
    newLeaf->keys.splitList(temp->keys);
    newLeaf->isLeaf = temp->isLeaf;
    parent->keys.insert(newLeaf->keys.front(), false);

    if (temp->isLeaf == true) // to link the leaf nodes
    {
        newLeaf->next = temp->next;
        temp->next = newLeaf;
    }
    else // front element is deleted for internal node splitting as it is redundant
    {
        newLeaf->keys.deleteNode(0);
        int mid = (order / 2) + 1;
        for (int i = mid; i <= order; i++) // to move the required child nodes from the original tree node
        {                                  // to the new tree node

            newLeaf->child.push_back(temp->child[mid]);
            temp->child.erase(temp->child.begin() + mid);
        }
    }

    int i;
    // adds the temp node and the newly created node as child nodes to parent in appropriate locations
    if (temp == root)
    {
        i = parent->keys.check(temp->keys.front());
        parent->child.insert(parent->child.begin() + i, temp);
    }
    i = parent->keys.check(newLeaf->keys.front());
    parent->child.insert(parent->child.begin() + i, newLeaf);
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

void BPlusTree::searchIndex(int index)
{
    TreeNode *temp = root;
    while (temp->isLeaf == false) // recursively traverse the tree till leaf node
    {
        int i = temp->keys.check(index);
        temp = temp->child[i];
    }

    cout << "Name: " << temp->keys.searchList(index);
}

TreeNode *BPlusTree::getRoot()
{
    return root;
}

int main()
{
    BPlusTree b;

    cout << "Welcome to ID-Name Management System";
    cout << "\n1.INSERT";
    cout << "\n2.SELECT";
    cout << "\n3.EXIT";
    char choice = '1';
    while (choice != 3)
    {
        cout << "\nEnter your choice:";
        cin >> choice;

        switch (choice)
        {
        case '1':
            cout << "Enter ID:";
            {
                int id;
                cin >> id;
                b.insertIndex(id, b.getRoot(), NULL);
            }
            break;
        case '2':
            cout << "Enter id:";
            {
                int id;
                cin >> id;
                b.searchIndex(id);
            }
            break;
        case '3':
            cout << "Exiting";
            break;
        default:
            cout << "Enter a valid choice:";
        }
    }
}
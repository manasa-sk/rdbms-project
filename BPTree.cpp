#include <iostream>
#include <vector>
#include <cmath>

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
    void deleteNode(int x);
    int searchList(int x);
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
    void deleteIndex(int index, TreeNode *temp, TreeNode *parent);
    TreeNode *searchIndex(int index);
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

int LinkedList::searchList(int x)
{
    Node *ptr = head;
    int i = 0;

    while (ptr != NULL)
    {
        if (x == ptr->index)
            return i;
        ptr = ptr->next;
        ++i;
    }

    return -1;
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

BPlusTree::BPlusTree() : root(NULL), height(0) {}

void BPlusTree::insertIndex(int index, TreeNode *temp, TreeNode *parent)
{
    if (root == NULL) // tree hasn't been created yet so creating the tree by initializing root node
    {
        root = new TreeNode;
        root->isLeaf = true;
        root->keys.insert(index);
        root->next = NULL;
    }
    else
    {
        if (temp->isLeaf == false)
        {
            int i = temp->keys.check(index);
            insertIndex(index, temp->child[i], temp); // recursively traverse the tree till leaf node
        }
        else
            temp->keys.insert(index);

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
    ++height;

    TreeNode *newLeaf = new TreeNode; // create new tree node and insert the firts element of it into parent
    newLeaf->keys.splitList(temp->keys);
    newLeaf->isLeaf = temp->isLeaf;
    parent->keys.insert(newLeaf->keys.front());

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
    if(temp == root)
    {
        i = parent->keys.check(temp->keys.front());
        parent->child.insert(parent->child.begin() + i, temp);
    }
    i = parent->keys.check(newLeaf->keys.front());
    parent->child.insert(parent->child.begin() + i, newLeaf);
    /*
    cout << "The vector elements are : ";
    for (int i = 0; i < parent->child.size(); i++)
    {
        parent->child[i]->keys.disList();
        cout << " ";
    }
    cout<<"\n";*/
}

void BPlusTree::deleteIndex(int index, TreeNode *temp, TreeNode *parent)
{
    int minKeys = ceil(order) - 1; // minimum number of keys that can be stored in the keyset of a tree node

    while (temp->isLeaf == false)
    {
        int i = temp->keys.check(index);
        temp = temp->child[i];
    }

    int i = temp->keys.searchList(index);
    temp->keys.deleteNode(i);

    if (temp->keys.size() < minKeys)
    {
        int j = parent->keys.check(index);                        // search for index of the child node in the vector to search for other child nodes to borrow from
        if (j > 0 && parent->child[j - 1]->keys.size() > minKeys) // check left sibling and borrow from it
        {
            TreeNode *leftSibling = parent->child[j - 1];
        }
        else if (j < order && parent->child[j + 1]->keys.size() > minKeys) // check right sibling and borrow from it
        {
            TreeNode *rightSibling = parent->child[j + 1];
            temp->keys.insert(rightSibling->keys.front());
            rightSibling->keys.deleteNode(0);

            parent->keys.deleteNode(j);
            parent->keys.insert(rightSibling->keys.front());
        }
    }
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

TreeNode *BPlusTree::getRoot()
{
    return root;
}

int main()
{
    BPlusTree b;
    
    cout << "inserted 1\n";
    b.insertIndex(1, b.getRoot(), NULL);
    b.dis(b.getRoot());
    cout << "inserted 3\n";
    b.insertIndex(3, b.getRoot(), NULL);
    b.dis(b.getRoot());
    cout << "inserted 5\n";
    b.insertIndex(5, b.getRoot(), NULL);
    b.dis(b.getRoot());
    cout << "inserted 7\n";
    b.insertIndex(7, b.getRoot(), NULL);
    b.dis(b.getRoot());
    cout << "inserted 9\n";
    b.insertIndex(9, b.getRoot(), NULL);
    b.dis(b.getRoot());
    cout << "inserted 2\n";
    b.insertIndex(2, b.getRoot(), NULL);
    b.dis(b.getRoot());
    cout << "inserted 4\n";
    b.insertIndex(4, b.getRoot(), NULL);
    b.dis(b.getRoot());
    cout << "inserted 6\n";
    b.insertIndex(6, b.getRoot(), NULL);
    b.dis(b.getRoot());
    cout << "inserted 8\n";
    b.insertIndex(8, b.getRoot(), NULL);
    b.dis(b.getRoot());
    cout << "inserted 10\n";
    b.insertIndex(10, b.getRoot(), NULL);
    b.dis(b.getRoot());
    /*
    cout << "inserted 28\n";
    b.insertIndex(28, b.getRoot(), NULL);
    b.dis(b.getRoot());
    cout << "inserted 42\n";
    b.insertIndex(42, b.getRoot(), NULL);
    b.dis(b.getRoot());*/
    
}

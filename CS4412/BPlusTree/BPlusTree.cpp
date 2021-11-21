#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <cmath>

using namespace std;

/*
Jason Bowden
CS 4414 - B+ Tree
*/

/* Person class stores the actual leaf data for our tree */
class Person
{
private:
    int id;
    string firstName;
    string lastName;
    string employer;
    string occupation;
    string city;
    string state;
public:
    Person(int id, string firstName, string lastName, string employer, string occupation, string city, string state)
    {
        this->id = id;
        this->firstName = firstName;
        this->lastName = lastName;
        this->employer = employer;
        this->occupation = occupation;
        this->city = city;
        this->state = state;
    }

    int GetId()
    {
        return this->id;
    }

    string GetLastName()
    {
        return this->lastName;
    }

    string GetFirstName()
    {
        return this->firstName;
    }

    string GetEmployer()
    {
        return this->employer;
    }

    string GetOccupation()
    {
        return this->occupation;
    }

    string GetCity()
    {
        return this->city;
    }

    string GetState()
    {
        return this->state;
    }
};

/* Leaf holds the actual data we're interested in */
struct Leaf
{
    Person* value;
    Leaf* next;
};

/* Custom implementation of a linked-list */
class LinkedList
{
private:
    Leaf** _list;
    int _currentListSize = 0;
    int _maxSize = 0;
public:
    LinkedList(int size)
    {
        _maxSize = size;
        _list = (Leaf**)calloc(1, sizeof(Leaf*));
    }

    int GetCurrentSize()
    {
        return _currentListSize;
    }

    int GetMaxSize()
    {
        return _maxSize;
    }

    /* insert leaf value in key order */
    void Insert(Person* person)
    {
        Leaf* item = new Leaf();
        item->value = person;
        item->next = NULL;

        Leaf* existingItem = _list[0];
        int position = 0;

        if (existingItem)
        {
            Leaf* prev = existingItem;

            while (existingItem)
            {
                if (person->GetId() < existingItem->value->GetId())
                {
                    item->next = existingItem;

                    if (position < 1)
                        _list[0] = item;
                    else if (prev->next)
                        prev->next = item;

                    return;
                }

                prev = existingItem;
                existingItem = existingItem->next;
                position++;
            }

            prev->next = item;
        }
        else
        {
            _list[0] = item;
        }
    }

    /* if key is found, remove the item */
    void Delete(int key) const
    {
        Leaf* existingItem = _list[0];

        if (existingItem)
        {
            Leaf* prev = existingItem;

            while (existingItem)
            {
                if (key == existingItem->value->GetId())
                {
                    Leaf* next = existingItem->next;

                    if (next)
                    {
                        // link previous item with next item
                        prev->next = next;
                    }

                    existingItem = NULL;
                    return;
                }

                prev = existingItem;
                existingItem = existingItem->next;
            }
        }
    }

    /* use key to find value in list */
    Leaf* Search(int key)
    {
        Leaf* existingItem = _list[0];

        if (existingItem)
        {
            while (existingItem)
            {
                if (key == existingItem->value->GetId())
                    return existingItem;

                existingItem = existingItem->next;
            }
        }

        return NULL;
    }

    Leaf* Split()
    {
        int half = _maxSize / 2;
        Leaf* item = _list[0];
        Leaf* prev = item;
        int i = 0;

        while (item)
        {
            if (i++ == half) // split here
            {
                prev->next = NULL; // "delete" half the nodes
                return item; // return half
            }

            Leaf* prev = item;
            item = item->next;
        }

        return NULL;
    }

    void PrintList()
    {
        Leaf* existingItem = _list[0];
        int i = 0;

        if (existingItem)
        {
            while (existingItem)
            {
                cout << "[" << i++ << ":" << existingItem->value->GetId() << "]->";

                existingItem = existingItem->next;
            }

            cout << "NULL";
        }
    }
};

/* 
   An NonLeafNodeKey belongs to a Non-leaf node and contains key info
   with left and right leaves                       
*/
class NonLeafNodeKey
{
private:
    LinkedList* _left;
    LinkedList* _right;
    int _leftKeyCountCurrentSize = 0;
    int _rightKeyCountCurrentSize = 0;
    int _maxLeafCount;
    int _index;
    NonLeafNodeKey* _next;
public:
    NonLeafNodeKey(int index, int leafCount)
    {
        _index = index;
        _maxLeafCount = leafCount;
        _left = new LinkedList(leafCount);
        _right = new LinkedList(leafCount);
    }

    void SetNext(NonLeafNodeKey* key)
    {
        _next = key;
    }

    NonLeafNodeKey* GetNext()
    {
        return _next;
    }

    /* return the index value for this key (we use this for left/right insertions) */
    int GetIndex()
    {
        return _index;
    }

    /* add a leaf to the left of this key */
    NonLeafNodeKey* InsertLeft(Person* p)
    {
        if (_leftKeyCountCurrentSize > 0 && _leftKeyCountCurrentSize == _left->GetMaxSize())
        {
            // return bottom half of leaf nodes
            // also, remove those leaf nodes from their current list and update the size accordingly
            Leaf* leaf = _left->Split();
            _leftKeyCountCurrentSize /= 2;

            NonLeafNodeKey* key = new NonLeafNodeKey(leaf->value->GetId(), _maxLeafCount);

            key->InsertRight(p);

            while (leaf)
            {
                key->InsertRight(leaf->value);

                leaf = leaf->next;
            }

            return key;
        }
        else
        {
            _leftKeyCountCurrentSize++;
            _left->Insert(p);
        }

        return NULL;
    }

    /* use the key to search the left leaves */
    Leaf* SearchLeft(int key)
    {
        return _left->Search(key);
    }
    
    /* use the key to remove a record from the left side */
    void DeleteLeft(int key)
    {
        _leftKeyCountCurrentSize--;

        _left->Delete(key);
    }

    /* use the key to search the right leaves */
    Leaf* SearchRight(int key)
    {
        return _right->Search(key);
    }

    /* add a leaf to the right leaves */
    NonLeafNodeKey* InsertRight(Person* p)
    {
        if (_rightKeyCountCurrentSize > 0 && _rightKeyCountCurrentSize == _right->GetMaxSize())
        {
            // return bottom half of leaf nodes
            // also, remove those leaf nodes from their current list and update the size accordingly
            Leaf* leaf = _right->Split();
            _rightKeyCountCurrentSize /= 2;

            NonLeafNodeKey* key = new NonLeafNodeKey(leaf->value->GetId(), _maxLeafCount);

            key->InsertRight(p);

            while (leaf)
            {
                key->InsertRight(leaf->value);
                leaf = leaf->next;
            }

            return key;
        }
        else
        {
            _rightKeyCountCurrentSize++;
            _right->Insert(p);
        }

        return NULL;
    }

    /* use the key to remove a leaf from the right leaves */
    void DeleteRight(int key)
    {
        _rightKeyCountCurrentSize--;

        _right->Delete(key);
    }

    /* determine how many leaves are on the left of this key */
    int GetLeftLeafSize()
    {
        return _leftKeyCountCurrentSize;
    }

    /* determine how many leaves are on the right of this key */
    int GetRightLeafSize()
    {
        return _rightKeyCountCurrentSize;
    }

    void PrintLeftLeaves()
    {
        _left->PrintList();
    }

    void PrintRightLeaves()
    {
        _right->PrintList();
    }
};

/* Non-leaf Node that consists of Non-leaf Node Key */
class NonLeafNode
{
    NonLeafNodeKey** _keys;
    int _maxKeyCount = 5;
    int _currentKeyCount = 0;

    void Insert(NonLeafNodeKey* key, Person* p)
    {
        NonLeafNodeKey* existingItem = _keys[0];
        int position = 0;

        NonLeafNodeKey* prev = existingItem;

        if (existingItem)
        {
            while (existingItem)
            {
                if (p->GetId() < existingItem->GetIndex())
                {
                    key->SetNext(existingItem);

                    if (position < 1)
                        _keys[0] = key;
                    else if (prev->GetNext())
                        prev->SetNext(key);

                    _currentKeyCount++;

                    return;
                }

                prev = existingItem;
                existingItem = existingItem->GetNext();
                position++;
            }

            prev->SetNext(key);
        }
        else
        {
            _keys[0] = key;
        }

        _currentKeyCount++;
    }
public:
    NonLeafNode(int maxKeyCount)
    {
        _maxKeyCount = maxKeyCount;
        _keys = (NonLeafNodeKey**)calloc(1, sizeof(NonLeafNodeKey*));
    }

    /* Use the key to determine which key node the leaf belongs to and on which side */
    NonLeafNodeKey* InsertKey(NonLeafNodeKey* key, Person* p)
    {
        double leavesPerNode = _maxKeyCount;
        double denominator = 2;
        leavesPerNode = ceil(leavesPerNode / denominator);

        if (_currentKeyCount < leavesPerNode)
        {
            Insert(key, p);
        }

        NonLeafNodeKey* current = _keys[0];
        while (current)
        {
            if (current && (key->GetIndex() < current->GetIndex()))
            {
                // go left
                NonLeafNodeKey* k = current->InsertLeft(p);

                if (k && _currentKeyCount < _maxKeyCount) // a split occurred, create new node with leaves... or propagate up to root
                    Insert(k, p);
                else if (_currentKeyCount == _maxKeyCount)
                {
                    return k;
                }

                break;
            }
            else if ((current && current->GetNext()) && key->GetIndex() > current->GetIndex() && key->GetIndex() < current->GetNext()->GetIndex())
            {
                // go right
                NonLeafNodeKey* k = current->InsertRight(p);

                if (k && _currentKeyCount < _maxKeyCount)
                    Insert(k, p);
                else if (_currentKeyCount == _maxKeyCount)
                {
                    return k;
                }

                break;
            }
            else if (current && current->GetIndex() == key->GetIndex())
            {
                // go right
                NonLeafNodeKey* k = current->InsertRight(p);

                if (k && _currentKeyCount < _maxKeyCount)
                    Insert(k, p);
                else if (_currentKeyCount == _maxKeyCount)
                {
                    return k;
                }

                break;
            }
            else if (current && key->GetIndex() > current->GetIndex() && !current->GetNext())
            {
                // go right
                NonLeafNodeKey* k = current->InsertRight(p);

                if (k && _currentKeyCount < _maxKeyCount)
                    Insert(k, p);
                else if (_currentKeyCount == _maxKeyCount)
                {
                    return k;
                }

                break;
            }

            current = current->GetNext();
        }

        return NULL;
    }

    /* use the key to direct where the leaf would be if it exists */
    Leaf* Find(int key)
    {
        NonLeafNodeKey* current = _keys[0];

        while (current)
        {
            if (current && (key < current->GetIndex()))
            {
                // go left
                return current->SearchLeft(key);
            }
            else if ((current && current->GetNext()) && key > current->GetIndex() && key < current->GetNext()->GetIndex())
            {
                // go right
                return current->SearchRight(key);
            }
            else if (current && current->GetIndex() == key)
            {
                // go right
                return current->SearchRight(key);
            }
            else if (current && key > current->GetIndex() && !current->GetNext())
            {
                // go right
                return current->SearchRight(key);
            }

            current = current->GetNext();
        }
    }

    /* use the key to determine where its leaf might be, if found; remove it */
    void Delete(int key)
    {
        NonLeafNodeKey* current = _keys[0];

        while (current)
        {
            if (current && (key < current->GetIndex()))
            {
                // go left
                return current->DeleteLeft(key);
            }
            else if ((current && current->GetNext()) && key > current->GetIndex() && key < current->GetNext()->GetIndex())
            {
                // go right
                return current->DeleteRight(key);
            }
            else if (current && current->GetIndex() == key)
            {
                // go right
                return current->DeleteRight(key);
            }
            else if (current && key > current->GetIndex() && !current->GetNext())
            {
                // go right
                return current->DeleteRight(key);
            }

            current = current->GetNext();
        }
    }

    void PrintNodeContents()
    {
        NonLeafNodeKey* existingItem = _keys[0];
        int i = 1;

        if (existingItem)
        {
            while (existingItem)
            {
                cout << "--------------------" << endl;
                cout << "NLN Key #" << i++ << " = " << existingItem->GetIndex() << "|" << endl;
                cout << "--------------------" << "|" << endl;
                cout << "Left Leaves " << "|" << endl;
                cout << "--------------------" << "|" << endl;
                existingItem->PrintLeftLeaves();
                cout << endl;
                cout << "--------------------" << "|" << endl;
                cout << "Right Leaves " << "|" << endl;
                cout << "--------------------" << "|" << endl;
                existingItem->PrintRightLeaves();
                cout << endl;
                cout << "--------------------" << "|" << endl;
                cout << endl;
                cout << endl;

                existingItem = existingItem->GetNext();
            }
        }
    }

    /* return the key index for the ith key in the node */
    int GetKeyAtLocation(int i)
    {
        return _keys[i]->GetIndex();
    }

    /* return the amount of keys currently stored in the node */
    int GetCurrentKeyCount()
    {
        return _currentKeyCount;
    }

    /* return the maximum allowance of keys for this node */
    int GetMaxKeyCount()
    {
        return _maxKeyCount;
    }
};

/* Root key class holds logic for tracking whether
   a leaf should go to a left or a right Non-leaf node */
class RootKey
{
private:
    int _index;
    int _maxKeyCount; // this is only for passing down to NLN
    NonLeafNode* _leftNode;
    NonLeafNode* _rightNode;
    RootKey* _next;

public:
    RootKey(int index, int maxKeyCount)
    {
        _index = index;
        _maxKeyCount = maxKeyCount;
    }

    void SetNext(RootKey* key)
    {
        _next = key;
    }

    RootKey* GetNext()
    {
        return _next;
    }

    /* get the index (or key) of this particular Root Key */
    int GetIndex()
    {
        return _index;
    }

    /* put the leaf in the left node */
    NonLeafNodeKey* InsertLeft(NonLeafNodeKey* key, Person* p)
    {
        if (!_leftNode)
            _leftNode = new NonLeafNode(_maxKeyCount);

        return _leftNode->InsertKey(key, p);
    }

    /* put the leaf in the right node */
    NonLeafNodeKey* InsertRight(NonLeafNodeKey* key, Person* p)
    {
        if (!_rightNode)
            _rightNode = new NonLeafNode(_maxKeyCount);

        return _rightNode->InsertKey(key, p);
    }

    /* use the key to remove the corresponding leaf from the left node */
    void DeleteLeft(int key)
    {
        _leftNode->Delete(key);
    }

    /* use the key to remove the corresponding leaf from the right node */
    void DeleteRight(int key)
    {
        _rightNode->Delete(key);
    }

    /* use the key to find the corresponding leaf in the left node */
    Leaf* SearchLeft(int key)
    {
        return _leftNode->Find(key);
    }

    /* use the key to find the corresponding leaf in the right node */
    Leaf* SearchRight(int key)
    {
        return _rightNode->Find(key);
    }

    /* determine how full the left non-leaf node is */
    int GetLeftNodeKeyCount()
    {
        if (!_leftNode)
            return 0;

        return _leftNode->GetCurrentKeyCount();
    }

    /* determine how full the right non-leaf node is */
    int GetRightNodeKeyCount()
    {
        if (!_rightNode)
            return 0;

        return _rightNode->GetCurrentKeyCount();
    }

    void PrintRightNodeContents()
    {
        if (!_rightNode)
            return;

        _rightNode->PrintNodeContents();
    }

    void PrintLeftNodeContents()
    {
        if (!_leftNode)
            return;

        _leftNode->PrintNodeContents();
    }
};

/* Root consists of m (_maxKeyCount) RootKeys and is at the top of the tree */
class RootNode
{
    RootKey** _keys;
    int _currentKeyCount = 0;
    int _maxKeyCount = 5;
    int _leafCount = 56;

    void InsertKey(RootKey* key, Person* p)
    {
        RootKey* existingItem = _keys[0];
        int position = 0;
        RootKey* prev = existingItem;

        if (existingItem)
        {
            while (existingItem)
            {
                if (p->GetId() < existingItem->GetIndex())
                {
                    key->SetNext(existingItem);

                    if (position < 1)
                        _keys[0] = key;
                    else if (prev->GetNext())
                        prev->SetNext(key);

                    _currentKeyCount++;

                    return;
                }

                prev = existingItem;
                existingItem = existingItem->GetNext();
                position++;
            }

            prev->SetNext(key);
        }
        else
        {
            _keys[0] = key;
        }

        _currentKeyCount++;
    }
public:
    RootNode(int maxKeyCount, int leafCount)
    {
        _leafCount = leafCount;
        _maxKeyCount = maxKeyCount;
        _keys = (RootKey**)calloc(1, sizeof(RootKey*));
    }

    /* Determine if a leaf should go to the left/right of a root key */
    void Insert(RootKey* key, Person* p)
    {
        if (_currentKeyCount < 2)
            InsertKey(key, p);

        RootKey* current = _keys[0];
        while (current)
        {
            if (current && (key->GetIndex() < current->GetIndex()))
            {
                NonLeafNodeKey* k = new NonLeafNodeKey(key->GetIndex(), _leafCount);

                // go left
                NonLeafNodeKey* r = current->InsertLeft(k, p);

                //if (r)
                //{
                // Capacity has been reached. Do something about it
                //}

                break;
            }
            else if ((current && current->GetNext()) && key->GetIndex() > current->GetIndex() && key->GetIndex() < current->GetNext()->GetIndex())
            {
                NonLeafNodeKey* k = new NonLeafNodeKey(key->GetIndex(), _leafCount);
                // go right
                current->InsertRight(k, p);

                break;
            }
            else if (current && current->GetIndex() == key->GetIndex())
            {
                NonLeafNodeKey* k = new NonLeafNodeKey(key->GetIndex(), _leafCount);
                // go right
                current->InsertRight(k, p);
                break;
            }
            else if (current && key->GetIndex() > current->GetIndex() && !current->GetNext())
            {
                NonLeafNodeKey* k = new NonLeafNodeKey(key->GetIndex(), _leafCount);
                // go right
                current->InsertRight(k, p);
                break;
            }

            current = current->GetNext();
        }
    }

    /* use key to follow the tree to the leaf node it belongs to */
    Leaf* Find(int key)
    {
        RootKey* current = _keys[0];

        while (current)
        {
            if (current && (key < current->GetIndex()))
            {
                return current->SearchLeft(key);
            }
            else if ((current && current->GetNext()) && key > current->GetIndex() && key < current->GetNext()->GetIndex())
            {
                return current->SearchRight(key);
            }
            else if (current && current->GetIndex() == key)
            {
                return current->SearchRight(key);
            }
            else if (current && key > current->GetIndex() && !current->GetNext())
            {
                return current->SearchRight(key);
            }

            current = current->GetNext();
        }
    }

    /* use the same logic as a find in order to delete the leaf if it is found */
    void Delete(int key)
    {
        RootKey* current = _keys[0];
        int position = 0;

        if (current)
        {
            RootKey* prev = current;

            while (current)
            {
                if (current && (key < current->GetIndex()))
                {
                    return current->DeleteLeft(key);
                }
                else if ((current && current->GetNext()) && key > current->GetIndex() && key < current->GetNext()->GetIndex())
                {
                    return current->DeleteRight(key);
                }
                else if (current && current->GetIndex() == key)
                {
                    return current->DeleteRight(key);
                }
                else if (current && key > current->GetIndex() && !current->GetNext())
                {
                    return current->DeleteRight(key);
                }

                prev = current;
                current = current->GetNext();
                position++;
            }
        }
    }

    /* return current root keys */
    RootKey** GetRootKeys()
    {
        return _keys;
    }

    void Print()
    {
        RootKey* current = _keys[0];
        int i = 1;

        while (current)
        {
            cout << "Root Key : " << i++ << " : " << current->GetIndex() << endl;
            cout << "Left NLN Contents " << endl;
            cout << "--------------------" << endl;
            current->PrintLeftNodeContents();
            cout << "Right NLN Contents " << endl;
            cout << "--------------------" << endl;
            current->PrintRightNodeContents();

            current = current->GetNext();
        }
    }

    /* return the size of the root node */
    int GetNodeSize()
    {
        return _currentKeyCount;
    }
};


/*
Jason Bowden
*/
class BPlusTree
{
private:
    RootNode* _root;
    int _maxKeyCount = 10;
    int _rowCount = 2000;
public:
    BPlusTree(int rowCount, int keysPerNode)
    {
        _maxKeyCount = keysPerNode;

        double leafCount = ((double)rowCount) / ((double)(_maxKeyCount + 1));
        leafCount = ceil(leafCount);
        double leaves = leafCount / ((double)(_maxKeyCount + 1));
        int l = ceil(leaves);

        _root = new RootNode(_maxKeyCount, l);
    }

    void Insert(Person* person)
    {
        RootKey* key = new RootKey(person->GetId(), _maxKeyCount);

        _root->Insert(key, person);
    }

    Person* Find(int key)
    {
        Leaf* item = _root->Find(key);

        if (item)
            return item->value;
        else
            return NULL;
    }

    void Delete(int key) const
    {
        _root->Delete(key);
    }

    /* never got this to work */
    //Person** Range(int low, int high)
    //{
    //    
    //}

    void Print()
    {
        _root->Print();
    }

    /* Didn't manage to get my tree to scale upward */
    int Min()
    {
        return 3;
    }

    /* Didn't manage to get my tree to scale upward */
    int Max()
    {
        return 3;
    }
};

BPlusTree* Tree;

/* Create matrix from file contents */
void populateBTreeWithFile()
{
    Tree = new BPlusTree(2000,5);
    string line = "";
    ifstream file("personRelation.csv");
    int n = 0;
    int lineCount = 0;

    if (file.is_open())
    {
        while (getline(file, line))
        {
            string firstName = "";
            string lastName = "";
            int id = 0;
            string employer = "";
            string occupation = "";
            string city = "";
            string state = "";
            size_t position = 0;
            int column = 0;

            if (lineCount > 0)
            {
                while ((position = line.find(",")) != std::string::npos)
                {
                    string s = line.substr(0, position);

                    if (column == 0) firstName = s;
                    else if (column == 1) lastName = s;
                    else if (column == 2) id = stoi(s);
                    else if (column == 3) employer = s;
                    else if (column == 4) occupation = s;
                    else if (column == 5) city = s;

                    line.erase(0, position + 1);
                    column++;
                }

                if (!line.empty())
                {
                    state = line;
                }

                Person* p = new Person(id, firstName, lastName, employer, occupation, city, state);
                Tree->Insert(p);
            }

            lineCount++;
        }

        file.close();
    }
    else cout << "Unable to open file";
}

/* Start here */
int main()
{
    populateBTreeWithFile();

    Tree->Delete(1026156);
    Tree->Delete(1071903);

    Tree->Print();

    int id;
    do
    {
        cout << "Enter id to begin search (0 to exit): " << endl;
        cin >> id;

        if (id == 0)
            break;

        Person* person = Tree->Find(id);

        if (person)
        {
            cout << "Id: " << person->GetId() << endl;
            cout << "First Name: " << person->GetFirstName() << endl;
            cout << "Last Name: " << person->GetLastName() << endl;
            cout << "Employer: " << person->GetEmployer() << endl;
            cout << "Occupation: " << person->GetOccupation() << endl;
            cout << "City: " << person->GetCity() << endl;
            cout << "State: " << person->GetState() << endl;
        }
        else
            cout << "Record not found." << endl;

        cout << endl; 

    } while (id > 0);
}

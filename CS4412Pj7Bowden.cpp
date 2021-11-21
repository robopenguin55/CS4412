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
public:
    LinkedList(int size)
    {
        _list = (Leaf**)calloc(size, sizeof(Leaf*));
    }

    /* insert leaf value in key order */
    void Insert(Person* person)
    {
        Leaf* item = new Leaf();
        item->value = person;
        item->next = NULL;

        Leaf* existingItem = _list[0];

        if (existingItem)
        {
            Leaf* prev = existingItem;

            while (existingItem)
            {
                prev = existingItem;
                existingItem = existingItem->next;
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

    void PrintList()
    {
        Leaf* existingItem = _list[0];

        if (existingItem)
        {
            while (existingItem)
            {
                cout << existingItem->value->GetId() << "->";

                existingItem = existingItem->next;
            }
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
    int _index;
public:
    NonLeafNodeKey(int index, int leafCount)
    {
        _index = index;
        _left = new LinkedList(leafCount);
        _right = new LinkedList(leafCount);
    }

    /* return the index value for this key (we use this for left/right insertions) */
    int GetIndex()
    {
        return _index;
    }

    /* add a leaf to the left of this key */
    void InsertLeft(Person* p)
    {
        _leftKeyCountCurrentSize++;

        _left->Insert(p);
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
    void InsertRight(Person* p)
    {
        _rightKeyCountCurrentSize++;

        _right->Insert(p);
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
public:
    NonLeafNode(int maxKeyCount)
    {
        _maxKeyCount = maxKeyCount;
        _keys = (NonLeafNodeKey**)calloc(_maxKeyCount, sizeof(NonLeafNodeKey*));
        for (int i = 0; i < _maxKeyCount; i++) _keys[i] = NULL;
    }

    /* Use the key to determine which key node the leaf belongs to and on which side */
    void InsertKey(NonLeafNodeKey* key, Person* p)
    {
        double leavesPerNode = _maxKeyCount;
        double denominator = 2;
        leavesPerNode = ceil(leavesPerNode/denominator);

        if (_currentKeyCount < leavesPerNode)
        {
            if (_currentKeyCount == 0)
                _keys[_currentKeyCount] = key;
            else if (key->GetIndex() > _keys[_currentKeyCount - 1]->GetIndex())
                _keys[_currentKeyCount] = key;
            else // current key is greater
            {
                NonLeafNodeKey* t = _keys[_currentKeyCount - 1];
                _keys[_currentKeyCount - 1] = key;
                _keys[_currentKeyCount] = t;
            }

            _currentKeyCount++;
        }

        int i = 0;

        while (_keys[i])
        {
            NonLeafNodeKey* current = _keys[i];

            if (current && (key->GetIndex() < current->GetIndex()))
            {
                // go left
                current->InsertLeft(p);
                break;
            }
            else if ((current && _keys[i + 1]) && key->GetIndex() > current->GetIndex() && key->GetIndex() < _keys[i + 1]->GetIndex())
            {
                // go right
                current->InsertRight(p);
                break;
            }
            else if (current && current->GetIndex() == key->GetIndex())
            {
                // go right
                current->InsertRight(p);
                break;
            }
            else if (current && key->GetIndex() > current->GetIndex() && !_keys[i + 1])
            {
                // go right
                current->InsertRight(p);
                break;
            }

            i++;
        }
    }

    /* use the key to direct where the leaf would be if it exists */
    Leaf* Find(int key)
    {
        int i = 0;

        while (_keys[i])
        {
            NonLeafNodeKey* current = _keys[i];

            if (current && (key < current->GetIndex()))
            {
                // go left
                return current->SearchLeft(key);
            }
            else if ((current && _keys[i + 1]) && key > current->GetIndex() && key < _keys[i + 1]->GetIndex())
            {
                // go right
                return current->SearchRight(key);
            }
            else if (current && current->GetIndex() == key)
            {
                // go right
                return current->SearchRight(key);
            }
            else if (current && key > current->GetIndex() && !_keys[i + 1])
            {
                // go right
                return current->SearchRight(key);
            }

            i++;
        }
    }

    /* use the key to determine where its leaf might be, if found; remove it */
    void Delete(int key)
    {
        int i = 0;

        while (_keys[i])
        {
            NonLeafNodeKey* current = _keys[i];

            if (current && (key < current->GetIndex()))
            {
                // go left
                return current->DeleteLeft(key);
            }
            else if ((current && _keys[i + 1]) && key > current->GetIndex() && key < _keys[i + 1]->GetIndex())
            {
                // go right
                return current->DeleteRight(key);
            }
            else if (current && current->GetIndex() == key)
            {
                // go right
                return current->DeleteRight(key);
            }
            else if (current && key > current->GetIndex() && !_keys[i + 1])
            {
                // go right
                return current->DeleteRight(key);
            }

            i++;
        }
    }

    void PrintNodeContents()
    {
        for (int i = 0; i < _currentKeyCount; i++)
        {
            cout << "--------------------" << endl;
            cout << "NLN Key #" << i + 1 << " = " << _keys[i]->GetIndex() << "|" << endl;
            cout << "--------------------" << "|" << endl;
            cout << "Left Leaves " << "|" << endl;
            cout << "--------------------" << "|" << endl;
            _keys[i]->PrintLeftLeaves();
            cout << endl;
            cout << "--------------------" << "|" << endl;
            cout << "Right Leaves " << "|" << endl;
            cout << "--------------------" << "|" << endl;
            _keys[i]->PrintRightLeaves();
            cout << endl;
            cout << "--------------------" << "|" << endl;
            cout << endl;
            cout << endl;
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
    int _maxKeyCount;
    int _currentKeyCount;
    NonLeafNode* _leftNode;
    NonLeafNode* _rightNode;

public:
    RootKey(int index, int maxKeyCount)
    {
        _index = index;
        _maxKeyCount = maxKeyCount;
    }

    /* get the index (or key) of this particular Root Key */
    int GetIndex()
    {
        return _index;
    }

    /* put the leaf in the left node */
    void InsertLeft(NonLeafNodeKey* key, Person* p)
    {
        if (!_leftNode)
            _leftNode = new NonLeafNode(_maxKeyCount);

        _leftNode->InsertKey(key, p);
    }

    /* put the leaf in the right node */
    void InsertRight(NonLeafNodeKey* key, Person* p)
    {
        if (!_rightNode)
            _rightNode = new NonLeafNode(_maxKeyCount);

        _rightNode->InsertKey(key, p);
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
public:
    RootNode(int maxKeyCount, int leafCount)
    {
        _leafCount = leafCount;
        _maxKeyCount = maxKeyCount;
        _keys = (RootKey**)calloc(_maxKeyCount, sizeof(RootKey*));
        for (int i = 0; i < _maxKeyCount; i++) _keys[i] = NULL;
    }

    /* Determine if a leaf should go to the left/right of a root key */
    void Insert(RootKey* key, Person* p)
    {
        if (_currentKeyCount < 2)
        {
            if (_currentKeyCount == 0)
                _keys[_currentKeyCount] = key;
            else if (key->GetIndex() > _keys[_currentKeyCount - 1]->GetIndex() && (_keys[_currentKeyCount] && key->GetIndex() < _keys[_currentKeyCount]->GetIndex())) // new key greater than previous and less than next?
            {
                _keys[_currentKeyCount] = key;
            }
            else
            {
                RootKey* t = _keys[_currentKeyCount - 1];
                _keys[_currentKeyCount - 1] = key;
                _keys[_currentKeyCount] = t;
            }

            _currentKeyCount++;
        }

        int i = 0;
        while (_keys[i])
        {
            RootKey* current = _keys[i];

            if (current && (key->GetIndex() < current->GetIndex()))
            {
                NonLeafNodeKey* k = new NonLeafNodeKey(key->GetIndex(), _leafCount);

                // go left
                current->InsertLeft(k, p);
                break;
            }
            else if ((current && _keys[i + 1]) && key->GetIndex() > current->GetIndex() && key->GetIndex() < _keys[i + 1]->GetIndex())
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
            else if (current && key->GetIndex() > current->GetIndex() && !_keys[i+1])
            {
                NonLeafNodeKey* k = new NonLeafNodeKey(key->GetIndex(), _leafCount);
                // go right
                current->InsertRight(k, p);
                break;
            }

            i++;
        }
    }

    /* use key to follow the tree to the leaf node it belongs to */
    Leaf* Find(int key)
    {
        int i = 0;
        while (_keys[i])
        {
            RootKey* current = _keys[i];

            if (current && (key < current->GetIndex()))
            {
                return current->SearchLeft(key);
            }
            else if ((current && _keys[i + 1]) && key > current->GetIndex() && key < _keys[i + 1]->GetIndex())
            {
                return current->SearchRight(key);
            }
            else if (current && current->GetIndex() == key)
            {
                return current->SearchRight(key);
            }
            else if (current && key > current->GetIndex() && !_keys[i + 1])
            {
                return current->SearchRight(key);
            }

            i++;
        }
    }

    /* use the same logic as a find in order to delete the leaf if it is found */
    void Delete(int key)
    {
        int i = 0;
        while (_keys[i])
        {
            RootKey* current = _keys[i];

            if (current && (key < current->GetIndex()))
            {
                return current->DeleteLeft(key);
            }
            else if ((current && _keys[i + 1]) && key > current->GetIndex() && key < _keys[i + 1]->GetIndex())
            {
                return current->DeleteRight(key);
            }
            else if (current && current->GetIndex() == key)
            {
                return current->DeleteRight(key);
            }
            else if (current && key > current->GetIndex() && !_keys[i + 1])
            {
                return current->DeleteRight(key);
            }

            i++;
        }
    }

    /* return current root keys */
    RootKey** GetRootKeys()
    {
        return _keys;
    }

    void Print()
    {
        for (int i = 0; i < _currentKeyCount; i++)
        {
            cout << "Root Key : " << i+1 << " : " << _keys[i]->GetIndex() << endl;
            cout << "Left NLN Contents " << endl;
            cout << "--------------------" << endl;
            _keys[i]->PrintLeftNodeContents();
            cout << "Right NLN Contents " << endl;
            cout << "--------------------" << endl;
            _keys[i]->PrintRightNodeContents();
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

    //Tree->Delete(1026156);

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

    } while (id > 0);
}

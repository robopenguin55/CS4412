#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <cmath>

using namespace std;

/*
Jason Bowden
*/
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

struct Node;

struct Key
{
    int index;
    Node* left;
    Node* right;
    Person* value;
};

struct Node
{
    int keyCount;
    Key** keys;
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

    void InsertAt(Person* person, int index)
    {
        Leaf* item = new Leaf();
        item->value = person;
        item->next = NULL;

        Leaf* existingItem = _list[index];

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
            _list[index] = item;
        }
    }

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

    Person** Range(int low, int high)
    {
        Leaf* existingItem = _list[0];
        Person** p = (Person**)calloc(high-low, sizeof(Person*));
        int i = 0;

        if (existingItem)
        {
            while (existingItem)
            {
                if (low <= existingItem->value->GetId() || existingItem->value->GetId() <= high)
                    p[i++] = existingItem->value;

                existingItem = existingItem->next;
            }
        }

        // resize
        Person** r = (Person**)calloc(i, sizeof(Person*));

        int j = 0;
        while (p[j])
        {
            r[j] = p[j];
            j++;
        }

        r[j] = NULL;

        return r;
    }
};
//
//class Key
//{
//private:
//    int _index;
//    int _m;
//    int _l;
//    Person* _value; // use only for leaf nodes
//    Node* _leftChildren;
//    Node* _rightChildren;
//public:
//    Key()
//    {
//        _value = NULL;
//    }
//
//    void SetIndex(int index)
//    {
//        _index = index;
//    }
//
//    int GetIndex()
//    {
//        return _index;
//    }
//
//    void SetValue(Person* p)
//    {
//        _value = p;
//    }
//
//    Person* GetValue()
//    {
//        return _value;
//    }
//};

//class Node
//{
//private:
//    int _keyDepth;
//    int _currentItemCount = 0;
//    //bool _isRoot;
//    //bool _isLeaf;
//    Key** _keys;
//    Node* _parent; // for tracking root
//    Node* _rightNeighbor; // for leaf nodes
//    Node* _leftNeighbor; // for leaf nodes
//    
//public:
//    Node(int m)
//    {
//        _keyDepth = m;
//        _parent = NULL;
//        _leftNeighbor = NULL;
//        _rightNeighbor = NULL;
//        //_isRoot = isRoot;
//        //_isLeaf = isLeaf;
//
//        _keys = (Key**)calloc(_keyDepth, sizeof(Key*));
//        for (int i = 0; i < _keyDepth; i++) _keys[i] = NULL;
//        _keys[0] = new Key();
//        _keys[0]->SetIndex(0);
//    }
//
//    void Insert(Person* p)
//    {
//        int i = 0;
//        Key* r = new Key();
//        r->SetIndex(p->GetId());
//
//  /*      if (_currentItemCount < 2)
//        {*/
//            // store values in first few keys
//            r->SetValue(p);
//
//            if (_currentItemCount == 0)
//                _keys[_currentItemCount] = r;
//            else if (r->GetIndex() > _keys[_currentItemCount - 1]->GetIndex())
//                _keys[_currentItemCount] = r;
//            else // current key is greater
//            {
//                Key* t = _keys[_currentItemCount - 1];
//                _keys[_currentItemCount - 1] = r;
//                _keys[_currentItemCount] = t;
//            }
//
//            _currentItemCount++;
//        //}
//
//        //delete r;
//    }
//
//    Person* Search(int key)
//    {
//        if (!_currentItemCount)
//            return NULL;
//        
//        int i = 0;
//        while (_keys[i])
//        {
//            if (_keys[i]->GetIndex() == key)
//                return _keys[i]->GetValue();
//            else if (key < _keys[i]->GetIndex())
//            {
//                // go left
//            }
//            else if (_keys[i+1] && (key > _keys[i]->GetIndex() && key < _keys[i + 1]->GetIndex()))
//            {
//                // go right
//            } 
//
//
//            i++;
//        }
//
//    }
//};

//class LeafNode : public Node
//{
//private:
//    LeafNode* _rightNeighbor;
//    LeafNode* _leftNeighbor;
//    int _l;
//public:
//    LeafNode(int l) : Node(l)
//    {
//        _l = l;
//    }
//};
//
//class NonLeafNode : public Node 
//{
//private:
//    NonLeafNode* _parent; // for tracking root
//    bool _isRoot;
//    int _m;
//public: 
//    NonLeafNode(int m, int isRoot) : Node(m)
//    {
//        _m = m;
//        _isRoot = isRoot;
//    }
//};

/*
Jason Bowden
*/
class BPlusTree
{
private:
    Node* _root;
    LinkedList* _leaves;
    int _m = 5;
    int _l = 56;

    void Insert(Person* person, Node* node)
    {
        if (!node)
        {
            node = new Node();
            node->keys = (Key**)calloc(_m, sizeof(Key*));
            for (int i = 0; i < _m; i++) node->keys[i] = NULL;
        }

        double d = ((double)_m) / ((double)2);
        d = ceil(d);
        int count = node->keyCount;

        if (count < d)
        {
            node->keys[count] = new Key();
            node->keys[count]->index = person->GetId();
            node->keys[count]->value = person;
            node->keys[count]->left = NULL;
            node->keys[count]->right = NULL;

            node->keyCount++;

            return;
        }

        int i = 0;
        while (node->keys[i])
        {
            if (person->GetId() < node->keys[i]->index)
            {
                // go left
                // use right side of key pointed to at ith index
                Insert(person, node->keys[i]->left);
                return;
            }
            else if (node->keys[i + 1] && (person->GetId() > node->keys[i]->index && person->GetId() < node->keys[i + 1]->index))
            {
                // go right
                Insert(person, node->keys[i]->right);
                return;
            }
            else if (!(node->keys[i + 1]) && person->GetId() > node->keys[i]->index)
            {
                // go right
                Insert(person, node->keys[i]->right);
                return;
            }
            else
            {
                node->keys[count] = new Key();
                node->keys[count]->index = person->GetId();
                node->keys[count]->value = person;
                node->keys[count]->left = NULL;
                node->keys[count]->right = NULL;

                node->keyCount++;
                return;
            }

            i++;
        }

        // implement index here
        //_root->keys[0]->value = person;

        

    }

    Person* Find(int key, Node* node)
    {
        // I am wonder if we should use root if node is null
        // otherwise use node??
        if (!node)
            node = new Node();

        // implement index here 
        if (!node->keyCount && !node)
            return NULL;

        int i = 0;
        while (node->keys[i])
        {
            if (node->keys[i]->index == key)
            {
                // this means get value for key at ith index
                return node->keys[i]->value;
            }
            else if (key < node->keys[i]->index)
            {
                // go left
                // use right side of key pointed to at ith index
                return Find(key, node->keys[i]->left);
            }
            else if (node->keys[i + 1] && (key > node->keys[i]->index && key < node->keys[i + 1]->index))
            {
                // go right
                return Find(key, node->keys[i]->right);
            }

            i++;
        }

        Leaf* item = _leaves->Search(key);

        if (item)
            return item->value;
        else
            return NULL;
    }
public:
    BPlusTree(int size, int keysPerNode)
    {
        _m = keysPerNode;

        double leavesPerNode = ((double)size) / ((double)(_m + 1));
        leavesPerNode = ceil(leavesPerNode);
        double leavesPerBin = leavesPerNode / ((double)(_m + 1));
        _l = ceil(leavesPerBin);

        _leaves = new LinkedList(size);
        _root = new Node();
        _root->keys = (Key**)calloc(keysPerNode, sizeof(Key*));
        for (int i = 0; i < keysPerNode; i++) _root->keys[i] = NULL;
    }

    void Insert(Person* person)
    {
        Insert(person, _root);

        // add record to the linked-list
        _leaves->Insert(person);
    }

    Person* Find(int key)
    {
        return Find(key, _root);
    }

    void Delete(int key) const
    {
        // implement index here 


        _leaves->Delete(key);
    }

    Person** Range(int low, int high)
    {
        return this->_leaves->Range(low, high);
    }

    void Print()
    {
        //auto keys = _root->GetKeys();
        //auto size = _root->GetNodeSize();

        //cout << "Root : | ";

        //for (int i = 0; i < size; i++)
        //{
        //    cout << keys[i]->GetKey() << " | ";
        //}

        //cout << endl;
    }

    int Min()
    {
        return 0;
    }

    int Max()
    {
        return 0;
    }
};


BPlusTree* Tree;

/* Create matrix from file contents */
void populateBTreeWithFile()
{
    Tree = new BPlusTree(2000, 5);
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

int main()
{
    //populateBTreeWithFile();
    Tree = new BPlusTree(5, 5);
    Person* p1 = new Person(10, "Jason", "Bowden", "Iteris", "Software boi", "Pocatello", "ID");
    Person* p2 = new Person(5, "Julie", "Garcia-Bowden", "VA", "Pharm gurl", "Pocatello", "ID");
    Person* p3 = new Person(30, "Jimmy", "Brown", "Iteris", "Software boi", "Pocatello", "ID");
    Person* p4 = new Person(40, "Timmy", "Giggles", "Iteris", "Software boi", "Pocatello", "ID");
    Person* p5 = new Person(50, "Mr D", "Big", "Iteris", "Software boi", "Pocatello", "ID");

    Tree->Insert(p1);
    Tree->Insert(p2);
    Tree->Insert(p3);
    Tree->Insert(p4);
    Tree->Insert(p5);

    //Person** range = Tree->Range(5, 50);

    //int j = 0;
    //while (range[j])
    //{
    //    Person* person = range[j];

    //    cout << "Id: " << person->GetId() << endl;
    //    cout << "First Name: " << person->GetFirstName() << endl;
    //    cout << "Last Name: " << person->GetLastName() << endl;
    //    cout << "Employer: " << person->GetEmployer() << endl;
    //    cout << "Occupation: " << person->GetOccupation() << endl;
    //    cout << "City: " << person->GetCity() << endl;
    //    cout << "State: " << person->GetState() << endl;
    //    j++;
    //}

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

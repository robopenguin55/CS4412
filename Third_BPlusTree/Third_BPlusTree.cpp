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

struct Key
{
    int index;
    Person* value;
};

struct Node
{
    int keyCount;
    Key** keys;
    Node* next;
};

struct RootNode
{
    int keyCount;
    int index;
    Node** nodes;
};

/*
Jason Bowden
*/
class BPlusTree
{
private:
    Node* _root;
    //LinkedList* _leaves;
    int _m = 5;
    int _l = 56;

    void Insert(Person* person, Node* node)
    {
        
    }

    Person* Find(int key, Node* node)
    {

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

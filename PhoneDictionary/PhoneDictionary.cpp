#include <iostream>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

using namespace std;

/*
Jason Bowden
Contains the various combinations of a persons name
*/
class Name
{
private:
    string nameInStandardForm;
    string firstName;
    string middleName;
    string lastName;
public:
    Name(string first, string middle, string last)
    {
        firstName = first;
        middleName = middle;
        lastName = last;
        nameInStandardForm = last + middle[0] + first;
    }

    string GetName()
    {
        return nameInStandardForm;
    }

    string GetFullName()
    {
        return firstName + " " + middleName + " " + lastName;
    }

    string GetFirstName()
    {
        return firstName;
    }
};

/*
Jason Bowden
Contains the street number for an address
*/
class StreetNumber
{
private:
    int number;
public:
    StreetNumber(int n)
    {
        number = n;
    }
};

/*
Jason Bowden
Contains the street name for an address
*/
class StreetName
{
private:
    string street;
public:
    StreetName(string streetName)
    {
        street = streetName;
    }
};

/*
Jason Bowden
Contains the zip code of an address
*/
class Zip
{
private:
    string zip;
public:
    Zip(string zipCode)
    {
        zip = zipCode;
    }
};

/*
Jason Bowden
Contains the city for an address
*/
class City
{
private:
    string city;
public:
    City(string cityName)
    {
        city = cityName;
    }
};

/*
Jason Bowden
Contains the area code for a phone number
*/
class AreaCode
{
private:
    string code;
public:
    AreaCode(string areaCode)
    {
        code = areaCode;
    }

    string GetAreaCode()
    {
        return code;
    }
};

/*
Jason Bowden
Contains the state for an address
*/
class State
{
private:
    string state;
public:
    State(string stateName)
    {
        state = stateName;
    }
};

/*
Jason Bowden
Contains the phone number with an area code
*/
class PhoneNumber
{
private:
    AreaCode* code;
    string number;
public:
    PhoneNumber(AreaCode * areaCode, string phoneNumber)
    {
        number = phoneNumber;
        code = areaCode;
    }

    string GetPhoneNumber()
    {
        return code->GetAreaCode() + number;
    }
};

/*
Jason Bowden
Contains the properties that make up an address
*/
class Address
{
private:
    StreetNumber* streetNum;
    StreetName * streetName;
    City * city;
    State * state;
    Zip * zip;

public:
    Address(StreetNumber* sNum,
        StreetName* sName,
        City* c,
        State* st,
        Zip* z)
    {
        streetNum = sNum;
        streetName = sName;
        city = c;
        state = st;
        zip = z;
    }
};

/*
Jason Bowden
Contains the properties that make up a phone contact
*/
class PhoneEntry
{
private:
    Name * name;
    Address * address;
    PhoneNumber* phoneNum;

public:
    PhoneEntry(Name * n, Address * a, PhoneNumber* p)
    {
        name = n;
        address = a;
        phoneNum = p;
    }

    string GetName()
    {
        return name->GetName();
    }

    string GetFullName()
    {
        return name->GetFullName();
    }

    string GetPhoneNumber()
    {
        return phoneNum->GetPhoneNumber();
    }
};

/*
Jason Bowden
Data structure to be used by a hash table. 
Contains a key for searchability, a value to be stored per key, 
and a next pointer to establish it as a linked list if necessary
*/
struct HashItem
{
    string key;
    PhoneEntry* value;
    HashItem* next;
};

/*
Jason Bowden
Implementation of a Hashtable that stores PhoneEntries as its values.
Bucket size is 8 so performance won't be great. But its a start!
*/
class HashTable
{
private:
    HashItem** items;
    const unsigned int size = 8;
    unsigned int count = 0;

    unsigned int HashFunction(string str) 
    {
        unsigned int i = 0;
        unsigned int stringLength = str.length();

        for (unsigned j = 0; j < stringLength; j++) i = str[j] + i;

        return i % 8;
    }
public:
    HashTable()
    {
        items = (HashItem**)calloc(size,sizeof(HashItem*));
        for (int i = 0; i < size; i++) items[i] = NULL;
    }

    void Insert(string key, PhoneEntry* entry)
    {
        unsigned int index = HashFunction(key);

        HashItem* item = new HashItem();

        item->key = key;
        item->value = entry;
        item->next = NULL;

        HashItem* existingItem = items[index];

        if (existingItem)
        {
            HashItem* prev = existingItem;

            while (existingItem)
            {
                // dont accept duplicate keys
                if (key == existingItem->key)
                    return;

                prev = existingItem;
                existingItem = existingItem->next;
            }

            prev->next = item;
        }
        else
        {
            items[index] = item;
            count++;
        }
    }

    bool ContainsKey(string key)
    {
        for (int index = 0; index < size; index++)
        {
            HashItem* existingItem = items[index];

            if (existingItem)
            {
                while (existingItem)
                {
                    // bingo!
                    if (key == existingItem->key)
                        return true;

                    existingItem = existingItem->next;
                }
            }
        }

        return false;
    }

    void DisplayString()
    {
        for (int index = 0; index < size; index++)
        {
            HashItem* existingItem = items[index];

            if (existingItem)
            {
                while (existingItem)
                {
                    cout << "Bucket : " << index << endl;
                    cout << "Key : " << existingItem->key << endl;

                    existingItem = existingItem->next;
                }
            }
        }
    }

    PhoneEntry* Fetch(string key)
    {
        unsigned int index = HashFunction(key);

        HashItem* existingItem = items[index];

        if (existingItem)
        {
            while (existingItem)
            {
                if (key == existingItem->key)
                    return existingItem->value;

                existingItem = existingItem->next;
            }
        }
    }

    void Delete(string key)
    {
        unsigned int index = HashFunction(key);

        HashItem* existingItem = items[index];

        if (existingItem)
        {
            HashItem* prev = existingItem;

            while (existingItem)
            {
                // found it, now kill it
                if (key == existingItem->key)
                {
                    HashItem* next = existingItem->next;

                    if (next)
                    {
                        // link previous item with next item
                        prev->next = next;
                    }

                    existingItem = NULL;
                    return; // we're done here
                }

                prev = existingItem;
                existingItem = existingItem->next;
            }
        }
    }
};

/*
Jason Bowden
HashTable wrapper class that attempts to partially imitate the C# Dictionary
by allowing similar function calls.
*/
class Dictionary
{
private:
    HashTable* table;
public:
    Dictionary()
    {
        table = new HashTable();
    }

    void Add(string key, PhoneEntry* value)
    {
        table->Insert(key, value);
    }

    void Remove(string key)
    {
        table->Delete(key);
    }

    void DisplayString()
    {
        table->DisplayString();
    }

    PhoneEntry* Get(string key)
    {
        return table->Fetch(key);
    }

    void Clear()
    {
        table = new HashTable();
    }

    bool ContainsKey(string key)
    {
        return table->ContainsKey(key);
    }
};

/*
Jason Bowden
Random alpha string generator for building strings that are a given length
*/
string generateRandomName(int length)
{
    const char alpha[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    int stringLength = sizeof(alpha) - 1;
    string name = "";

    for (int i = 0; i < length; i++)
    {
        name += alpha[rand() % stringLength];
    }

    return name;
}

/*
Jason Bowden
Random numeric string generator for building strings that are a given length
*/
string generateRandomNumericString(int length)
{
    const char num[] =
        "0123456789";

    int stringLength = sizeof(num) - 1;
    string name = "";

    for (int i = 0; i < length; i++)
    {
        name += num[rand() % stringLength];
    }

    return name;
}

/*
Jason Bowden
Random number generator that uses system time as a seed. Unfortunately, the thread sleep
is necessary for generating different numbers between calls. Increase the time for better
random numbers... at the cost of the application taking longer to run each call of course.
*/
int generateRandomNumber()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    srand(time(NULL)); // set seed for random number generator
    return (int)rand() % 10;
}

int main()
{
    Dictionary* dict = new Dictionary();

    // trying to randomly populate our dictionary
    for (int i = 0; i < 10; i++)
    {
        Name* name = new Name(generateRandomName(generateRandomNumber()), generateRandomName(generateRandomNumber()), generateRandomName(generateRandomNumber()));
        AreaCode* areaCode = new AreaCode(generateRandomNumericString(3));
        PhoneNumber* phone = new PhoneNumber(areaCode, generateRandomNumericString(7));
        StreetNumber* streetNum = new StreetNumber(generateRandomNumber());
        StreetName* streetName = new StreetName(generateRandomName(generateRandomNumber()));
        City* city = new City(generateRandomName(generateRandomNumber()));
        State* state = new State(generateRandomName(generateRandomNumber()));
        Zip* zip = new Zip(generateRandomNumericString(7));
        Address* jjAddress = new Address(streetNum, streetName, city, state, zip);
        PhoneEntry* phoneEntry = new PhoneEntry(name, jjAddress, phone);

        dict->Add(name->GetName(), phoneEntry);

        cout << "Adding " << name->GetFullName() << endl;
    }

    dict->DisplayString();

    string last, middle, first;
    do
    {
        cout << "Enter last name to begin search (or q to quit): " << endl;
        cin >> last;

        if (last != "q")
        {
            cout << "Enter middle name: " << endl;
            cin >> middle;
            cout << "Enter first name: " << endl;
            cin >> first;

            Name* name = new Name(first, middle, last);

            bool isFound = dict->ContainsKey(name->GetName());

            if (isFound)
            {
                cout << "Record was found." << endl;
                PhoneEntry* entry = dict->Get(name->GetName());
                cout << "Full name   : " << entry->GetFullName() << endl;
                cout << "Phone number: " << entry->GetPhoneNumber() << endl;
            }
        }

    } while (last != "q");
}
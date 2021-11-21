
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

using namespace std;
using namespace std::chrono;

void fooSort(unsigned int* a, unsigned int size);
void minMaxFooSort(unsigned int* a, unsigned int size);
void populateArrayWithPsuedoRandomNumbers(unsigned int* arr, int arraySize);
void printArray(unsigned int* a, int size);
unsigned int* minMax(unsigned int* a, unsigned int size);
void vectorFooSort(vector<unsigned int> a);

int main()
{
	unsigned int const arrayToSortSize = pow(10, 3);//pow(2,32)-1;

	unsigned int* a = new unsigned int[arrayToSortSize];
	unsigned int* b = new unsigned int[arrayToSortSize];
	unsigned int* c = new unsigned int[arrayToSortSize];

	populateArrayWithPsuedoRandomNumbers(a, arrayToSortSize);
	populateArrayWithPsuedoRandomNumbers(b, arrayToSortSize);
	populateArrayWithPsuedoRandomNumbers(c, arrayToSortSize);

	//cout << "Original unsorted a array" << endl;
	//printArray(a, arrayToSortSize);

	auto start = high_resolution_clock::now();
	fooSort(a, arrayToSortSize);
	auto end = high_resolution_clock::now();

	auto duration = duration_cast<milliseconds>(start - end);

	cout << "random sort took " << abs(duration.count()) << " milliseconds for " << arrayToSortSize << " records." << endl;

	start = high_resolution_clock::now();
	fooSort(a, arrayToSortSize);
	end = high_resolution_clock::now();

	duration = duration_cast<milliseconds>(start - end);

	cout << "pre-sorted sort took " << abs(duration.count()) << " milliseconds for " << arrayToSortSize << " records." << endl;

	//cout << "Sorted array" << endl;
	//printArray(a, arrayToSortSize);

	//cout << "Original unsorted b array" << endl;
	//printArray(b, arrayToSortSize);

	start = high_resolution_clock::now();
	minMaxFooSort(b, arrayToSortSize);
	end = high_resolution_clock::now();

	duration = duration_cast<milliseconds>(start - end);

	cout << "minMax sort took " << abs(duration.count()) << " milliseconds for " << arrayToSortSize << " records." << endl;

	//cout << "sorted b array" << endl;
	//printArray(b, arrayToSortSize);

	vector<unsigned int> aa;

	// populate vector
	for (unsigned int i = 0; i < arrayToSortSize; i++) aa.insert(aa.begin() + i, c[i]);

	start = high_resolution_clock::now();
	vectorFooSort(aa);
	end = high_resolution_clock::now();

	duration = duration_cast<milliseconds>(start - end);

	cout << "vector sort took " << abs(duration.count()) << " milliseconds for " << arrayToSortSize << " records." << endl;

	delete[] a;

	int t;
	cin >> t;
}

void fooSort(unsigned int* a, unsigned int size)
{
	unsigned int mapSize = pow(2,32)-1;
	unsigned int* freqCount = new unsigned int[mapSize];

	// initialize array to 0
	for (unsigned int i = 0; i < mapSize; i++) freqCount[i] = 0;

	// traverse the array 
	for (unsigned int i = 0; i < size; i++) freqCount[a[i]] += 1;

	// traverse our freqCount array
	unsigned int t = 0;
	for (unsigned int i = 0; i < mapSize; i++)
	{
		if (freqCount[i] > 0)
			for (unsigned int j = 0; j < freqCount[i]; j++) a[t++] = i;
	}

	// deallocate freqCount
	delete[] freqCount;
}

void vectorFooSort(vector<unsigned int> a)
{
	unsigned int aSize = a.size();
	unsigned int mapSize= pow(2, 32)-1;

	vector<unsigned int> freqCount;

	freqCount.resize(mapSize);

	// traverse the array 
	for (int i = 0; i < aSize; i++) freqCount[a[i]] += 1;

	// traverse our freqCount array
	int t = 0;
	for (int i = 0; i < mapSize; i++)
	{
		if (t == aSize)
			break; // lets get out of here

		if (freqCount[i] > 0)
			for (int j = 0; j < freqCount[i]; j++) a[t++] = i;
	}
}

void minMaxFooSort(unsigned int* a, unsigned int size)
{
	auto mm = minMax(a, size);

	unsigned int mapSize = mm[1];
	unsigned int* freqCount = new unsigned int[mapSize+1];

	// initialize array to 0
	for (unsigned int i = 0; i < mapSize; i++) freqCount[i] = 0;

	// traverse the array 
	for (unsigned int i = 0; i < size; i++) freqCount[a[i]] += 1;

	// traverse our freqCount array
	unsigned int t = 0;
	for (unsigned int i = 0; i < mapSize; i++)
	{
		if (freqCount[i] > 0)
			for (unsigned int j = 0; j < freqCount[i]; j++) a[t++] = i;
	}

	// deallocate freqCount
	delete[] freqCount;
}

unsigned int* minMax(unsigned int* a, unsigned int size)
{
	unsigned int min = UINT_MAX; 
	unsigned int max = 0;

	for (unsigned int i = 0; i < size; i++)
	{
		if (a[i] < min)
			min = a[i];

		if (a[i] > max)
			max = a[i];
	}

	unsigned int* minMax = new unsigned int[2];

	minMax[0] = min;
	minMax[1] = max;

	return minMax;
}

void printArray(unsigned int* a, int size)
{
	for (unsigned int i = 0; i < size; i++) cout << a[i] << " ";

	cout << endl;
}

void populateArrayWithPsuedoRandomNumbers(unsigned int* arr, int arraySize)
{
	std::this_thread::sleep_for(std::chrono::seconds(1));
	srand(time(NULL)); // set seed for random number generator
	for (unsigned int i = 0; i < arraySize; i++) arr[i] = rand() % 1000;
}

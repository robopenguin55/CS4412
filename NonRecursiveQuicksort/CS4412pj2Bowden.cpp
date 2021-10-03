/*
    Jason Bowden
    CS4412
    Project 2 - Alternate Quicksort partitions, WorseCase BigO, Indexed Sorting
*/

#include <iostream>
#include <math.h>
#include <ctime>
#include <chrono>

using namespace std;
using namespace std::chrono;

void nonRecursiveQuicksortWithFirstElementAsPivot(int* arr, int arraySize);
void nonRecursiveQuicksortWithMiddleElementAsPivot(int* arr, int arraySize);
void nonRecursiveQuicksortWithRandomElementAsPivot(int* arr, int arraySize);
int* nonRecursiveIndexedQuicksortWithFirstElementAsPivot(int* arr, int arraySize);
void showArrayContents(int* arr, int arraySize, int pivot, int comparison);
void showArrayContents(int* arr, int arraySize);
void populateArrayWithPsuedoRandomNumbers(int* arr, int arraySize);
void nonRecursiveQuicksort(int* arr, int arraySize);
int findMiddleValue(int arr[3]);
void bubbleSort(int* a, int size);

int main()
{
    const unsigned int arraySize = (int)pow(10,8);
    int* arr1 = new int[arraySize];
    int* arr2 = new int[arraySize];
    //int* arr3 = new int[arraySize];
    //int* arr4 = new int[arraySize];

    populateArrayWithPsuedoRandomNumbers(arr1, arraySize);

    // copy arr1 to arr2
    for (int i = 0; i < arraySize; i++) arr2[i] = arr1[i];
    // copy arr1 to arr3
    //for (int i = 0; i < arraySize; i++) arr3[i] = arr1[i];
    //// copy arr1 to arr4
    //for (int i = 0; i < arraySize; i++) arr4[i] = arr1[i];

    // sort first randomized array (using first element as pivot)
    auto start = high_resolution_clock::now();
    nonRecursiveQuicksortWithFirstElementAsPivot(arr1, arraySize);
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(start - end);

    cout << "first element sort took " << abs(duration.count()) << " milliseconds." << endl;

    // sort second randomized array (using middle element as the pivot)
    start = high_resolution_clock::now();
    nonRecursiveQuicksortWithMiddleElementAsPivot(arr2, arraySize);
    end = high_resolution_clock::now();

    duration = duration_cast<milliseconds>(start - end);

    cout << "middle element sort took " << abs(duration.count()) << " milliseconds." << endl;

    // sort third randomized array (using random element as the pivot)
    //start = high_resolution_clock::now();
    //nonRecursiveQuicksortWithRandomElementAsPivot(arr3, arraySize);
    //end = high_resolution_clock::now();

    //duration = duration_cast<milliseconds>(start - end);

    //cout << "random element sort took " << abs(duration.count()) << " milliseconds." << endl;

    //// sort array that is already sorted
    start = high_resolution_clock::now();
    nonRecursiveQuicksortWithFirstElementAsPivot(arr1, arraySize);
    end = high_resolution_clock::now();

    duration = duration_cast<milliseconds>(start - end);

    cout << "first element sorted sort took " << abs(duration.count()) << " milliseconds." << endl;

    // sort second array that has already been sorted (using middle element as the pivot)
    start = high_resolution_clock::now();
    nonRecursiveQuicksortWithMiddleElementAsPivot(arr2, arraySize);
    end = high_resolution_clock::now();

    duration = duration_cast<milliseconds>(start - end);

    cout << "middle element sorted sort took " << abs(duration.count()) << " milliseconds." << endl;

    // sort third array that is already sorted
    //start = high_resolution_clock::now();
    //nonRecursiveQuicksortWithFirstElementAsPivot(arr3, arraySize);
    //end = high_resolution_clock::now();

    //duration = duration_cast<milliseconds>(start - end);

    //cout << "random element sorted sort took " << abs(duration.count()) << " milliseconds." << endl;

    //showArrayContents(arr1, arraySize);

    /* indexed sorting */
    //int* indexArray = nonRecursiveIndexedQuicksortWithFirstElementAsPivot(arr4, arraySize);
    //int* sortedArray = new int[arraySize];

    //// populate sorted array after indexed sort
    //for (int i = 0; i < arraySize; i++)
    //{
    //    sortedArray[i] = arr4[indexArray[i]];
    //}

    //showArrayContents(sortedArray, arraySize);

    // wait for input to close
    int x;
    cin >> x;
}

void nonRecursiveQuicksort(int* arr, int arraySize)
{
    int lastsmall, foo, left = 0, right = arraySize - 1;;
    int* leftRightStack = (int*)malloc(sizeof(int) * arraySize * 4);
    int stackTop = -1; // index to top element in stack

    leftRightStack[++stackTop] = left; // push left then right.
    leftRightStack[++stackTop] = right;

    while (stackTop >= 0)
    {
        right = leftRightStack[stackTop--]; // pop right then left
        left = leftRightStack[stackTop--];
        if (left < right)
        {
            lastsmall = left;

            for (int i = left + 1; i <= right; i++)
            {
                //showArrayContents(arr, arraySize, left, i);
                if (arr[i] < arr[left])
                {
                    lastsmall++;

                    foo = arr[i];

                    arr[i] = arr[lastsmall];
                    arr[lastsmall] = foo;
                    //showArrayContents(arr, arraySize, left, lastsmall);
                }
            }
            foo = arr[left];
            arr[left] = arr[lastsmall];
            arr[lastsmall] = foo;

            //showArrayContents(arr, arraySize, left, lastsmall);

            // qSort(list, left, lastsmall - 1);
            leftRightStack[++stackTop] = left;
            leftRightStack[++stackTop] = lastsmall - 1;

            // qSort(list, lastsmall + 1, right);
            leftRightStack[++stackTop] = lastsmall + 1;
            leftRightStack[++stackTop] = right;
        }
    }

    free(leftRightStack);
}

void nonRecursiveQuicksortWithFirstElementAsPivot(int* arr, int arraySize)
{
    nonRecursiveQuicksort(arr, arraySize);
}

void nonRecursiveQuicksortWithMiddleElementAsPivot(int* arr, int arraySize)
{
    int lastsmall, foo, left = 0, right = arraySize - 1, middle = (arraySize-1)/2;

    int a[3];

    a[0] = arr[left];
    a[1] = arr[middle];
    a[2] = arr[right];

    int middleValue = findMiddleValue(a);

    int pivot = 0;

    if (middleValue == arr[left])
    {
        pivot = left;
    }
    else if (middleValue == arr[middle])
    {
        pivot = middle;
    }
    else if (middleValue == arr[right])
    {
        pivot = right;
    }

    // now I just need to swap the first value with the pivot
    arr[pivot] = arr[0];
    arr[0] = middleValue;

    nonRecursiveQuicksort(arr, arraySize);
}

void nonRecursiveQuicksortWithRandomElementAsPivot(int* arr, int arraySize)
{
    srand(time(NULL)); // set seed for random number generator
    int random = rand();
    int randomIndex = random % arraySize;

    int value = arr[randomIndex];

    // swap first element with random element
    arr[randomIndex] = arr[0];
    arr[0] = value;

    nonRecursiveQuicksort(arr, arraySize);
}

int* nonRecursiveIndexedQuicksortWithFirstElementAsPivot(int* arr, int arraySize)
{
    int lastsmall, foo, left = 0, right = arraySize - 1;;
    unsigned int* leftRightStack = (unsigned int*)malloc(sizeof(int) * arraySize * 4);
    int* index = new int[arraySize];
    int stackTop = -1; // index to top element in stack

    leftRightStack[++stackTop] = left; // push left then right.
    leftRightStack[++stackTop] = right;

    // set up index
    for (int i = 0; i < arraySize; i++) index[i] = i;

    while (stackTop >= 0)
    {
        right = leftRightStack[stackTop--]; // pop right then left
        left = leftRightStack[stackTop--];
        if (left < right)
        {
            // left is our pivot point
            lastsmall = left;

            for (int i = left + 1; i <= right; i++)
            {
                if (arr[index[i]] < arr[index[left]])
                {
                    lastsmall++;

                    foo = index[i];

                    index[i] = index[lastsmall];
                    index[lastsmall] = foo;
                }
            }
            foo = index[left];
            index[left] = index[lastsmall];
            index[lastsmall] = foo;

            // qSort(list, left, lastsmall - 1);
            leftRightStack[++stackTop] = left;
            leftRightStack[++stackTop] = lastsmall - 1;

            // qSort(list, lastsmall + 1, right);
            leftRightStack[++stackTop] = lastsmall + 1;
            leftRightStack[++stackTop] = right;
        }
    }

    //showArrayContents(index, arraySize);

    free(leftRightStack);

    return index;
}

void showArrayContents(int* arr, int arraySize, int pivot, int comparison)
{
    for (int i = 0; i < arraySize; i++)
    {
        if (i == pivot)
        {
            if ((arr[i] / 10) > 0)
                cout << "vv" << " ";
            else
                cout << "v" << " ";
        }
        else if (i == comparison)
        {
            if ((arr[i] / 10) > 0)
                cout << "--" << " ";
            else
                cout << "-" << " ";
        }
        else
        {
            if ((arr[i] / 10) > 0)
                cout << "  " << " ";
            else
                cout << " " << " ";
        }
    }

    cout << endl;

    showArrayContents(arr, arraySize);
}

int findMiddleValue(int arr[3])
{
    bubbleSort(arr, 3);

    return arr[1];
}

void bubbleSort(int* a, int size) {
    int temp, i, bubblePass;
    for (i = 0; i < size - 1; i++)
        for (bubblePass = 1; bubblePass < size; bubblePass++)
            if (a[bubblePass - 1] > a[bubblePass]) {
                temp = a[bubblePass - 1];
                a[bubblePass - 1] = a[bubblePass];
                a[bubblePass] = temp;
            }
}

void showArrayContents(int* arr, int arraySize)
{
    for (int i = 0; i < arraySize; i++) cout << arr[i] << " ";
    cout << endl;
}

void populateArrayWithPsuedoRandomNumbers(int* arr, int arraySize)
{
    srand(time(NULL)); // set seed for random number generator
    for (unsigned int i = 0; i < arraySize; i++) arr[i] = rand() % 100;
}

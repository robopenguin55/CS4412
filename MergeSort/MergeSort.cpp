#include <iostream>
#include <cmath>
#include <string>
using namespace std;
void doMerge(int* a, int low, int high, int mid);
void mergeSort(int* a, int low, int high);
int main() 
{
    const int listSize = 25;
    int list[listSize];
    for (int i = 0; i < listSize; i++) list[i] = rand() % 100;
    for (int i = 0; i < listSize; i++) cout << list[i] << " ";
    cout << endl;
    mergeSort(list, 0, listSize - 1);
    for (int i = 0; i < listSize; i++) cout << list[i] << " ";
    cout << endl;
    int x;
    cin >> x;
}
void mergeSort(int* a, int low, int high) 
{
    int mid;
    if (low < high) {
        mid = (high + low) / 2;
        mergeSort(a, low, mid);
        mergeSort(a, mid + 1, high);
        doMerge(a, low, high, mid);
    }
}
//merge already sorted sublists (low-mid, mid+1to high)
// into single list from low to high
void doMerge(int* a, int low, int high, int mid) 
{
    int i, j, k, temp[50];
    k = low;
    i = low;
    j = mid + 1;
    while (i <= mid && j <= high)
    {
        if (a[i] < a[j])
        {
            temp[k] = a[i];

            i++;
        }
        else
        {
            temp[k] = a[j];

            j++;
        }

        k++;
    }

    while (i <= mid) temp[k++] = a[i++];
    while (j <= high) temp[k++] = a[j++];
    for (i = low; i < k; i++) a[i] = temp[i];
}

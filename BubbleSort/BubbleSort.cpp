#include <iostream>
#include <cmath>
#include <string>
using namespace std;
void bubbleSort(int* a, int size);
int main() {
	const int size = 20;
	int list[size];
	for (int i = 0; i < size; i++) list[i] = rand() % 100;
	for (int i = 0; i < size; i++) cout << list[i] << " ";
	cout << endl;
	bubbleSort(list, size);
	for (int i = 0; i < size; i++) cout << list[i] << " ";
	cout << endl;
	int x;
	cin >> x;
	return 0;
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


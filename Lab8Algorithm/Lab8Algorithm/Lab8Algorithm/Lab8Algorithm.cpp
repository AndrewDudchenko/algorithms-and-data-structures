#include <iostream>
#include <vector>
#include <iomanip>
#include <ctime>
#include <chrono>

using namespace std;
using namespace std::chrono;

long long comparisons = 0;
long long assignments = 0;

int Partition(vector<int>& arr, int p, int r) {
	// Вибір випадкового опорного елемента
	int pivotIndex = p + rand() % (r - p + 1);
	swap(arr[pivotIndex], arr[r]);
	assignments += 3;

	int x = arr[r];
	assignments++;
	int i = p - 1;
	for (int j = p; j <= r - 1; j++) {
		comparisons++;
		if (arr[j] <= x) {
			i++;
			swap(arr[i], arr[j]);
			assignments += 3;
		}
	}
	swap(arr[i + 1], arr[r]);
	assignments += 3;
	return i + 1;
}

void QuickSort(vector<int>& arr, int p, int r) {
	if (p < r) {
		int q = Partition(arr, p, r);
		QuickSort(arr, p, q - 1);
		QuickSort(arr, q + 1, r);
	}
}

void RunTest(vector<int> arr, string type) {
	comparisons = 0;
	assignments = 0;
	int n = arr.size();
	auto start = high_resolution_clock::now();
	if (n > 0) {
		QuickSort(arr, 0, n - 1);
	}
	auto stop = high_resolution_clock::now();
	duration<double, milli> elapsed = stop - start;
	cout << setw(12) << type << " | "
		<< setw(10) << fixed << setprecision(3) << elapsed.count() << " | "
		<< setw(15) << comparisons << " | "
		<< setw(15) << assignments << endl;
}

int main() {
	srand(time(0));

	int sizes[] = { 10, 100, 1000, 5000, 10000 };

	for (int n : sizes) {
		cout << "Test for n = " << n << ":" << endl;
		cout << setw(12) << "Data Type" << " | "
			<< setw(10) << "Time (ms)" << " | "
			<< setw(15) << "Comparisons" << " | "
			<< setw(15) << "Assignments" << endl;
		cout << "-------------------------------------------------------------" << endl;

		// 1. Випадкова послідовність
		vector<int> randomData;
		for (int i = 0; i < n; i++) randomData.push_back(rand() % 100000);
		RunTest(randomData, "Random");

		// 2. Зростаюча послідовність
		vector<int> ascData;
		for (int i = 0; i < n; i++) ascData.push_back(i);
		RunTest(ascData, "Ascending");

		// 3. Спадна послідовність
		vector<int> descData;
		for (int i = n; i > 0; i--) descData.push_back(i);
		RunTest(descData, "Descending");

		cout << endl;
	}

	return 0;

}

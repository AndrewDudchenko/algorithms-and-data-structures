#include <iostream>
#include <vector>
#include <iomanip>
#include <ctime>
#include <chrono>

using namespace std;
using namespace std::chrono;

long long comparisons = 0;
long long assignments = 0;

void MaxHeapify(vector<int>& arr, int n, int i) {
	int largest = i;
	int left = 2 * i + 1;
	int right = 2 * i + 2;
	comparisons++;
	if (left < n && arr[left] > arr[largest]) {
		largest = left;
	}
	comparisons++;
	if (right < n && arr[right] > arr[largest]) {
		largest = right;
	}
	if (largest != i) {
		swap(arr[i], arr[largest]);
		assignments += 3;
		MaxHeapify(arr, n, largest);
	}
}

void BuildMaxHeap(vector<int>& arr, int n) {
	for (int i = n / 2 - 1; i >= 0; i--) {
		MaxHeapify(arr, n, i);
	}
}

void HeapSort(vector<int>& arr) {
	int n = arr.size();
	BuildMaxHeap(arr, n);
	for (int i = n - 1; i > 0; i--) {
		swap(arr[0], arr[i]);
		assignments += 3;
		MaxHeapify(arr, i, 0);
	}
}

void RunTest(vector<int> arr, string type) {
	comparisons = 0;
	assignments = 0;
	int n = arr.size();
	auto start = high_resolution_clock::now();
	if (n > 0) {
		HeapSort(arr);
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
#include <iostream>
#include <vector>
#include <iomanip>
#include <ctime>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Функція для сортування вставками та підрахунку кількості порівнянь і присвоєнь
void InsertionSort(vector<int> A, string type) {
    int n = A.size();
    long long comparisons = 0;
    long long assignments = 0;


	auto start = high_resolution_clock::now();
    // Сортування вставками
    for (int j = 1; j < n; j++) {
        int key = A[j];
        assignments++;

        int i = j - 1;

        
        while (i >= 0) {
            comparisons++;
            if (A[i] > key) {
                A[i + 1] = A[i];
                assignments++;
                i = i - 1;
            }
            else {
                break;
            }
        }
        A[i + 1] = key;
        assignments++;
    }

	auto stop = high_resolution_clock::now();
	duration<double, milli> elapsed = stop - start;

    // Вивід результатів у табличному форматі
    cout << setw(12) << type << " | "
        << setw(10) << fixed << setprecision(3) << elapsed.count() << " | "
        << setw(15) << comparisons << " | "
        << setw(15) << assignments << endl;
}

int main() {
    srand(time(0));

    int sizes[] = { 16, 21, 6, 9, 11,4, 18, 5, 31, 2 };

    // Тестування для різних розмірів масивів
    for (int n : sizes) {
        cout << "Test for n = " << n << ":" << endl;
        cout << setw(12) << "Data Type" << " | "
            << setw(10) << "Time" << " | "
            << setw(15) << "Comparisons" << " | "
            << setw(15) << "Assignments" << endl;
        cout << "-------------------------------------------------------------" << endl;

        // 1. Випадкова
        vector<int> randomData;
        for (int i = 0; i < n; i++) randomData.push_back(rand() % 100000);
        InsertionSort(randomData, "Random");

        // 2. Зростаючиа
        vector<int> ascData;
        for (int i = 0; i < n; i++) ascData.push_back(i);
        InsertionSort(ascData, "Ascending");

        // 3. Спадна
        vector<int> descData;
        for (int i = n; i > 0; i--) descData.push_back(i);
        InsertionSort(descData, "Descending");

        cout << endl;
    }

    return 0;
}
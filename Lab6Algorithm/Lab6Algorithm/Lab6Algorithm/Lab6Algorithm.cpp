#include <iostream>
#include <vector>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <climits>

using namespace std;
using namespace std::chrono;

long long comparisons = 0;
long long assignments = 0;

// Функція для злиття двох частин масиву згідно з лекційним псевдокодом
void Merge(vector<int>& A, int p, int q, int r) {
    int n1 = q - p + 1;
    int n2 = r - q;

    // Створюємо масиви L[0..n1] та R[0..n2] (розмір +1 для вартового)
    vector<int> L(n1 + 1);
    vector<int> R(n2 + 1);

    // Копіювання даних у тимчасові масиви
    for (int i = 0; i < n1; i++) {
        L[i] = A[p + i];
        assignments++;
    }
    for (int j = 0; j < n2; j++) {
        R[j] = A[q + 1 + j];
        assignments++;
    }

    // Встановлення "вартових" (нескінченність)
    L[n1] = INT_MAX;
    R[n2] = INT_MAX;

    int i = 0;
    int j = 0;

    // Одне злиття за допомогою циклу for
    for (int k = p; k <= r; k++) {
        comparisons++;
        if (L[i] <= R[j]) {
            A[k] = L[i];
            i++;
        }
        else {
            A[k] = R[j];
            j++;
        }
        assignments++;
    }
}

// Рекурсивна функція сортування злиттям
void MergeSort(vector<int>& A, int p, int r) {
    if (p < r) {
        // Знаходження середини
        int q = (p + r) / 2;
        MergeSort(A, p, q);
        MergeSort(A, q + 1, r);
        Merge(A, p, q, r);
    }
}

// Функція-обертка для проведення тестів
void RunTest(vector<int> A, string type) {
    comparisons = 0;
    assignments = 0;
    int n = A.size();

    auto start = high_resolution_clock::now();

    if (n > 0) {
        MergeSort(A, 0, n - 1);
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
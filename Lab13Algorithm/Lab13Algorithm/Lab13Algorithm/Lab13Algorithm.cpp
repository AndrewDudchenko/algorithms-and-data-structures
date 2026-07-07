#include <iostream>
#include <vector>
#include <unordered_set>
#include <climits>
#include <random>
#include <algorithm>
using namespace std;

// Структура для зберігання ребра графа
struct Edge {
    int destination;
    int weight;
};

// 1. Динамічне програмування на графах
void solveGraphTask() {
    int numVertices = 16; // Кількість вершин
    vector<vector<Edge>> adjacencyList(numVertices + 1);

    adjacencyList[1] = { {2, 4}, {3, 5}, {4, 4}, {5, 2} };
    adjacencyList[2] = { {6, 3}, {7, 4} };
    adjacencyList[3] = { {6, 1}, {7, 1}, {8, 3} };
    adjacencyList[4] = { {6, 4}, {7, 5}, {8, 5} };
    adjacencyList[5] = { {7, 2}, {8, 5} };
    adjacencyList[6] = { {9, 3}, {10, 4}, {12, 6} };
    adjacencyList[7] = { {9, 6}, {10, 7}, {11, 7} };
    adjacencyList[8] = { {10, 4}, {11, 4}, {12, 4} };
    adjacencyList[9] = { {13, 7}, {15, 6} };
    adjacencyList[10] = { {13, 4}, {14, 7} };
    adjacencyList[11] = { {14, 8}, {15, 7} };
    adjacencyList[12] = { {13, 9}, {15, 10} };
    adjacencyList[13] = { {16, 2} };
    adjacencyList[14] = { {16, 3} };
    adjacencyList[15] = { {16, 2} };

    vector<int> minDistance(numVertices + 1, INT_MAX); // Масив мінімальних відстаней
    vector<int> predecessor(numVertices + 1, -1); // Масив для відновлення шляху

    minDistance[1] = 0; // Відстань від початкової вершини до неї самої дорівнює 0

    // Оскільки вершини вже відсортовані топологічно (за рівнями), ми можемо просто пройтись по них від 1 до N.
    for (int currentVertex = 1; currentVertex <= numVertices; ++currentVertex) {
        if (minDistance[currentVertex] != INT_MAX) {
            for (const auto& edge : adjacencyList[currentVertex]) {
                int neighbor = edge.destination;
                int weight = edge.weight;

                // Рекурентне співвідношення ДП: якщо шлях через поточну вершину коротший, оновлюємо
                if (minDistance[currentVertex] + weight < minDistance[neighbor]) {
                    minDistance[neighbor] = minDistance[currentVertex] + weight;
                    predecessor[neighbor] = currentVertex;
                }
            }
        }
    }

    cout << "--- Task 1: Shortest Path Search ---" << endl;
    cout << "Minimum path cost from 1 to 16: " << minDistance[16] << endl;

    // Відновлення шляху з кінця до початку
    vector<int> optimalPath;
    for (int v = 16; v != -1; v = predecessor[v]) {
        optimalPath.push_back(v);
    }
    reverse(optimalPath.begin(), optimalPath.end());

    cout << "Optimal path: ";
    for (size_t i = 0; i < optimalPath.size(); ++i) {
        cout << optimalPath[i] << (i + 1 < optimalPath.size() ? " -> " : "");
    }
    cout << endl;
}

// 2-4. Робота з хеш-таблицею
void solveHashTableTask() {
    cout << "--- Task 2-4: Hash Table Operations ---" << endl;

    unordered_set<int> numberSet;

    // Ініціалізація генератора випадкових чисел
    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<> randomDist(1, 100);

    // 2. Заповнення хеш-таблиці випадковими числами
    cout << "Initial elements: ";
    for (int i = 0; i < 15; ++i) {
        int randomNum = randomDist(rng);
        numberSet.insert(randomNum);
        cout << randomNum << " ";
    }
    cout << endl;

    // 3. Видалення всіх парних чисел
    for (auto it = numberSet.begin(); it != numberSet.end(); ) {
        if (*it % 2 == 0) {
            it = numberSet.erase(it);
        }
        else {
            ++it;
        }
    }

    // 4. Виведення в консоль елементів, що залишилися
    cout << "Elements after removing even numbers: ";
    if (numberSet.empty()) {
        cout << "Table is empty.";
    }
    else {
        for (int num : numberSet) {
            cout << num << " ";
        }
    }
    cout << endl;
}

int main() {
    solveGraphTask();
    solveHashTableTask();
    return 0;
}
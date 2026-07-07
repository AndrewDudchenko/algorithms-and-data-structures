#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <string>

using namespace std;

// --- Структури даних ---

// Структура ребра для алгоритму Крускала
struct Edge {
    int startVertex;
    int endVertex;
    int weight;

    // Функція для сортування ребер за зростанням ваги
    bool operator<(const Edge& otherEdge) const {
        return weight < otherEdge.weight;
    }
};

// Система неперетинних множин (Disjoint Set Union)
struct DisjointSetUnion {
    vector<int> parentVertex;
    vector<int> treeRank;

    // Конструктор
    DisjointSetUnion(int numberOfVertices) {
        parentVertex.resize(numberOfVertices);
        treeRank.assign(numberOfVertices, 0);
        for (int i = 0; i < numberOfVertices; i++) {
            parentVertex[i] = i;
        }
    }

    // Пошук головної вершини групи зі стисненням шляху
    int findSet(int vertex, long long& operationsCount) {
        operationsCount++;
        if (vertex == parentVertex[vertex]) {
            return vertex;
        }
        operationsCount++;
        parentVertex[vertex] = findSet(parentVertex[vertex], operationsCount);
        return parentVertex[vertex];
    }

    // Об'єднання двох множин за рангом (щоб дерево не ставало занадто глибоким)
    void unionSets(int vertexA, int vertexB, long long& operationsCount) {
        vertexA = findSet(vertexA, operationsCount);
        vertexB = findSet(vertexB, operationsCount);

        operationsCount++;
        if (vertexA != vertexB) {
            operationsCount++;
            if (treeRank[vertexA] < treeRank[vertexB]) {
                // Зміна місцями через тимчасову змінну
                int temporary = vertexA;
                vertexA = vertexB;
                vertexB = temporary;
                operationsCount++;
            }
            parentVertex[vertexB] = vertexA;
            operationsCount++;

            if (treeRank[vertexA] == treeRank[vertexB]) {
                treeRank[vertexA]++;
                operationsCount++;
            }
        }
    }
};

// --- Алгоритми ---

// Алгоритм Крускала
void runKruskal(int verticesCount, vector<Edge> edges, double& timeTaken, long long& operationsCount) {
    operationsCount = 0;
    clock_t startTime = clock();

    // Сортування ребер (швидке сортування з бібліотеки algorithm)
    sort(edges.begin(), edges.end());
    if (edges.size() > 0) {
        // Додаємо теоретичну кількість операцій для швидкого сортування: E * log2(E)
        operationsCount += edges.size() * (log(edges.size()) / log(2));
    }

    DisjointSetUnion dsu(verticesCount);
    int edgesAddedCount = 0;

    for (int i = 0; i < edges.size(); i++) {
        Edge currentEdge = edges[i];
        operationsCount++;

        // Перевірка на відсутність циклу (якщо вершини в різних групах)
        if (dsu.findSet(currentEdge.startVertex, operationsCount) != dsu.findSet(currentEdge.endVertex, operationsCount)) {
            dsu.unionSets(currentEdge.startVertex, currentEdge.endVertex, operationsCount);
            edgesAddedCount++;
            operationsCount++;
        }

        operationsCount++;
        // Якщо додали достатньо ребер для створення дерева - зупиняємось
        if (edgesAddedCount == verticesCount - 1) {
            break;
        }
    }

    clock_t endTime = clock();
    timeTaken = (double)(endTime - startTime) / CLOCKS_PER_SEC;
}

// Алгоритм Прима (Реалізація через матрицю суміжності)
void runPrim(int verticesCount, const vector<vector<int>>& adjacencyMatrix, double& timeTaken, long long& operationsCount) {
    operationsCount = 0;
    const int INFINITY_VALUE = 1000000000;

    clock_t startTime = clock();

    // Масив найкоротших відстаней до дерева
    vector<int> minimumEdgeWeight(verticesCount, INFINITY_VALUE);
    // Прапорці: чи знаходиться вершина вже в остовному дереві
    vector<bool> inSpanningTree(verticesCount, false);

    minimumEdgeWeight[0] = 0;
    operationsCount += verticesCount;

    for (int count = 0; count < verticesCount; count++) {
        operationsCount++;
        int currentMinimumWeight = INFINITY_VALUE;
        int currentVertex = -1;

        // Лінійний пошук вершини з найменшою відстанню
        for (int vertex = 0; vertex < verticesCount; vertex++) {
            operationsCount += 2;
            if (inSpanningTree[vertex] == false && minimumEdgeWeight[vertex] < currentMinimumWeight) {
                currentMinimumWeight = minimumEdgeWeight[vertex];
                currentVertex = vertex;
                operationsCount += 2;
            }
        }

        // Якщо граф розірваний на частини
        if (currentVertex == -1) {
            break;
        }

        inSpanningTree[currentVertex] = true;
        operationsCount += 2;

        // Оновлення відстаней до сусідів по матриці суміжності
        for (int neighbor = 0; neighbor < verticesCount; neighbor++) {
            operationsCount += 2;
            if (adjacencyMatrix[currentVertex][neighbor] != 0 && inSpanningTree[neighbor] == false && adjacencyMatrix[currentVertex][neighbor] < minimumEdgeWeight[neighbor]) {
                minimumEdgeWeight[neighbor] = adjacencyMatrix[currentVertex][neighbor];
                operationsCount++;
            }
        }
    }

    clock_t endTime = clock();
    timeTaken = (double)(endTime - startTime) / CLOCKS_PER_SEC;
}

// --- Генератор випадкових графів ---

void generateGraph(int verticesCount, int densityPercentage, vector<Edge>& edgeList, vector<vector<int>>& adjacencyMatrix) {
    edgeList.clear();
    // Створення двовимірної матриці суміжності, заповненої нулями
    adjacencyMatrix.assign(verticesCount, vector<int>(verticesCount, 0));

    for (int row = 0; row < verticesCount; row++) {
        for (int col = row + 1; col < verticesCount; col++) {
            int randomChance = rand() % 100;

            // Якщо випадкове число менше за відсоток щільності - створюємо ребро
            if (randomChance < densityPercentage) {
                int weight = (rand() % 100) + 1;

                // Додаємо ребро у список для алгоритму Крускала
                Edge newEdge;
                newEdge.startVertex = row;
                newEdge.endVertex = col;
                newEdge.weight = weight;
                edgeList.push_back(newEdge);

                // Додаємо вагу ребра в матрицю суміжності для алгоритму Прима
                adjacencyMatrix[row][col] = weight;
                adjacencyMatrix[col][row] = weight;
            }
        }
    }
}

// --- Головна функція ---

int main() {
    // Ініціалізація генератора випадкових чисел
    srand(time(0));

    // Параметри 10 тестів: {Кількість вершин, Щільність у відсотках}
    int testCases[10][2] = {
        {500, 10},  {500, 90},
        {1000, 10}, {1000, 40}, {1000, 90},
        {1500, 10}, {1500, 90},
        {2000, 5},  {2000, 20}, {2000, 80}
    };

    // Виведення вирівняного заголовка таблиці
    cout << left
        << "| " << setw(3) << "#"
        << "| " << setw(9) << "Vertices"
        << "| " << setw(8) << "Density"
        << "| " << setw(16) << "Kruskal Time(s)"
        << "| " << setw(13) << "Kruskal Ops"
        << "| " << setw(13) << "Prim Time(s)"
        << "| " << setw(11) << "Prim Ops"
        << "| " << setw(8) << "Winner" << " |" << endl;

    cout << string(103, '-') << endl;

    for (int index = 0; index < 10; index++) {
        int verticesCount = testCases[index][0];
        int densityPercentage = testCases[index][1];

        vector<Edge> edgeList;
        vector<vector<int>> adjacencyMatrix;

        // Генеруємо однаковий граф одразу для обох алгоритмів
        generateGraph(verticesCount, densityPercentage, edgeList, adjacencyMatrix);

        double kruskalTime = 0.0;
        long long kruskalOperations = 0;
        runKruskal(verticesCount, edgeList, kruskalTime, kruskalOperations);

        double primTime = 0.0;
        long long primOperations = 0;
        runPrim(verticesCount, adjacencyMatrix, primTime, primOperations);

        // Визначаємо переможця через класичний if-else
        string winnerName = "";
        if (kruskalTime < primTime) {
            winnerName = "Kruskal";
        }
        else {
            winnerName = "Prim";
        }

        // Додаємо знак відсотка до тексту щільності
        string densityString = to_string(densityPercentage) + "%";

        // Виведення рядка таблиці з результатами (роздільники за межами setw)
        cout << "| " << left << setw(3) << (index + 1)
            << "| " << right << setw(8) << verticesCount << " "
            << "| " << right << setw(7) << densityString << " "
            << "| " << fixed << setprecision(6) << right << setw(15) << kruskalTime << " "
            << "| " << right << setw(12) << kruskalOperations << " "
            << "| " << right << setw(12) << primTime << " "
            << "| " << right << setw(10) << primOperations << " "
            << "| " << left << setw(8) << winnerName << " |" << endl;
    }

    return 0;
}
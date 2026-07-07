#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <chrono>

using namespace std;

// БЛОК АЛГОРИТМУ КРУСКАЛА
struct rebro {
    int versh1;
    int versh2;
    int vaga;

    bool operator<(const rebro& other) const {
        return vaga < other.vaga;
    }
};

struct DSU {
    vector<int> connections;

    DSU(int kil_ver) {
        connections.resize(kil_ver + 1);
        for (int i = 1; i <= kil_ver; i++) {
            connections[i] = i;
        }
    }

    int find_set(int v) {
        if (v == connections[v])
            return v;
        return connections[v] = find_set(connections[v]);
    }

    void unite(int a, int b) {
        a = find_set(a);
        b = find_set(b);
        if (a != b) {
            connections[b] = a;
        }
    }
};

void kruskal_algorithm(int kil_ver, vector<rebro> rebra) {
    sort(rebra.begin(), rebra.end());

    DSU dsu(kil_ver);

    size_t i = 0;
    int sum = 0;
    vector<rebro> MST;

    while (MST.size() < kil_ver - 1 && i < rebra.size()) {
        int u = rebra[i].versh1;
        int v = rebra[i].versh2;

        if (dsu.find_set(u) != dsu.find_set(v)) {
            MST.push_back(rebra[i]);
            dsu.unite(u, v);
            sum += rebra[i].vaga;
        }
        i++;
    }

    cout << "Minimalna vaga ostovnoho dereva (Kruskal): " << sum << endl;
    cout << "Rebra v derevi:" << endl;
    for (const auto& r : MST) {
        cout << r.versh1 << " - " << r.versh2 << " (vaga: " << r.vaga << ")" << endl;
    }
}

// БЛОК АЛГОРИТМУ ПРИМА
struct edge {
    int to;
    int weight;
};

struct queue_item {
    int vertex;
    int key;

    bool operator<(const queue_item& other) const {
        return key > other.key; // Найменший ключ зверху
    }
};

const int INF = 1e9;

void prim_algorithm(int kil_ver, const vector<vector<edge>>& graph, int start_vertex) {
    vector<int> key(kil_ver + 1, INF);
    vector<int> pi(kil_ver + 1, -1);
    vector<bool> in_MST(kil_ver + 1, false);

    priority_queue<queue_item> pq;

    key[start_vertex] = 0;
    pq.push({ start_vertex, 0 });

    int sum_mst = 0;

    while (!pq.empty()) {
        int u = pq.top().vertex;
        pq.pop();

        if (in_MST[u]) continue;

        in_MST[u] = true;
        sum_mst += key[u];

        for (const edge& e : graph[u]) {
            int v = e.to;
            int weight = e.weight;

            if (!in_MST[v] && weight < key[v]) {
                pi[v] = u;
                key[v] = weight;
                pq.push({ v, key[v] });
            }
        }
    }

    cout << "Minimalna vaga ostovnoho dereva (Prim): " << sum_mst << endl;
    cout << "Rebra v derevi:" << endl;
    for (int i = 1; i <= kil_ver; i++) {
        if (pi[i] != -1) {
            cout << pi[i] << " - " << i << " (vaga: " << key[i] << ")" << endl;
        }
    }
}

// ГОЛОВНА ФУНКЦІЯ
int main() {
    int kil_ver = 10;

    vector<rebro> rebra = {
        {1, 2, 2}, {1, 3, 3}, {1, 4, 1},
        {2, 4, 10}, {2, 5, 8},
        {3, 4, 7}, {3, 6, 3},
        {4, 5, 6}, {4, 7, 5},
        {5, 10, 10},
        {6, 7, 4}, {6, 9, 11},
        {7, 8, 13}, {7, 9, 12},
        {8, 10, 14},
        {9, 10, 12}
    };

    vector<vector<edge>> graph(kil_ver + 1);
    for (const auto& r : rebra) {
        graph[r.versh1].push_back({ r.versh2, r.vaga });
        graph[r.versh2].push_back({ r.versh1, r.vaga });
    }

    cout << "=== ALGORYTM KRUSKALA ===" << endl;
    auto start_k = chrono::high_resolution_clock::now();

    kruskal_algorithm(kil_ver, rebra);

    auto end_k = chrono::high_resolution_clock::now();
    auto time_k = chrono::duration_cast<chrono::microseconds>(end_k - start_k).count();
    cout << "Chas vykonannya: " << time_k << " mikrosekund" << endl;

    cout << "=== ALGORYTM PRYMA ===" << endl;
    auto start_p = chrono::high_resolution_clock::now();

    prim_algorithm(kil_ver, graph, 1);

    auto end_p = chrono::high_resolution_clock::now();
    auto time_p = chrono::duration_cast<chrono::microseconds>(end_p - start_p).count();
    cout << "Chas vykonannya: " << time_p << " mikrosekund" << endl;

    return 0;
}
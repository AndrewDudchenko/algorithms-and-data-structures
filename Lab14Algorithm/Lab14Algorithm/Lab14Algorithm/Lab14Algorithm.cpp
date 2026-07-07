#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

const int N = 8;

void solveDP(const vector<vector<int>>& P) {
    // Матриця для зберігання максимальних накопичених призів F[i][j]
    vector<vector<int>> F(N, vector<int>(N, 0));

    // ЗВОРОТНИЙ ХІД
    // Кінцева точка: права верхня клітинка
    F[0][7] = P[0][7];

    // Заповнюємо верхній рядок (можливий рух тільки вправо, тому зворотний - вліво)
    for (int j = N - 2; j >= 0; --j) {
        F[0][j] = P[0][j] + F[0][j + 1];
    }

    // Заповнюємо правий стовпець (можливий рух тільки вгору, тому зворотний - вниз)
    for (int i = 1; i < N; ++i) {
        F[i][7] = P[i][7] + F[i - 1][7];
    }

    // Заповнюємо решту матриці за рекурентним співвідношенням:
    // F[i][j] = P[i][j] + max{ F[i][j+1] (вправо), F[i-1][j] (вгору), F[i-1][j+1] (по діагоналі) }
    for (int i = 1; i < N; ++i) {
        for (int j = N - 2; j >= 0; --j) {
            F[i][j] = P[i][j] + max({ F[i][j + 1], F[i - 1][j], F[i - 1][j + 1] });
        }
    }

    // Виведення розрахованої матриці F[i][j] для перевірки та звіту
    cout << "Calculated matrix F[i][j]:";
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << setw(4) << F[i][j] << " ";
        }
        cout << endl;
    }

    // Максимальний приз знаходиться у стартовій клітинці - ліва нижня (7, 0)
    int maxPrize = F[N - 1][0];
    cout << "Maximum possible prize: " << maxPrize << endl;

    // ПРЯМИЙ ХІД (Відновлення шляху)
    cout << "Optimal path (in mathematical coordinates (i, j)):";
    int r = N - 1; // Поточний рядок
    int c = 0; // Поточний стовпець
    int currentSum = P[r][c];

    // знизу вгору
    auto print_step = [&](int row, int col, int prize, int total) {
        int math_i = 8 - row;
        int math_j = col + 1;
        cout << "Cell (" << math_i << ", " << math_j << ") | "
            << "Prize: " << setw(2) << prize
            << " | Accumulated: " << total << "\n";
        };

    print_step(r, c, P[r][c], currentSum);

    // Поки не досягли кінцевої клітинки (0, 7)
    while (r > 0 || c < N - 1) {
        int next_r = r, next_c = c;
        int max_val = -1e9;

        // Перевірка руху вправо
        if (c + 1 < N && F[r][c + 1] > max_val) {
            max_val = F[r][c + 1];
            next_r = r;
            next_c = c + 1;
        }
        // Перевірка руху вгору
        if (r - 1 >= 0 && F[r - 1][c] > max_val) {
            max_val = F[r - 1][c];
            next_r = r - 1;
            next_c = c;
        }
        // Перевірка руху по діагоналі вгору-вправо
        if (r - 1 >= 0 && c + 1 < N && F[r - 1][c + 1] > max_val) {
            max_val = F[r - 1][c + 1];
            next_r = r - 1;
            next_c = c + 1;
        }

        r = next_r;
        c = next_c;
        currentSum += P[r][c];
        print_step(r, c, P[r][c], currentSum);
    }
}

int main() {
    vector<vector<int>> P = {
            { 2, -1,  5,  4, -2, -3,  4,  6},
            {-1,  2, -1, -1,  6,  2, -3,  1},
            { 1, -1,  4,  3, -1,  2, -1,  4},
            { 3, -2,  6,  3,  3, -4, -4,  6},
            {-1,  4, -1,  3, -5,  1, -3, -3},
            {-2, -2, -1, -5,  3,  1, -3, -1},
            {-1, -3, -1,  3, -2,  3, -1, -3},
            { 4, -1, -1, -3, -2, -1,  4,  2}
    };

    solveDP(P);

    return 0;
}
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <iomanip>
#include <string>

using namespace std;

// ГЛОБАЛЬНІ КОНСТАНТИ ТА НАЛАШТУВАННЯ АЛГОРИТМУ
const int POPULATION_SIZE = 1000; // Розмір популяції
const int MAX_GENERATIONS = 1000; // Умова завершення: кількість поколінь
const double CROSSOVER_RATE = 0.8; // Ймовірність схрещування (80%)
const double MUTATION_RATE = 0.1; // Ймовірність мутації (10%)
const double MUTATION_DELTA = 0.5; // Величина зсуву при мутації (для локального пошуку)
const double PENALTY_WEIGHT = 1e6; // Вага штрафу за порушення обмежень

// Межі для пошуку генів. Центр кола (1,1), радіус 6. Оскільки x1>=1, x2>=1, межі [1.0, 7.0]
const double GENE_MIN = 1.0;
const double GENE_MAX = 7.0;

// ОПИС ОБ'ЄКТІВ
struct Individual {
    double x1;
    double x2;
    double fitness;
};

// Сучасний генератор випадкових чисел C++ (краще за старий rand())
random_device rd;
mt19937 gen(rd());
uniform_real_distribution<double> dis_gene(GENE_MIN, GENE_MAX);
uniform_real_distribution<double> dis_prob(0.0, 1.0);
uniform_real_distribution<double> dis_mutation(-MUTATION_DELTA, MUTATION_DELTA);

// МАТЕМАТИЧНА МОДЕЛЬ
double objectiveFunction(double x1, double x2) {
    return 4.0 * x1 + 3.0 * x2;
}

// Перевірка системи обмежень (чи знаходиться точка в дозволеній зоні)
bool isFeasible(double x1, double x2) {
    double constraint = x1 * x1 - 2.0 * x1 + x2 * x2 - 2.0 * x2 - 34.0;
    return (constraint <= 0.0) && (x1 >= 1.0) && (x2 >= 1.0);
}

// Метод штрафів для обробки обмежень. Чим більше порушення, тим більший штраф. Це дозволяє алгоритму досліджувати навіть невідповідні рішення, але з великою вартістю, що сприяє пошуку допустимих оптимумів.
// maximize=true (шукаємо максимум), maximize=false (шукаємо мінімум)
double calculateFitness(double x1, double x2, bool maximize) {
    double objValue = objectiveFunction(x1, x2);
    double penalty = 0.0;

    // Розрахунок порушень. Чим сильніше порушено обмеження, тим більший штраф.
    double c1 = x1 * x1 - 2.0 * x1 + x2 * x2 - 2.0 * x2 - 34.0;
    if (c1 > 0.0) penalty += PENALTY_WEIGHT * c1;
    if (x1 < 1.0) penalty += PENALTY_WEIGHT * (1.0 - x1);
    if (x2 < 1.0) penalty += PENALTY_WEIGHT * (1.0 - x2);

    if (maximize) {
        return objValue - penalty;
    }
    else {
        return -objValue - penalty;
    }
}

// ГЕНЕТИЧНІ ОПЕРАТОРИ
// Турнірний відбір
Individual tournamentSelection(const vector<Individual>& pop) {
    int bestIdx = uniform_int_distribution<>(0, POPULATION_SIZE - 1)(gen);
    Individual best = pop[bestIdx];

    for (int i = 1; i < 3; ++i) { // Розмір турніру = 3
        int randIdx = uniform_int_distribution<>(0, POPULATION_SIZE - 1)(gen);
        if (pop[randIdx].fitness > best.fitness) {
            best = pop[randIdx];
        }
    }
    return best;
}

// Арифметичне схрещування
void crossover(const Individual& p1, const Individual& p2, Individual& c1, Individual& c2) {
    if (dis_prob(gen) < CROSSOVER_RATE) {
        double alpha = dis_prob(gen);
        c1.x1 = alpha * p1.x1 + (1.0 - alpha) * p2.x1;
        c1.x2 = alpha * p1.x2 + (1.0 - alpha) * p2.x2;

        c2.x1 = (1.0 - alpha) * p1.x1 + alpha * p2.x1;
        c2.x2 = (1.0 - alpha) * p1.x2 + alpha * p2.x2;
    }
    else {
        c1 = p1;
        c2 = p2;
    }
}

// Мутація із зсувом (Delta Mutation) - краще для точного локального пошуку
void mutate(Individual& ind) {
    if (dis_prob(gen) < MUTATION_RATE) {
        ind.x1 += dis_mutation(gen);
        ind.x1 = max(GENE_MIN - 0.5, min(GENE_MAX + 0.5, ind.x1)); // Дозволяємо легкий вихід за межі для дослідження
    }
    if (dis_prob(gen) < MUTATION_RATE) {
        ind.x2 += dis_mutation(gen);
        ind.x2 = max(GENE_MIN - 0.5, min(GENE_MAX + 0.5, ind.x2));
    }
}

// ГОЛОВНИЙ ЦИКЛ ГЕНЕТИЧНОГО АЛГОРИТМУ
Individual runGeneticAlgorithm(bool maximize) {
    // Ініціалізація популяції
    vector<Individual> pop(POPULATION_SIZE);
    for (auto& ind : pop) {
        ind.x1 = dis_gene(gen);
        ind.x2 = dis_gene(gen);
        ind.fitness = calculateFitness(ind.x1, ind.x2, maximize);
    }

    // Зберігаємо найкращого за всю історію
    Individual absoluteBest = pop[0];

    for (int genIdx = 0; genIdx < MAX_GENERATIONS; ++genIdx) {

        // Знаходимо найкращого в поточному поколінні
        auto currentBestIt = max_element(pop.begin(), pop.end(), [](const Individual& a, const Individual& b) {
            return a.fitness < b.fitness;
            });

        if (currentBestIt->fitness > absoluteBest.fitness) {
            absoluteBest = *currentBestIt;
        }

        vector<Individual> newPop;
        newPop.reserve(POPULATION_SIZE);

        // Елітизм: переносимо найкращого з поточного покоління без змін
        newPop.push_back(*currentBestIt);

        while (newPop.size() < POPULATION_SIZE) {
            Individual p1 = tournamentSelection(pop);
            Individual p2 = tournamentSelection(pop);
            Individual c1, c2;

            crossover(p1, p2, c1, c2);

            mutate(c1);
            mutate(c2);

            c1.fitness = calculateFitness(c1.x1, c1.x2, maximize);
            c2.fitness = calculateFitness(c2.x1, c2.x2, maximize);

            newPop.push_back(c1);
            if (newPop.size() < POPULATION_SIZE) newPop.push_back(c2);
        }
        pop = newPop;
    }
    return absoluteBest;
}

void printResult(const string& label, const Individual& ind) {
    cout << "--- " << label << " ---" << endl;
    cout << "  Optimal x1 = " << fixed << setprecision(5) << ind.x1 << endl;
    cout << "  Optimal x2 = " << fixed << setprecision(5) << ind.x2 << endl;
    cout << "  Function Z = " << fixed << setprecision(5) << objectiveFunction(ind.x1, ind.x2) << endl;

    cout << "  Is Feasible: ";
    if (isFeasible(ind.x1, ind.x2)) {
        cout << "YES";
    }
    else {
        cout << "NO";
    }
    cout << endl << endl;
}

int main() {
    cout << "=========================================================" << endl;
    cout << "LABORATORY WORK 15. GENETIC ALGORITHMS" << endl;
    cout << "Task Variant 3" << endl;
    cout << "Objective: f(x1, x2) = 4*x1 + 3*x2 -> extremum" << endl;
    cout << "Constraints:" << endl;
    cout << "  1) x1^2 - 2*x1 + x2^2 - 2*x2 - 34 <= 0" << endl;
    cout << "  2) x1 >= 1.0" << endl;
    cout << "  3) x2 >= 1.0" << endl;
    cout << "=========================================================" << endl << endl;

    cout << "GA Parameters: Pop Size=" << POPULATION_SIZE << ", Generations=" << MAX_GENERATIONS << endl;

    // Запуск для пошуку МАКСИМУМУ
    Individual maxResult = runGeneticAlgorithm(true);
    printResult("MAXIMUM RESULT", maxResult);

    // Запуск для пошуку МІНІМУМУ
    Individual minResult = runGeneticAlgorithm(false);
    printResult("MINIMUM RESULT", minResult);

    return 0;
}
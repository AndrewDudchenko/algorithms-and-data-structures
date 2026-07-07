#include <iostream>
#include <vector>
#include <cmath>
#include <random>

using namespace std;

// Налаштування генетичного алгоритму
const int POPULATION_SIZE = 5000; // Розмір популяції
const int MAX_GENERATIONS = 1000; // Максимальна кількість поколінь
const double MUTATION_RATE = 0.2; // Ймовірність мутації (20%)
const int MIN_VAL = -100; // Мінімальне значення для x та y
const int MAX_VAL = 100; // Максимальне значення для x та y

// Структура для представлення особини (можливого розв'язку)
struct Individual {
    int x;
    int y;
    int error; // Відхилення від нуля (чим менше, тим краще)
    double fitness; // Функція пристосованості
};

// Генератор випадкових чисел
random_device rd;
mt19937 gen(rd());

// Функція для генерації випадкового цілого числа в заданому діапазоні
int getRandomInt(int min, int max) {
    uniform_int_distribution<> dist(min, max);
    return dist(gen);
}

// Функція для генерації випадкового дійсного числа від 0 до 1
double getRandomDouble() {
    uniform_real_distribution<> dist(0.0, 1.0);
    return dist(gen);
}

int calculateEquationError(int x, int y) {
    return abs(x * x - 4 * x - y * y + 6);
}

// Обчислення пристосованості (fitness). Чим менша похибка, тим більший fitness
double calculateFitness(int error) {
    return 1.0 / (1.0 + error); // Якщо error == 0, fitness == 1.0 (ідеал)
}

// Ініціалізація початкової популяції
vector<Individual> initializePopulation() {
    vector<Individual> population(POPULATION_SIZE);
    for (int i = 0; i < POPULATION_SIZE; ++i) {
        population[i].x = getRandomInt(MIN_VAL, MAX_VAL);
        population[i].y = getRandomInt(MIN_VAL, MAX_VAL);
        population[i].error = calculateEquationError(population[i].x, population[i].y);
        population[i].fitness = calculateFitness(population[i].error);
    }
    return population;
}

// Турнірний відбір (обираємо найкращого з декількох випадкових)
Individual tournamentSelection(const vector<Individual>& population) {
    int tournamentSize = 5;
    Individual best = population[getRandomInt(0, POPULATION_SIZE - 1)];

    for (int i = 1; i < tournamentSize; ++i) {
        Individual contender = population[getRandomInt(0, POPULATION_SIZE - 1)];
        if (contender.fitness > best.fitness) {
            best = contender;
        }
    }
    return best;
}

// Оператор схрещування (одноточкове)
void crossover(const Individual& parent1, const Individual& parent2, Individual& child1, Individual& child2) {
    // Обмінюємося генами (x та y)
    child1.x = parent1.x;
    child1.y = parent2.y;

    child2.x = parent2.x;
    child2.y = parent1.y;
}

// Оператор мутації
void mutate(Individual& ind) {
    if (getRandomDouble() < MUTATION_RATE) {
        // З невеликою ймовірністю замінюємо ген на нове випадкове значення
        if (getRandomDouble() < 0.5) {
            //ind.x = getRandomInt(MIN_VAL, MAX_VAL);
            ind.x += getRandomInt(-2, 2); // Локальний пошук
            if (ind.x > MAX_VAL) ind.x = MAX_VAL;
            if (ind.x < MIN_VAL) ind.x = MIN_VAL;
        }
        else {
            //ind.y = getRandomInt(MIN_VAL, MAX_VAL);
            ind.y += getRandomInt(-2, 2);
            if (ind.y > MAX_VAL) ind.y = MAX_VAL;
            if (ind.y < MIN_VAL) ind.y = MIN_VAL;
        }
    }
}

int main() {
    cout << "--- Genetic Algorithm for solving a Diophantine equation ---" << endl;
    cout << "Equation: x^2 - 4x - y^2 + 6 = 0" << endl;
    cout << "------------------------------------------------------------" << endl;
    vector<Individual> population = initializePopulation();
    Individual bestSolution = population[0];

    int generation = 0;
    bool foundExactSolution = false;

    // Головний цикл генетичного алгоритму
    while (generation < MAX_GENERATIONS) {
        // Шукаємо найкращу особину в поточному поколінні
        for (const auto& ind : population) {
            if (ind.fitness > bestSolution.fitness) {
                bestSolution = ind;
            }
        }

        // Якщо знайшли точний розв'язок (похибка = 0), зупиняємось
        if (bestSolution.error == 0) {
            foundExactSolution = true;
            break;
        }

        // Створення нового покоління
        vector<Individual> newPopulation;

        // Елітизм: зберігаємо найкращу особину
        newPopulation.push_back(bestSolution);

        while (newPopulation.size() < POPULATION_SIZE) {
            // Відбір
            Individual parent1 = tournamentSelection(population);
            Individual parent2 = tournamentSelection(population);

            Individual child1, child2;

            // Схрещування
            crossover(parent1, parent2, child1, child2);

            // Мутація
            mutate(child1);
            mutate(child2);

            // Перерахунок похибки та пристосованості для нащадків
            child1.error = calculateEquationError(child1.x, child1.y);
            child1.fitness = calculateFitness(child1.error);
            child2.error = calculateEquationError(child2.x, child2.y);
            child2.fitness = calculateFitness(child2.error);

            newPopulation.push_back(child1);
            if (newPopulation.size() < POPULATION_SIZE) {
                newPopulation.push_back(child2);
            }
        }

        population = newPopulation;
        generation++;
    }

    // Виведення результатів
    cout << "Stopped at generation: " << generation << endl;
    if (foundExactSolution) {
        cout << "Exact solution found!" << endl;
        cout << "x = " << bestSolution.x << ", y = " << bestSolution.y << endl;
    }
    else {
        cout << "Exact solution not found (generation limit reached)." << endl;
        cout << "Best approximation found:" << endl;
        cout << "x = " << bestSolution.x << ", y = " << bestSolution.y << endl;
        cout << "Error (deviation from 0) = " << bestSolution.error << endl;
    }

    return 0;
}
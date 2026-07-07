#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>

using namespace std;
// Структура даних "Черга"
struct Queue {
	int arr[100];
	int front;
	int rear;
	int capacity;
	int count;
	// Ініціалізація черги
	void init(int size) {
		capacity = size;
		front = 0;
		rear = -1;
		count = 0;
	}
	// Перевірка на порожність
	bool isEmpty() {
		return count == 0;
	}
	// Перевірка на повність
	bool isFull() {
		return count == capacity;
	}
	// Додавання елементу
	void enqueue(int x) {
		if (!isFull()) {
			rear = (rear + 1) % capacity;
			arr[rear] = x;
			count++;
		}
	}
	// Видалення елементу
	int dequeue() {
		if (!isEmpty()) {
			int value = arr[front];
			front = (front + 1) % capacity;
			count--;
			return value;
		}
		return -1;
	}
	// Перегляд першого елементу без видалення
	int peek() {
		if (!isEmpty()) {
			return arr[front];
		}
		return -1;
	}
};
// Перевірка чи є число простим
bool isPrime(int n) {
	if (n <= 1) {
		return false;
	}
	for (int i = 2; i <= sqrt(n); i++) {
		if (n % i == 0) {
			return false;
		}
	}
	return true;
}

int main() {
	// Ініціалізація генератора випадкових чисел
	srand(time(0));

	int variant = 3;
	int S = variant * 5 + 50;
	cout << "My variant: " << variant << ", size: " << S << endl;

	Queue myQueue;
	myQueue.init(S);
	// Заповнення черги випадковими числами від 1 до 1000
	cout << "Main Queue: " << endl;
	for (int i = 0; i < S; i++) {
		int num = rand() % 1000 + 1;
		cout << num << " ";
		myQueue.enqueue(num);
	}
	cout << endl << "--------------------------" << endl;
	// Вибірка елементів та вивід лише простих чисел
	cout << "Prime numbers: " << endl;
	int primeCount = 0;
	// Поки черга не стане порожньою, дістаємо елементи один за одним
	while (!myQueue.isEmpty()) {
		int num = myQueue.dequeue();
		if (isPrime(num)) {
			cout << num << " ";
			primeCount++;
		}
	}
	// Перевірка, чи були взагалі знайдені прості числа
	if (primeCount == 0) {
		cout << "No prime numbers found." << endl;
	}
	else {
		cout << endl;
	}
	return 0;

}
#include <iostream>
#include <cmath>

using namespace std;
// Елементарна одиниця динамічної структури даних
struct Node {
	int data;// Ключ
	Node* next;// Покажчик на наступний вузол
};
// Структура даних: Хеш-таблиця
struct HashTable {
	Node** table;
	int size;
	// Ініціалізація хеш-таблиці
	void init(int s) {
		size = s;
		table = new Node * [size];
		for (int i = 0; i < size; i++) {
			table[i] = nullptr;// Встановлення початкового стану
		}
	}
	// Хеш-функція — метод ділення
	int hashFunction(int key) {
		return abs(key) % size;
	}
	// Метод вставки з використанням методу ланцюжків
	void insert(int key) {
		int index = hashFunction(key);
		Node* newNode = new Node;
		newNode->data = key;
		newNode->next = table[index];// Вирішення колізії
		table[index] = newNode;
	}
	// Метод видалення вузлів за певною ознакою
	bool search(int key) {
		int index = hashFunction(key);
		Node* temp = table[index];
		while (temp != nullptr) {
			if (temp->data == key) {
				return true;
			}
			temp = temp->next;
		}
		return false;
	}
	// Видалення конкретного елемента за ключем
	void remove(int key) {
		int index = hashFunction(key);
		Node* temp = table[index];
		Node* prev = nullptr;
		// Шукаємо вузол у списку
		while (temp != nullptr && temp->data != key) {
			prev = temp;
			temp = temp->next;
		}
		// Якщо не знайшли — виходимо
		if (temp == nullptr) {
			return;
		}
		// Якщо це перший елемент у списку
		if (prev == nullptr) {
			table[index] = temp->next;
		}
		// Якщо елемент десь посередині або в кінці
		else {
			prev->next = temp->next;
		}
		delete temp;
	}
	// Спеціальна функція для видалення всіх парних чисел
	void removeEvens() {
		for (int i  = 0; i < size; i++) {
			Node* temp = table[i];
			Node* prev = nullptr;
			while (temp != nullptr) {
				if (temp->data % 2 == 0) {// Перевірка на парність
					Node* toDelete = temp;
					if (prev == nullptr) {// Видаляємо перший елемент у комірці
						table[i] = temp->next;
						temp = table[i];
					}
					else {// Видаляємо наступний після prev
						prev->next = temp->next;
						temp = temp->next;
					}
					delete toDelete;
				}
				else {// Якщо число непарне, просто йдемо далі
					prev = temp;
					temp = temp->next;
				}
			}
		}
	}
	// Виведення таблиці
	void display() {
		for (int i = 0; i < size; i++) {
			cout << "Index " << i << ": ";
			Node* temp = table[i];
			if (temp == nullptr) {
				cout << "empty";
			}
			while (temp != nullptr) {
				cout << temp->data << " ";
				temp = temp->next;
			}
			cout << endl; //все з нового рядка
		}
	}
};

int main() {
	srand(time(0)); // Щоб числа були завжди різні

	int variant = 3;
	int N = variant * 5 + 50;
	int S = (int)(N * 0.75);

	cout << "Variant: " << variant << endl;
	cout << "N: " << N << endl;
	cout << "S: " << S << endl;

	HashTable myHashTable;
	myHashTable.init(S);

	for (int i = 0; i < N; i++) {
		myHashTable.insert(rand() % 1000 + 1);
	}
	cout << "Hash table before removing even numbers:" << endl;
	myHashTable.display();

	myHashTable.removeEvens();
	cout << "Hash table after removing even numbers:" << endl;
	myHashTable.display();
	return 0;
}
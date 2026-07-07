#include <iostream>
#include <string>
using namespace std;

struct Node {
	string name;
	string phone;
	Node* left;
	Node* right;
	Node* p;
	// Конструктор для створення нового вузла
	Node(string name, string phone) : name(name), phone(phone), left(nullptr), right(nullptr), p(nullptr) {}
};

class Tree {
private:
	Node* root; // Вказівник на корінь дерева

	Node*treeMinimum(Node* x) {
		while (x->left != nullptr) {
			x = x->left; // Рухаємося вліво, поки не знайдемо мінімальний елемент
		}
		return x;
	}
	// Функція для знаходження наступника вузла x
	Node*treeSuccessor(Node* x) {
		if (x->right != nullptr) { 
			return treeMinimum(x->right);
		}
		// Якщо правого піддерева немає, рухаємося вгору по дереву
		Node* y = x->p;
		while (y != nullptr && x == y->right) {
			x = y;
			y = y->p;
		}
		return y;
	}
	// Вивід елементів дерева у відсортованому порядку
	void Sort(Node* x) {
		if (x != nullptr) {
			Sort(x->left);
			cout << x->name << ": " << x->phone << endl;
			Sort(x->right);
		}
	}

public:
	Tree() : root(nullptr) {}
	// Функція для отримання кореня дерева
	Node* getRoot() {
		return root;
	}
	// Функція для виводу телефонної книги
	void display() {
		if (root == nullptr) {
			cout << "Phone book is empty." << endl;
		}
		else {
			Sort(root);
		}
	}
	// Функція для пошуку контакту за ім'ям
	Node* Tree_Search(Node* x, string k) {
		if (x == nullptr || k == x->name) {
			return x;
		}
		if (k < x->name) {
			return Tree_Search(x->left, k);
		}
		else {
			return Tree_Search(x->right, k);
		}
	}
	// Функція для вставки нового контакту в дерево
	void Tree_Insert(string name, string phone) {
		Node* z = new Node(name, phone);
		Node* y = nullptr;
		Node* x = root;
		while (x != nullptr) {
			y = x;
			if (z->name < x->name) {
				x = x->left;
			}
			else {
				x = x->right;
			}
		}
		z->p = y;
		if (y == nullptr) {
			root = z;
		}
		else if (z->name < y->name) {
			y->left = z;
		}
		else {
			y->right = z;
		}
	}
	// Функція для видалення контакту з дерева
	void Delete(string name) {
		Node* z = Tree_Search(root, name);
		if (z == nullptr) {
			return;
		}
		Node *y, *x;
		if (z->left == nullptr || z->right == nullptr) {
			y = z;
		}
		else {
			y = treeSuccessor(z);
		}
		if (y->left != nullptr) {
			x = y->left;
		}
		else {
			x = y->right;
		}
		if (x != nullptr) {
			x->p = y->p;
		}
		if (y->p == nullptr) {
			root = x;
		}
		else if (y == y->p->left) {
			y->p->left = x;
		}
		else {
			y->p->right = x;
		}
		if (y != z) {
			z->name = y->name;
			z->phone = y->phone;
		}
		delete y;
	}

};

int main() {
	Tree tree;
	tree.Tree_Insert("Alice", "123456789");
	tree.Tree_Insert("Bob", "987654321");
	tree.Tree_Insert("Charlie", "555555555");

	int choice;
	string name, phone;

	do {
		cout << "--- Phone Book Menu ---" << endl;
		cout << "1. Display phone book" << endl;
		cout << "2. Search for a contact" << endl;
		cout << "3. Delete a contact" << endl;
		cout << "4. Add new contact" << endl;
		cout << "0. Exit" << endl;
		cout << "--------------------------" << endl;
		cin >> choice;
		

		switch (choice) {

		case 1:
		{
			cout << "--- Current phone book ---" << endl;
			tree.display();
			cout << "--------------------------" << endl;
			break;
		}

		case 2:
		{
			string searchKey;
			cout << "Enter name to search: ";
			cin >> searchKey;
			Node* result = tree.Tree_Search(tree.getRoot(), searchKey);
			if (result) {
				cout << "Found: " << result->name << ": " << result->phone << endl;
			}
			else {
				cout << "Not found" << endl;
			}
			cout << "--------------------------" << endl;
			break;
		}
		case 3:
		{
			string deleteKey;
			cout << "Enter name to delete: ";
			cin >> deleteKey;
			tree.Delete(deleteKey);
			cout << "Contact deleted if it existed." << endl;
			cout << "--------------------------" << endl;
			break;
		}
		case 4:
		{
			cout << "Enter name: ";
			cin >> name;
			cout << "Enter phone number: ";
			cin >> phone;
			tree.Tree_Insert(name, phone);
			cout << "Contact added." << endl;
			cout << "--------------------------" << endl;
			break;
		}
		case 0:
			cout << "Exiting..." << endl;
			break;
		default:
			cout << "Invalid choice. Please try again." << endl;
		}
	} while (choice != 0);

		return 0;
}
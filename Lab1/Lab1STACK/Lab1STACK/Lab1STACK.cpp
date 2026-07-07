#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

struct Stack {
	int arr[100];
	int last;
	int capacity;

	void init(int size) {
		capacity = size;
		last = -1;
	}
	bool isEmpty() {
		return last == -1;
	}
	bool isFull() {
		return last == capacity - 1;
	}
	int size() {
		return last + 1;
	}
	int peek() {
		if (!isEmpty()) {
			return arr[last];
		}
		return -1;
	}
	void push(int x) {
		if (!isFull()) {
			last++;
			arr[last] = x;
		}
	}
	int pop() {
		if (!isEmpty()) {
			int value = arr[last];
			last--;
			return value;
		}
		return -1;
	}
};

int main() {
	setlocale(LC_ALL, "ukr");
	srand(time(0));
	int variant = 3;
	int size = variant * 5 + 50;
	cout << "My variant: " << variant << ", size: " << size << endl;
	
	Stack mainStack, evenStack, oddStack;
	mainStack.init(size);
	evenStack.init(size);
	oddStack.init(size);
	
	cout << "Main Stack: " << endl;
	for (int i = 0; i < size; i++) {
		int num = rand() % 1000 + 1;
		mainStack.push(num);
	}

	while (!mainStack.isEmpty()) {
		int num = mainStack.pop();
		if (num % 2 == 0) {
			evenStack.push(num);
		}
		else {
			oddStack.push(num);
		}
	}
	
	cout << "Even Stack: " << endl;
	while (!evenStack.isEmpty()) {
		cout << evenStack.pop() << " ";
	}
	cout << endl;
	
	cout << "Odd Stack: " << endl;
	while (!oddStack.isEmpty()) {
		cout << oddStack.pop() << " ";
	}
	return 0;
}
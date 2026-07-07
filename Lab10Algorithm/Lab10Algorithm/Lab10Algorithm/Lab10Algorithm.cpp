#include <iostream>
#include <string>
#include <queue>
#include <map>

using namespace std;

// Структура вузла дерева
struct Node {
    string symbol;
    int frequency;
    Node* left;
    Node* right;

    Node(string s, int f) : symbol(s), frequency(f), left(nullptr), right(nullptr) {}
};

// Компаратор для черги
struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->frequency > b->frequency;
    }
};

// Генерація кодів Гаффмена
void generateCodes(Node* root, string code, map<string, string>& huffmanCodes) {
    if (!root) return;

    // Якщо це листок (немає нащадків) - записуємо код
    if (!root->left && !root->right) {
        huffmanCodes[root->symbol] = code;
    }

    generateCodes(root->left, code + "0", huffmanCodes);
    generateCodes(root->right, code + "1", huffmanCodes);
}

// Декодування рядка
string decode(Node* root, string encoded) {
    string result = "";
    Node* curr = root;

    for (char bit : encoded) {
        if (bit == '0') curr = curr->left;
        else            curr = curr->right;

        // Дійшли до листка
        if (!curr->left && !curr->right) {
            result += curr->symbol;
            curr = root;
        }
    }
    return result;
}

int main() {
    // Вхідні дані латинкою (транслитом)
    string text[] = { "45000", "5000", "12000", "13000", "9000", "16000" };
    int n = 6; // загальна кількість символів

    cout << "Tekst dlya koduvannya: burulky buly bulky" << endl;

    // 1. Підрахунок частот
    map<string, int> freqMap;
    for (int i = 0; i < n; i++) {
        freqMap[text[i]]++;
    }

    // 2. Створення пріоритетної черги (вільних вузлів)
    priority_queue<Node*, vector<Node*>, Compare> pq;
    for (auto pair : freqMap) {
        pq.push(new Node(pair.first, pair.second));
    }

    // 3. Побудова дерева Гаффмена
    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();

        // Створюємо батьківський вузол із сумою частот
        Node* parent = new Node(left->symbol + right->symbol, left->frequency + right->frequency);
        parent->left = left;
        parent->right = right;
        pq.push(parent);
    }

    // Корінь дерева
    Node* root = pq.top();

    // 4. Отримання кодів
    map<string, string> huffmanCodes;
    generateCodes(root, "", huffmanCodes);

    // Вивід таблиці кодів
    cout << "Tablytsya kodiv Haffmena:" << endl;
    for (auto pair : freqMap) {
        cout << "'" << pair.first << "' : chastota " << pair.second
            << " -> kod " << huffmanCodes[pair.first] << endl;
    }

    // 5. Кодування тексту
    string encodedString = "";
    for (int i = 0; i < n; i++) {
        encodedString += huffmanCodes[text[i]];
    }

    cout << "Zakodovanyi rezultat:" << encodedString << endl;

    // 6. Декодування тексту (перевірка)
    string decodedString = decode(root, encodedString);
    cout << "Dekodovanyi rezultat:" << decodedString << endl;

    return 0;
}
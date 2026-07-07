#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Структура для опису заявки: номер, початок і кінець інтервалу
struct Activity {
	int id;
	int start;
	int finish;
};

// Компаратор, що забезпечує стратегію жадібного вибору: вибирати заявку з найранішим часом завершення
bool compareActivities(Activity a, Activity b) {
	return a.finish < b.finish;
}

// Ркурсивна функція вибору заявок
void recursiveActivitySelector(vector<Activity>& activities, int i, int n, vector<Activity>& selected) {
	int m = i + 1;
	// Шукаємо першу сумісну заявку
	while (m <= n && activities[m].start < activities[i].finish) {
		m++;
	}
	if (m <= n) {
		selected.push_back(activities[m]);
		recursiveActivitySelector(activities, m, n, selected);
	}
}

int main() {
	// Вхідні дані
	vector<Activity> activities = {
		{0,0,0}, // Фіктивна(технічна) заявка для старту рукурурсії
		{1,3,5},
		{2,0,6},
		{3,12,14},
		{4,2,13},
		{5,8,12},
		{6,8,11},
		{7,1,3},
		{8,5,7},
		{9,5,9},
		{10,6,10},
		{11,3,8},
	};

	// Підготовка до жадібного вибору: сортуємо заявки за часом завершення
	sort(activities.begin() + 1, activities.end(), compareActivities);
	
	vector<Activity> selected;

	// Початок виконання алгоритму: викликаємо рекурсивну функцію від 0-го елемента
	recursiveActivitySelector(activities, 0, activities.size() - 1, selected);

	//Виводимо результат
	cout << "Selected activities: " << selected.size() << endl;
	for (const auto& activity : selected) {
		cout << "(" << activity.start << ", " << activity.finish << ") ";
	}

	return 0;

}
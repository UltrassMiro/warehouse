#include "Warehouse.h"
#include <iostream>
#include <algorithm>
using namespace std;

// Додає новий предмет до списку
void Warehouse::addItem(const Item& item) {
    items.push_back(item);
}
// Видаляє предмет зі списку
void Warehouse::removeItem(int id) {
    items.erase(remove_if(items.begin(), items.end(),
                               [id](Item& i){ return i.getId() == id; }), items.end());
}
// Пошук за назвою
Item* Warehouse::findItemByName(const string& name) {
    for (auto& i : items)
        if (i.getName() == name) return &i;
    return nullptr;
}
// Пошук за ID
Item* Warehouse::findItemById(int id) {
    for (auto& i : items)
        if (i.getId() == id) return &i;
    return nullptr;
}
// Оновлення інфо
void Warehouse::updateItem(int id, const string& name, int quantity, const string& category) {
    Item* item = findItemById(id);
    if (item) {
        item->setName(name);
        item->setQuantity(quantity);
        item->setCategory(category);
    }
}
// Вивід всіх предметів в БД
void Warehouse::displayAll() {
    for (auto& i : items) {
        cout << i.getId() << " | "
            << i.getName() << " | "
            << i.getQuantity() << " | "
            << i.getCategory() << "\n";
    }
}
// Вивід всіх предметів в БД за обраною категорією
void Warehouse::displayByCategory(const string& category) {
    for (auto& i : items) {
        if (i.getCategory() == category) {
            cout << i.getId() << " | "
                << i.getName() << " | "
                << i.getQuantity() << "\n";
        }
    }
}
// Вивід кількості предметів в БД
int Warehouse::getTotalItems() {
    int sum = 0;
    for (auto& i : items) sum += i.getQuantity();
    return sum;
}
// Вивід кількості предметів в БД за категорією
int Warehouse::getCategoryCount(const string& category) {
    int sum = 0;
    for (auto& i : items)
        if (i.getCategory() == category)
            sum += i.getQuantity();
    return sum;
}

vector<Item>& Warehouse::getItems() {
    return items;
}
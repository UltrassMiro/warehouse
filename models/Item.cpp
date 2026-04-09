#include "Item.h"
using namespace std;

int Item::nextId = 1;

// Конструктор для створення нового товару
// ID автоматично збільшується
Item::Item(string name, int quantity, string category)
        : id(nextId++), name(name), quantity(quantity), category(category) {}

// Конструктор для завантаження з файлу
Item::Item(int id, string name, int quantity, string category)
        : id(id), name(name), quantity(quantity), category(category) {
    if (id >= nextId) nextId = id + 1;
}

// Гетери
int Item::getId() const { return id; }
string Item::getName() const { return name; }
int Item::getQuantity() const { return quantity; }
string Item::getCategory() const { return category; }

// Сетери
void Item::setName(const string& name) { this->name = name; }
void Item::setQuantity(int quantity) { this->quantity = quantity; }
void Item::setCategory(const string& category) { this->category = category; }

// Формує рядок у CSV
string Item::toCSV() const {
    return to_string(id) + "," + name + "," +
           to_string(quantity);
}
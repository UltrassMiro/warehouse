#ifndef ITEM_H
#define ITEM_H

#include <string>
using namespace std;

// Представляє одиницю предметів на складі
class Item {
private:
    int id;
    string name;
    int quantity;
    string category;

    static int nextId;

public:
    // Конструктор для створення нового товару
    Item(string name, int quantity, string category);
    // Конструктор для завантаження з файлу
    Item(int id, string name, int quantity, string category);

    // Геттери отримують інфо про предмет
    int getId() const;
    string getName() const;
    int getQuantity() const;
    string getCategory() const;

    // Сетери задають предмету значення
    void setName(const string& name);
    void setQuantity(int quantity);
    void setCategory(const string& category);

    // Перетворення інфо у CSV для БД
    string toCSV() const;
};

#endif
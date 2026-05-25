#include "FileManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

// Перевіряє, чи можна безпечно перетворити текст у число

static bool safeStoi(const string& text, int& value) {
    try {
        if (text.empty()) {
            return false;
        }

        size_t pos = 0;
        value = stoi(text, &pos);

        return pos == text.size();
    }
    catch (...) {
        return false;
    }
}

FileManager::FileManager(const string& filePath) : filePath(filePath) {}

// Зберігає всі товари у warehouse.csv
void FileManager::save(Warehouse& warehouse) {

    ofstream file(filePath);

    if (!file.is_open())
        return;

    for (const Item& item : warehouse.getItems()) {

        file << item.getId() << ";"
        << item.getName() << ";"
        << item.getQuantity() << ";"
        << item.getCategory() << endl;
    }

    file.close();
}

// Завантажує дані з warehouse.csv
void FileManager::load(Warehouse& warehouse) {

    ifstream file(filePath);

    if (!file.is_open())
        return;

    warehouse.clear();

    string line;

    while (getline(file, line)) {

        if (line.empty())
            continue;

        stringstream ss(line);

        string idText;
        string name;
        string quantityText;
        string category;

        getline(ss, idText, ';');
        getline(ss, name, ';');
        getline(ss, quantityText, ';');
        getline(ss, category, ';');

        int id = 0;
        int quantity = 0;

    // Пропускає пошкоджені записи
        if (!safeStoi(idText, id))
            continue;

        if (!safeStoi(quantityText, quantity))
            continue;

        if (name.empty() || category.empty())
            continue;

        warehouse.addItem(Item(id, name, quantity, category));
    }

    file.close();
}
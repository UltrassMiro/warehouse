#include "Warehouse.h"
#include <iostream>
#include <algorithm>

using namespace std;

void Warehouse::addItem(const Item& item) {
    items.push_back(item);
}

void Warehouse::removeItem(int id) {
    items.erase(
        remove_if(items.begin(), items.end(),
                  [id](const Item& item) {
                      return item.getId() == id;
                  }),
        items.end()
    );
}

Item* Warehouse::findItemByName(const string& name) {
    for (auto& item : items) {
        if (item.getName() == name) {
            return &item;
        }
    }
    return nullptr;
}

Item* Warehouse::findItemById(int id) {
    for (auto& item : items) {
        if (item.getId() == id) {
            return &item;
        }
    }
    return nullptr;
}

void Warehouse::updateItem(int id, const string& name, int quantity, const string& category) {
    Item* item = findItemById(id);

    if (item != nullptr) {
        item->setName(name);
        item->setQuantity(quantity);
        item->setCategory(category);
    }
}

void Warehouse::displayAll() {
    for (auto& item : items) {
        cout << item.getId() << " | "
             << item.getName() << " | "
             << item.getQuantity() << " | "
             << item.getCategory() << endl;
    }
}

void Warehouse::displayByCategory(const string& category) {
    for (auto& item : items) {
        if (item.getCategory() == category) {
            cout << item.getId() << " | "
                 << item.getName() << " | "
                 << item.getQuantity() << endl;
        }
    }
}

int Warehouse::getTotalItems() {
    int sum = 0;

    for (auto& item : items) {
        sum += item.getQuantity();
    }

    return sum;
}

int Warehouse::getCategoryCount(const string& category) {
    int sum = 0;

    for (auto& item : items) {
        if (item.getCategory() == category) {
            sum += item.getQuantity();
        }
    }

    return sum;
}

vector<Item>& Warehouse::getItems() {
    return items;
}

const vector<Item>& Warehouse::getItems() const {
    return items;
}

void Warehouse::clear() {
    items.clear();
}
#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include <vector>
#include <string>
#include "../models/Item.h"

using namespace std;

class Warehouse {
private:
    vector<Item> items;

public:
    void addItem(const Item& item);
    void removeItem(int id);

    Item* findItemByName(const string& name);
    Item* findItemById(int id);

    void updateItem(int id, const string& name, int quantity, const string& category);

    void displayAll();
    void displayByCategory(const string& category);

    int getTotalItems();
    int getCategoryCount(const string& category);

    vector<Item>& getItems();
    const vector<Item>& getItems() const;

    void clear();
};

#endif
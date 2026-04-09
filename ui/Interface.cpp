#include "Interface.h"
#include "../utils/InputHelper.h"
#include "../file/ExcelExporter.h"
#include <iostream>
using namespace std;

// Конструктор при запуску читає дані з файлів
Interface::Interface() {
    fileManager.load(warehouse);
}

// Меню
void Interface::showMenu() {
    cout << "   WAREHOUSE MANAGEMENT SYSTEM\n";
    cout << "[1] Show all\n";
    cout << "[2] By category\n";
    cout << "[3] Add item\n";
    cout << "[4] Delete item\n";
    cout << "[5] Update item\n";
    cout << "[6] Search\n";
    cout << "[7] Category count\n";
    cout << "[8] Total count\n";
    cout << "[9] Export Excel\n";
    cout << "[0] Exit\n";
}

void Interface::run() {
    int choice;

    do {
        showMenu();
        choice = InputHelper::getInt("Choice: ");

        if (choice == 1) {
            warehouse.displayAll();}


        else if (choice == 2) {
            string cat = InputHelper::getString("Category: ");
            warehouse.displayByCategory(cat);
        }

        else if (choice == 3) {
            string name = InputHelper::getString("Name: ");
            int qty = InputHelper::getInt("Quantity: ");
            string cat = InputHelper::getString("Category: ");

            warehouse.addItem(Item(name, qty, cat));
            fileManager.save(warehouse);
        }

        else if (choice == 4) {
            int id = InputHelper::getInt("ID: ");
            warehouse.removeItem(id);
            fileManager.save(warehouse);
        }

        else if (choice == 5) {
            int id = InputHelper::getInt("ID: ");
            string name = InputHelper::getString("New name: ");
            int qty = InputHelper::getInt("New quantity: ");
            string cat = InputHelper::getString("New category: ");

            warehouse.updateItem(id, name, qty, cat);
            fileManager.save(warehouse);
        }

        else if (choice == 6) {
            string name = InputHelper::getString("Name: ");
            Item* item = warehouse.findItemByName(name);

            if (item)
                cout << item->getId() << " | " << item->getName()
                    << " | " << item->getQuantity()
                    << " | " << item->getCategory() << "\n";
            else
                cout << "Not found\n";
        }

        else if (choice == 7) {
            string cat = InputHelper::getString("Category: ");
            cout << "Count: " << warehouse.getCategoryCount(cat) << "\n";
        }

        else if (choice == 8) {
            cout << "Total: " << warehouse.getTotalItems() << "\n";
        }

        else if (choice == 9) {
            fileManager.save(warehouse);
            ExcelExporter::exportToExcel(warehouse);
            cout << "Exported!\n";
        }

    } while (choice != 0);
}
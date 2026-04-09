#include "FileManager.h"
#include <fstream>
#include <sstream>
#include <map>
#include <set>
using namespace std;


string FileManager::getFile(const string& category) {
    return "../data/" + category + ".csv";
}

void FileManager::save(Warehouse& warehouse) {

    // Отримання інфо про категорії
    set<string> categories;
    for (auto& item : warehouse.getItems()) {
        categories.insert(item.getCategory());
    }

    // Чистка файлів
    for (auto& cat : categories) {
        ofstream file(getFile(cat), ios::trunc);
    }

    // Збереження
    map<string, ofstream> files;

    for (auto& item : warehouse.getItems()) {
        string fileName = getFile(item.getCategory());

        if (!files[fileName].is_open())
            files[fileName].open(fileName, ios::app);

        files[fileName] << item.toCSV() << "\n";
    }
}
// Перебір усіх файлів у папці data
void FileManager::load(Warehouse& warehouse) {
    vector<string> categories = {"Tools","Paints","Screws"};

    for (auto& cat : categories) {
        ifstream file(getFile(cat));
        string line;

        while (getline(file, line)) {
            stringstream ss(line);
            string id, name, qty;

            getline(ss, id, ',');
            getline(ss, name, ',');
            getline(ss, qty, ',');

            warehouse.addItem(Item(
                    stoi(id),
                    name,stoi(qty),cat
            ));
        }
    }
}
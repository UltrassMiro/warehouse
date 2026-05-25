#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include "../core/Warehouse.h"

using namespace std;

class FileManager {
private:
    string filePath;

public:
    explicit FileManager(const string& filePath = "warehouse.csv");

    void save(Warehouse& warehouse);
    void load(Warehouse& warehouse);
};

#endif
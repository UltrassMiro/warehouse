#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "../core/Warehouse.h"
#include <map>
using namespace std;

// Збереження/читання БД
class FileManager {
private:
    string getFile(const string& category);

public:
    void save(Warehouse& warehouse);
    void load(Warehouse& warehouse);
};

#endif
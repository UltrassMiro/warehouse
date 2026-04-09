#ifndef INTERFACE_H
#define INTERFACE_H

#include "../core/Warehouse.h"
#include "../file/FileManager.h"

class Interface {
private:
    Warehouse warehouse;
    FileManager fileManager;

    void showMenu();

public:
    Interface();
    void run();
};

#endif
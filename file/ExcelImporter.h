#ifndef EXCEL_IMPORTER_H
#define EXCEL_IMPORTER_H

#include <string>
#include "../core/Warehouse.h"

class ExcelImporter {
public:
    static void importFromExcel(Warehouse& warehouse, const std::string& filePath);
};

#endif
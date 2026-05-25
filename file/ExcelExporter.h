#ifndef EXCEL_EXPORTER_H
#define EXCEL_EXPORTER_H

#include <string>
#include "../core/Warehouse.h"

class ExcelExporter {
public:
    static void exportToExcel(Warehouse& warehouse, const std::string& filePath);
};

#endif
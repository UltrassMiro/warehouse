#ifndef EXCEL_EXPORTER_H
#define EXCEL_EXPORTER_H

#include "../core/Warehouse.h"

class ExcelExporter {
public:
    static void exportToExcel(Warehouse& warehouse);
};

#endif
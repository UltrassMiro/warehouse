#include "ExcelExporter.h"
#ifdef OpenXLSX_FOUND
#include <OpenXLSX.hpp>
using namespace OpenXLSX;
#endif

void ExcelExporter::exportToExcel(Warehouse& warehouse) {
#ifdef OpenXLSX_FOUND
    XLDocument doc;
    doc.create("warehouse.xlsx");

    auto sheet = doc.workbook().worksheet("Sheet1");

    int row = 1;
    for (auto& item : warehouse.getItems()) {
        sheet.cell(row, 1).value() = item.getId();
        sheet.cell(row, 2).value() = item.getName();
        sheet.cell(row, 3).value() = item.getQuantity();
        sheet.cell(row, 4).value() = item.getCategory();
        row++;
    }

    doc.save();
    doc.close();
#endif
}
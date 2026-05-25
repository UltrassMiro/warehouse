#include "ExcelImporter.h"
#include <stdexcept>
#include <string>
#include <vector>

#ifdef OPENXLSX_FOUND
#include <OpenXLSX.hpp>
using namespace OpenXLSX;
#endif

using namespace std;

void ExcelImporter::importFromExcel(Warehouse& warehouse, const string& filePath) {
#ifndef OPENXLSX_FOUND
    throw runtime_error("OpenXLSX is not connected. Excel import is impossible.");
#else
    XLDocument doc;
    doc.open(filePath);

    vector<string> sheetNames = doc.workbook().worksheetNames();

    for (const string& sheetName : sheetNames) {
        auto sheet = doc.workbook().worksheet(sheetName);

        int row = 2;

        while (true) {
            string name;

            try {
                name = sheet.cell(row, 2).value().get<string>();
            } catch (...) {
                break;
            }

            if (name.empty()) {
                break;
            }

            int id = sheet.cell(row, 1).value().get<int>();
            int quantity = sheet.cell(row, 3).value().get<int>();

            string category;

            try {
                category = sheet.cell(row, 4).value().get<string>();
            } catch (...) {
                category = sheetName;
            }

            if (category.empty()) {
                category = sheetName;
            }

            warehouse.addItem(Item(id, name, quantity, category));

            row++;
        }
    }

    doc.close();
#endif
}
#include "ExcelExporter.h"
#include <stdexcept>
#include <map>
#include <vector>
#include <string>

#ifdef OPENXLSX_FOUND
#include <OpenXLSX.hpp>
using namespace OpenXLSX;
#endif

using namespace std;

static string safeSheetName(string name) {
    for (char& ch : name) {
        if (ch == '/' || ch == '\\' || ch == '?' || ch == '*' ||
            ch == '[' || ch == ']' || ch == ':') {
            ch = '_';
        }
    }

    if (name.empty()) {
        name = "Other";
    }

    if (name.size() > 31) {
        name = name.substr(0, 31);
    }

    return name;
}

static void writeHeader(XLWorksheet& sheet) {
    sheet.cell(1, 1).value() = "ID";
    sheet.cell(1, 2).value() = "Name";
    sheet.cell(1, 3).value() = "Quantity";
    sheet.cell(1, 4).value() = "Category";
}

static void writeItem(XLWorksheet& sheet, int row, const Item& item) {
    sheet.cell(row, 1).value() = item.getId();
    sheet.cell(row, 2).value() = item.getName();
    sheet.cell(row, 3).value() = item.getQuantity();
    sheet.cell(row, 4).value() = item.getCategory();
}

void ExcelExporter::exportToExcel(Warehouse& warehouse, const string& filePath) {
#ifndef OPENXLSX_FOUND
    throw runtime_error("OpenXLSX is not connected. Excel export is impossible.");
#else
    XLDocument doc;
    doc.create(filePath);

    vector<string> categories = {
        "Tools",
        "Screws and nuts",
        "Paints",
        "Uniform"
    };

    map<string, vector<Item>> groupedItems;

    for (const string& category : categories) {
        groupedItems[category] = {};
    }

    groupedItems["Other"] = {};

    for (const Item& item : warehouse.getItems()) {
        string category = item.getCategory();

        if (groupedItems.find(category) != groupedItems.end()) {
            groupedItems[category].push_back(item);
        } else {
            groupedItems["Other"].push_back(item);
        }
    }

    bool firstSheet = true;

    for (const auto& pair : groupedItems) {
        string sheetName = safeSheetName(pair.first);
        XLWorksheet sheet;

        if (firstSheet) {
            sheet = doc.workbook().worksheet("Sheet1");
            sheet.setName(sheetName);
            firstSheet = false;
        } else {
            doc.workbook().addWorksheet(sheetName);
            sheet = doc.workbook().worksheet(sheetName);
        }

        writeHeader(sheet);

        int row = 2;

        for (const Item& item : pair.second) {
            writeItem(sheet, row, item);
            row++;
        }
    }

    doc.save();
    doc.close();
#endif
}
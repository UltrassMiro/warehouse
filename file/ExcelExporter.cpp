#include "ExcelExporter.h"
#include <map>
#include <vector>
#include <string>
#ifdef OPENXLSX_FOUND
#include <OpenXLSX.hpp>
using namespace OpenXLSX;
using namespace std;
#endif

// Замінює заборонені символи, щоб Excel не падав при створенні листів
static string safeSheetName(string name) {
    for (char& ch : name) {

        if (
                ch == '/'
                ||
                ch == '\\'
                ||
                ch == '?'
                ||
                ch == '*'
                ||
                ch == '['
                ||
                ch == ']'
                ||
                ch == ':'
                ) {
            ch = '_';
        }
    }

    if (name.empty())
    {
        name = "Other";
    }

    if (name.size() > 31)
    {
        name = name.substr(0, 31);
    }
    return name;
}

// Створює заголовки таблиці
static void writeHeader(XLWorksheet& sheet)
{
    sheet.cell(1, 1).value() = "ID";
    sheet.cell(1, 2).value() = "Name";
    sheet.cell(1, 3).value() = "Quantity";
    sheet.cell(1, 4).value() = "Category";
}

// Записує один товар у рядок Excel
static void writeItem(
    XLWorksheet& sheet, int row, const Item& item )
{
    sheet.cell(row,1).value() = item.getId();
    sheet.cell(row,2).value() = item.getName();
    sheet.cell(row,3).value() = item.getQuantity();
    sheet.cell(row,4).value() = item.getCategory();
}

// Експортує дані в Excel файл
// Для кожної категорії створюється окремий лист
void ExcelExporter::exportToExcel(Warehouse& warehouse, const string& filePath)
{

#ifndef OPENXLSX_FOUND throw runtime_error("OpenXLSX not connected.");
#else

    XLDocument doc;
    doc.create(filePath);

    vector<string>
            categories = {
            "Tools",
            "Screws and nuts",
            "Paints",
            "Uniform",
            "Other"
    };

    map<string, vector<Item>> groupedItems;

// Створює всі категорії
    for (const string& category : categories) {
            groupedItems[category] = {};
    }

// Розподіляє товари по категоріях
    for (const Item& item : warehouse.getItems()) {
            string category = item.getCategory();

        if (groupedItems.find(category) != groupedItems.end()) {
                groupedItems[category].push_back(item);
        }

        else {
            groupedItems["Other"].push_back(item);
        }
    }

    bool firstSheet = true;

    for (const auto& pair : groupedItems) {
        string sheetName = safeSheetName(pair.first);

        XLWorksheet sheet;

        if (firstSheet) {
            sheet = doc.workbook().worksheet("Sheet1");
            sheet.setName( sheetName );

            firstSheet = false;
        }

        else {
            doc.workbook().addWorksheet(sheetName);
            sheet = doc.workbook().worksheet(sheetName);
        }

        writeHeader(
                sheet
        );

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
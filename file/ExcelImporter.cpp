#include "ExcelImporter.h"

#include <stdexcept>
#include <string>
#include <vector>

#ifdef OPENXLSX_FOUND

#include <OpenXLSX.hpp>

using namespace OpenXLSX;

#endif

using namespace std;

/*
 Вирівнює назви категорій,
 щоб імпорт не ламався
 через різні назви листів
*/
static string normalizeCategoryName(
        const string& sheetName
) {

    if (
            sheetName ==
            "Screws_and_nuts"
            ) {

        return
                "Screws and nuts";
    }

    return sheetName;
}

/*
 Імпортує всі листи Excel,
 оновлює існуючі товари,
 додає нові
*/
void ExcelImporter::importFromExcel(

        Warehouse& warehouse,

        const string& filePath

) {

#ifndef OPENXLSX_FOUND

    throw runtime_error(
            "OpenXLSX not connected."
    );

#else

    XLDocument doc;

    doc.open(
            filePath
    );

    vector<string>
            sheetNames =

            doc
                    .workbook()
                    .worksheetNames();

    for (

            const string&
            sheetName :

            sheetNames

            ) {

        auto sheet =

                doc
                        .workbook()
                        .worksheet(
                                sheetName
                        );

        string category =

                normalizeCategoryName(
                        sheetName
                );

        int row = 2;

        while (true) {

            string name;

            try {

                name =

                        sheet

                                .cell(
                                        row,
                                        2
                                )

                                .value()

                                .get<string>();

            } catch (...) {

                break;
            }

            if (
                    name.empty()
                    )
                break;

            int id = 0;
            int quantity = 0;

            try {

                id =

                        sheet

                                .cell(
                                        row,
                                        1
                                )

                                .value()

                                .get<int>();

            } catch (...) {

                id =
                        row - 1;
            }

            try {

                quantity =

                        sheet

                                .cell(
                                        row,
                                        3
                                )

                                .value()

                                .get<int>();

            } catch (...) {

                quantity = 0;
            }

            /*
             Якщо товар існує —
             оновлюємо

             Якщо ні —
             додаємо
            */

            Item* existingItem =

                    warehouse
                            .findItemById(
                                    id
                            );

            if (
                    existingItem
                    != nullptr
                    ) {

                warehouse.updateItem(

                        id,

                        name,

                        quantity,

                        category
                );

            } else {

                warehouse.addItem(

                        Item(

                                id,

                                name,

                                quantity,

                                category
                        )
                );
            }

            row++;
        }
    }

    doc.close();

#endif
}
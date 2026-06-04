#include <gtest/gtest.h>
#include <fstream>
#include <cstdio>
#include "../models/Item.h"
#include "../core/Warehouse.h"
#include "../file/FileManager.h"

using namespace std;

TEST(WarehouseBusinessLogicTests, AddItemAndFindById_ReturnsCorrectItem) {
    Warehouse warehouse;

    Item item(1, "Laptop", 10, "Electronics");
    warehouse.addItem(item);

    Item* foundItem = warehouse.findItemById(1);

    ASSERT_NE(foundItem, nullptr);
    EXPECT_EQ(foundItem->getId(), 1);
    EXPECT_EQ(foundItem->getName(), "Laptop");
    EXPECT_EQ(foundItem->getQuantity(), 10);
    EXPECT_EQ(foundItem->getCategory(), "Electronics");
}

TEST(WarehouseBusinessLogicTests, GetTotalItems_ReturnsSumOfAllQuantities) {
    Warehouse warehouse;

    warehouse.addItem(Item(1, "Laptop", 10, "Electronics"));
    warehouse.addItem(Item(2, "Mouse", 25, "Electronics"));
    warehouse.addItem(Item(3, "Table", 5, "Furniture"));

    int total = warehouse.getTotalItems();

    EXPECT_EQ(total, 40);
}

TEST(FileManagerTests, SaveAndLoad_ValidData_RestoresWarehouseItems) {
    const string testFile = "test_warehouse.csv";

    Warehouse sourceWarehouse;
    sourceWarehouse.addItem(Item(1, "Laptop", 10, "Electronics"));
    sourceWarehouse.addItem(Item(2, "Table", 5, "Furniture"));

    FileManager fileManager(testFile);
    fileManager.save(sourceWarehouse);

    Warehouse loadedWarehouse;
    fileManager.load(loadedWarehouse);

    ASSERT_EQ(loadedWarehouse.getItems().size(), 2);

    EXPECT_EQ(loadedWarehouse.getItems()[0].getId(), 1);
    EXPECT_EQ(loadedWarehouse.getItems()[0].getName(), "Laptop");
    EXPECT_EQ(loadedWarehouse.getItems()[0].getQuantity(), 10);
    EXPECT_EQ(loadedWarehouse.getItems()[0].getCategory(), "Electronics");

    EXPECT_EQ(loadedWarehouse.getItems()[1].getId(), 2);
    EXPECT_EQ(loadedWarehouse.getItems()[1].getName(), "Table");
    EXPECT_EQ(loadedWarehouse.getItems()[1].getQuantity(), 5);
    EXPECT_EQ(loadedWarehouse.getItems()[1].getCategory(), "Furniture");

    remove(testFile.c_str());
}

TEST(FileManagerTests, Load_CorruptedRows_SkipsInvalidRecords) {
    const string testFile = "test_corrupted_warehouse.csv";

    ofstream file(testFile);
    file << "1;Laptop;10;Electronics\n";
    file << "abc;Mouse;5;Electronics\n";
    file << "2;Table;wrong;Furniture\n";
    file << "3;;7;Furniture\n";
    file << "4;Chair;12;\n";
    file.close();

    Warehouse warehouse;
    FileManager fileManager(testFile);

    fileManager.load(warehouse);

    ASSERT_EQ(warehouse.getItems().size(), 1);
    EXPECT_EQ(warehouse.getItems()[0].getId(), 1);
    EXPECT_EQ(warehouse.getItems()[0].getName(), "Laptop");
    EXPECT_EQ(warehouse.getItems()[0].getQuantity(), 10);
    EXPECT_EQ(warehouse.getItems()[0].getCategory(), "Electronics");

    remove(testFile.c_str());
}
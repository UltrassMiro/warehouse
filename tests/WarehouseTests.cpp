#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <vector>
#include <string>

#include "../models/Item.h"
#include "../core/Warehouse.h"

using namespace std;
using namespace testing;

class IFileStorage {
public:
    virtual ~IFileStorage() = default;

    virtual void saveItems(const std::vector<Item>& items) = 0;
    virtual std::vector<Item> loadItems() = 0;
};

class MockFileStorage : public IFileStorage {
public:
    MOCK_METHOD(void, saveItems, (const vector<Item>& items), (override));
    MOCK_METHOD(vector<Item>, loadItems, (), (override));
};

class WarehouseFileService {
private:
    IFileStorage& storage;

public:
    explicit WarehouseFileService(IFileStorage& storage) : storage(storage) {}

    void saveWarehouse(const Warehouse& warehouse) {
        storage.saveItems(warehouse.getItems());
    }

    void loadWarehouse(Warehouse& warehouse) {
        warehouse.clear();

        vector<Item> loadedItems = storage.loadItems();

        for (const Item& item : loadedItems) {
            warehouse.addItem(item);
        }
    }
};

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

TEST(FileStorageMockTests, SaveWarehouse_CallsStorageSaveWithCorrectItems) {
    Warehouse warehouse;

    warehouse.addItem(Item(1, "Laptop", 10, "Electronics"));
    warehouse.addItem(Item(2, "Table", 5, "Furniture"));

    MockFileStorage mockStorage;
    WarehouseFileService fileService(mockStorage);

    EXPECT_CALL(mockStorage, saveItems(_))
        .Times(1)
        .WillOnce([](const vector<Item>& items) {
            ASSERT_EQ(items.size(), 2);

            EXPECT_EQ(items[0].getId(), 1);
            EXPECT_EQ(items[0].getName(), "Laptop");
            EXPECT_EQ(items[0].getQuantity(), 10);
            EXPECT_EQ(items[0].getCategory(), "Electronics");

            EXPECT_EQ(items[1].getId(), 2);
            EXPECT_EQ(items[1].getName(), "Table");
            EXPECT_EQ(items[1].getQuantity(), 5);
            EXPECT_EQ(items[1].getCategory(), "Furniture");
        });

    fileService.saveWarehouse(warehouse);
}

TEST(FileStorageMockTests, LoadWarehouse_UsesMockedDataAndFillsWarehouse) {
    MockFileStorage mockStorage;
    WarehouseFileService fileService(mockStorage);

    vector<Item> mockedItems = {
        Item(1, "Laptop", 10, "Electronics"),
        Item(2, "Chair", 7, "Furniture")
    };

    EXPECT_CALL(mockStorage, loadItems())
        .Times(1)
        .WillOnce(Return(mockedItems));

    Warehouse warehouse;

    fileService.loadWarehouse(warehouse);

    ASSERT_EQ(warehouse.getItems().size(), 2);

    EXPECT_EQ(warehouse.getItems()[0].getId(), 1);
    EXPECT_EQ(warehouse.getItems()[0].getName(), "Laptop");
    EXPECT_EQ(warehouse.getItems()[0].getQuantity(), 10);
    EXPECT_EQ(warehouse.getItems()[0].getCategory(), "Electronics");

    EXPECT_EQ(warehouse.getItems()[1].getId(), 2);
    EXPECT_EQ(warehouse.getItems()[1].getName(), "Chair");
    EXPECT_EQ(warehouse.getItems()[1].getQuantity(), 7);
    EXPECT_EQ(warehouse.getItems()[1].getCategory(), "Furniture");
}